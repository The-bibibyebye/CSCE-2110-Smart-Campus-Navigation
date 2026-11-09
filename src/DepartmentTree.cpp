// Purpose: Implements department loading, search, and tree traversals.
// Author: Member C, with AI assistance.

#include "../include/DepartmentTree.h"
#include <fstream>
#include <istream>
#include <ostream>

namespace {

// Remove outer whitespace while preserving spaces within names.
std::string trimDepartment(const std::string& text) {
    const std::size_t first =
        text.find_first_not_of(" \t\r\n\f\v");

    if (first == std::string::npos) {
        return "";
    }

    const std::size_t last =
        text.find_last_not_of(" \t\r\n\f\v");

    return text.substr(first, last - first + 1);
}

// Normalize a name for case-insensitive comparisons.
std::string departmentKey(const std::string& text) {
    std::string result = trimDepartment(text);

    for (char& character : result) {
        if (character >= 'A' && character <= 'Z') {
            character += 'a' - 'A';
        }
    }

    return result;
}

}

// Search the current node, then its children in insertion order.
DepartmentTree::Node* DepartmentTree::find(
    Node* node, const std::string& name) {

    if (node == nullptr) {
        return nullptr;
    }

    if (departmentKey(node->name) == departmentKey(name)) {
        return node;
    }

    for (const auto& child : node->children) {
        Node* match = find(child.get(), name);

        if (match != nullptr) {
            return match;
        }
    }

    return nullptr;
}

// Validate a department before attaching it to the tree.
bool DepartmentTree::addDepartment(
    const std::string& name,
    const std::string& parent,
    std::string& error) {

    error.clear();
    const std::string cleanName = trimDepartment(name);

    if (cleanName.empty()) {
        error = "Department name cannot be blank.";
        return false;
    }

    if (find(root.get(), cleanName) != nullptr) {
        error = "Duplicate department: " + cleanName;
        return false;
    }

    if (empty()) {
        if (departmentKey(parent) != "none") {
            error = "The first department must have parent NONE.";
            return false;
        }

        root.reset(new Node(cleanName, nullptr));
        return true;
    }

    Node* parentNode = find(root.get(), parent);

    if (parentNode == nullptr) {
        error = "Unknown parent department: " + trimDepartment(parent);
        return false;
    }

    parentNode->children.emplace_back(
        new Node(cleanName, parentNode));

    return true;
}

// Load departments in file order and report rejected records.
void DepartmentTree::load(
    const std::string& filename, std::ostream& out) {

    std::ifstream file(filename);
    std::size_t accepted = 0;
    std::size_t rejected = 0;
    std::size_t lineNumber = 0;

    if (!file) {
        out << "Error: Could not open " << filename << '\n';
    }

    std::string line;

    while (std::getline(file, line)) {
        ++lineNumber;
        line = trimDepartment(line);

        // A comment begins with # after removing outer whitespace.
        if (line.empty() || line[0] == '#') {
            continue;
        }

        const std::size_t separator = line.find('|');
        std::string error;

        if (separator == std::string::npos ||
            line.find('|', separator + 1) != std::string::npos) {
            error = "Expected DepartmentName|ParentName.";
        }
        else {
            const std::string name =
                trimDepartment(line.substr(0, separator));

            const std::string parent =
                trimDepartment(line.substr(separator + 1));

            if (name.empty() || parent.empty()) {
                error = "Department and parent fields must be nonempty.";
            }
            else if (addDepartment(name, parent, error)) {
                ++accepted;
                continue;
            }
        }

        ++rejected;
        out << "Warning: " << filename << ':' << lineNumber
            << ": " << error << '\n';
    }

    out << filename << ": " << accepted << " accepted, "
        << rejected << " rejected\n";
}

// Display the matching department's parent, child count, and depth.
void DepartmentTree::displayDepartment(
    const std::string& name, std::ostream& out) const {

    if (trimDepartment(name).empty()) {
        out << "Error: Department name cannot be blank.\n";
        return;
    }

    const Node* node = find(root.get(), name);

    if (node == nullptr) {
        out << "Department not found: "
            << trimDepartment(name) << '\n';
        return;
    }

    std::size_t depth = 0;

    for (const Node* parent = node->parent;
         parent != nullptr;
         parent = parent->parent) {
        ++depth;
    }

    out << "Name: " << node->name << '\n';
    out << "Parent: "
        << (node->parent ? node->parent->name : "None") << '\n';
    out << "Direct children: " << node->children.size() << '\n';
    out << "Depth: " << depth << '\n';
}

// Visit each department before its children, indenting by depth.
void DepartmentTree::hierarchy(
    const Node* node, std::size_t depth, std::ostream& out) {

    out << std::string(depth * 4, ' ') << node->name << '\n';

    for (const auto& child : node->children) {
        hierarchy(child.get(), depth + 1, out);
    }
}

// Display the hierarchy or an empty-tree message.
void DepartmentTree::displayHierarchy(std::ostream& out) const {
    if (empty()) {
        out << "No departments are loaded.\n";
        return;
    }

    hierarchy(root.get(), 0, out);
}

// Build a flat preorder sequence in sibling insertion order.
void DepartmentTree::preorder(
    const Node* node, std::ostream& out, bool& first) {

    if (!first) {
        out << " -> ";
    }

    out << node->name;
    first = false;

    for (const auto& child : node->children) {
        preorder(child.get(), out, first);
    }
}

// Display preorder without indentation.
void DepartmentTree::displayPreorder(std::ostream& out) const {
    if (empty()) {
        out << "No departments are loaded.\n";
        return;
    }

    bool first = true;
    preorder(root.get(), out, first);
    out << '\n';
}

// Read a department addition without modifying the input file.
void DepartmentTree::promptAdd(
    std::istream& in, std::ostream& out) {

    std::string name;
    std::string parent = "NONE";
    std::string error;

    out << "Enter department name: ";

    if (!std::getline(in, name)) {
        return;
    }

    if (trimDepartment(name).empty()) {
        out << "Error: Department name cannot be blank.\n";
        return;
    }

    if (find(root.get(), name) != nullptr) {
        out << "Error: Duplicate department: "
            << trimDepartment(name) << '\n';
        return;
    }

    // An empty tree creates its root without asking for a parent.
    if (!empty()) {
        out << "Enter parent department: ";

        if (!std::getline(in, parent)) {
            return;
        }

        if (trimDepartment(parent).empty()) {
            out << "Error: Parent name cannot be blank.\n";
            return;
        }
    }

    if (addDepartment(name, parent, error)) {
        out << "Department added: " << trimDepartment(name) << '\n';
    }
    else {
        out << "Error: " << error << '\n';
    }
}
