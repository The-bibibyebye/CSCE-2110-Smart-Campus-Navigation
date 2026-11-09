// Purpose: Defines the general department tree.
// Author: Member C, with AI assistance.

#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

class DepartmentTree {
private:
    struct Node {
        std::string name;
        Node* parent;
        std::vector<std::unique_ptr<Node>> children;

        // Store the department name and its parent.
        Node(const std::string& name, Node* parent)
            : name(name), parent(parent) {}
    };

    std::unique_ptr<Node> root;

    // Find a department by name.
    static Node* find(Node* node, const std::string& name);

    // Print the hierarchy with indentation.
    static void hierarchy(const Node* node, std::size_t depth,
                          std::ostream& out);

    // Visit a department before its children.
    static void preorder(const Node* node, std::ostream& out,
                         bool& first);

public:
    // Check whether the tree has no root.
    bool empty() const { return !root; }

    // Add a department or report a validation error.
    bool addDepartment(const std::string& name,
                       const std::string& parent,
                       std::string& error);

    // Load department records from a file.
    void load(const std::string& filename, std::ostream& out);

    // Display a department's details.
    void displayDepartment(const std::string& name,
                           std::ostream& out) const;

    // Display the indented department hierarchy.
    void displayHierarchy(std::ostream& out) const;

    // Display the preorder sequence without indentation.
    void displayPreorder(std::ostream& out) const;

    // Ask the user for a new department.
    void promptAdd(std::istream& in, std::ostream& out);
};

#endif
