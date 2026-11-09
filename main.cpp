// Initial implementation by: Deepsan Shrestha
// Facility integration by: Member A
// Department integration and loading validation by: Member C
// Purpose: Load campus data and coordinate the Milestone 1 menu.

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "include/CampusGraph.h"
#include "include/Hashtable.h"
#include "include/DepartmentTree.h"

using namespace std;

namespace {

// Remove outer ASCII whitespace without changing spaces inside names.
string trimField(const string& text) {
    size_t first = text.find_first_not_of(" \t\r\n\f\v");
    if (first == string::npos) return "";
    size_t last = text.find_last_not_of(" \t\r\n\f\v");
    return text.substr(first, last - first + 1);
}

// Match names without regard to ASCII capitalization.
string nameKey(const string& text) {
    string result = trimField(text);
    for (char& c : result) {
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
    }
    return result;
}

// Return the accepted spelling, or an empty string for an unknown name.
string acceptedName(const vector<string>& names, const string& value) {
    const string key = nameKey(value);
    for (const string& name : names) {
        if (nameKey(name) == key) return name;
    }
    return "";
}

// Split all fields, including an empty field after a trailing pipe.
vector<string> splitFields(const string& line) {
    vector<string> fields;
    size_t start = 0;
    while (true) {
        size_t separator = line.find('|', start);
        fields.push_back(trimField(line.substr(start, separator - start)));
        if (separator == string::npos) break;
        start = separator + 1;
    }
    return fields;
}

// Print a rejection with the source file and physical line number.
void warning(const string& filename, size_t line, const string& reason) {
    cout << "Warning: " << filename << ':' << line << ": " << reason << '\n';
}

// Read accepted buildings in file order and retain their original spelling.
vector<string> loadBuildings(CampusGraph& campus, const string& filename) {
    vector<string> names;
    ifstream file(filename);
    size_t accepted = 0, rejected = 0, lineNumber = 0;
    if (!file) cout << "Error: Could not open " << filename << '\n';
    string line;
    while (getline(file, line)) {
        ++lineNumber;
        line = trimField(line);
        if (line.empty() || line[0] == '#') continue;
        string error;
        if (line.find('|') != string::npos) {
            error = "Expected one building-name field.";
        }
        else if (!campus.addBuilding(line)) {
            error = "Duplicate building: " + line;
        }
        else {
            names.push_back(line);
            ++accepted;
            continue;
        }
        ++rejected;
        warning(filename, lineNumber, error);
    }
    cout << filename << ": " << accepted << " accepted, "
         << rejected << " rejected\n";
    return names;
}

// Add undirected connections without changing neighbor insertion order.
void loadConnections(CampusGraph& campus, const vector<string>& buildings,
                     const string& filename) {
    ifstream file(filename);
    size_t accepted = 0, rejected = 0, lineNumber = 0;
    if (!file) cout << "Error: Could not open " << filename << '\n';
    string line;
    while (getline(file, line)) {
        ++lineNumber;
        line = trimField(line);
        if (line.empty() || line[0] == '#') continue;
        vector<string> fields = splitFields(line);
        string error;
        if (fields.size() != 2) {
            error = "Expected BuildingA|BuildingB.";
        }
        else if (fields[0].empty() || fields[1].empty()) {
            error = "Building fields must be nonempty.";
        }
        else {
            string a = acceptedName(buildings, fields[0]);
            string b = acceptedName(buildings, fields[1]);
            if (a.empty() || b.empty()) {
                error = "Unknown building: " + (a.empty() ? fields[0] : fields[1]);
            }
            else if (a == b) {
                error = "Self-connections are not allowed: " + a;
            }
            else if (!campus.addConnection(a, b)) {
                error = "Duplicate connection: " + a + " | " + b;
            }
            else {
                ++accepted;
                continue;
            }
        }
        ++rejected;
        warning(filename, lineNumber, error);
    }
    cout << filename << ": " << accepted << " accepted, "
         << rejected << " rejected\n";
}

// Validate facility records; ID lookup and duplicate checks use the hash table.
void loadFacilities(HashTable& facilities, const vector<string>& buildings,
                    const string& filename) {
    ifstream file(filename);
    vector<string> categories;
    size_t accepted = 0, rejected = 0, lineNumber = 0;
    if (!file) cout << "Error: Could not open " << filename << '\n';
    string line;
    while (getline(file, line)) {
        ++lineNumber;
        line = trimField(line);
        if (line.empty() || line[0] == '#') continue;
        vector<string> fields = splitFields(line);
        string error;
        if (fields.size() != 4) {
            error = "Expected FacilityID|FacilityName|BuildingName|Category.";
        }
        else if (fields[0].empty() || fields[1].empty() ||
                 fields[2].empty() || fields[3].empty()) {
            error = "All facility fields must be nonempty.";
        }
        else if (!facilities.checkId(fields[0])) {
            error = "Invalid ID: use F followed by exactly three digits.";
        }
        else {
            string building = acceptedName(buildings, fields[2]);
            if (building.empty()) {
                error = "Unknown building: " + fields[2];
            }
            else {
                string category = acceptedName(categories, fields[3]);
                Facility facility;
                facility.facilityId = fields[0];
                facility.facilityName = fields[1];
                facility.buildingName = building;
                facility.category = category.empty() ? fields[3] : category;
                if (facilities.addFacility(facility)) {
                    if (category.empty()) categories.push_back(facility.category);
                    ++accepted;
                    continue;
                }
                error = "Duplicate facility ID: " + fields[0];
            }
        }
        ++rejected;
        warning(filename, lineNumber, error);
    }
    cout << filename << ": " << accepted << " accepted, "
         << rejected << " rejected | hash capacity: " << facilities.getTableSize()
         << " | entries: " << facilities.getFacilityCount()
         << " | collisions: " << facilities.getCollisionCount() << '\n';
}

} // namespace

// Coordinate ordered loading and the Milestone 1 menu.
int main(int argc, char* argv[]) {
    const string dataDirectory = argc > 1 ? argv[1] : "data";
    CampusGraph campus;
    HashTable facilities;
    DepartmentTree departments;

    vector<string> buildings = loadBuildings(campus, dataDirectory + "/buildings.txt");
    loadConnections(campus, buildings, dataDirectory + "/connections.txt");
    loadFacilities(facilities, buildings, dataDirectory + "/facilities.txt");
    // Load departments fourth.
    departments.load(dataDirectory + "/departments.txt", cout);

    cout << "Smart Campus Navigation and Facility Analytics System"
         << endl;

    int choice = -1;

    do {
        cout << "\n===== SMART CAMPUS NAVIGATION MENU =====\n";
        cout << "1. View all facilities\n";
        cout << "2. Search facility by ID\n";
        cout << "3. View campus graph\n";
        cout << "4. Display graph counts and degrees\n";
        cout << "5. Find reachable buildings (BFS)\n";
        cout << "6. Add department\n";
        cout << "7. Search department\n";
        cout << "8. Display department hierarchy\n";
        cout << "9. Run preorder traversal\n";
        cout << "10. Exit\n";
        cout << "0. Exit (alias)\n";
        cout << "Enter your choice: ";

        string input;

        if (!getline(cin, input)) {
            break;
        }

        stringstream menuInput(input);
        int parsedChoice;
        char extra;

        // Reject blank input, letters, and trailing characters.
        if (!(menuInput >> parsedChoice) || (menuInput >> extra)) {
            cout << "Invalid input. Enter 1 through 10, or 0 to exit.\n";
            continue;
        }

        choice = parsedChoice;

        if (choice == 1) {
            facilities.displayFacilities();
        }
        else if (choice == 2) {
            string facilityId;
            Facility result;

            cout << "Enter facility ID: ";

            if (!getline(cin, facilityId)) {
                break;
            }

            if (!facilities.checkId(facilityId)) {
                cout << "Invalid ID. Use F and three digits, like F004.\n";
            }
            else if (facilities.findFacility(facilityId, result)) {
                cout << result.facilityId << " | "
                     << result.facilityName << " | "
                     << result.buildingName << " | "
                     << result.category << '\n';
            }
            else {
                cout << "Facility not found: " << facilityId << '\n';
            }
        }
        else if (choice == 3) {
            campus.displayGraph();
        }
        else if (choice == 4) {
            campus.displayStatistics();
        }
        else if (choice == 5) {
            string startingBuilding;

            cout << "Enter the starting building: ";

            if (!getline(cin, startingBuilding)) {
                break;
            }

            if (startingBuilding.find_first_not_of(
                    " \t\r\n\f\v") == string::npos) {
                cout << "Error: Building name cannot be blank.\n";
            }
            else {
                campus.displayReachable(trimField(startingBuilding));
            }
        }
        else if (choice == 6) {
            departments.promptAdd(cin, cout);
        }
        else if (choice == 7) {
            string name;

            cout << "Enter department name: ";

            if (!getline(cin, name)) {
                break;
            }

            departments.displayDepartment(name, cout);
        }
        else if (choice == 8) {
            departments.displayHierarchy(cout);
        }
        else if (choice == 9) {
            departments.displayPreorder(cout);
        }
        else if (choice == 10 || choice == 0) {
            cout << "Exiting the program.\n";
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 10 && choice != 0);

    return 0;
}
