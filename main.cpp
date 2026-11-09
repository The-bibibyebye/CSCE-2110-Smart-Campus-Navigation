// Initial implementation by: Deepsan Shrestha
// Facility integration by: Member A
// Department integration by: Member C, 
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

// Load campus data, then process menu operations until exit.
int main(int argc, char* argv[]) {
    const string dataDirectory = argc > 1 ? argv[1] : "data";

    CampusGraph campus;
    HashTable facilities;
    DepartmentTree departments;
    vector<string> buildingList;

    // Load buildings first.
    ifstream buildingFile(dataDirectory + "/buildings.txt");
    string buildingName;

    if (!buildingFile.is_open()) {
        cout << "Error: Could not open buildings.txt" << endl;
    }
    else {
        while (getline(buildingFile, buildingName)) {
            if (buildingName.empty() || buildingName[0] == '#') {
                continue;
            }

            bool added = campus.addBuilding(buildingName);

            if (added) {
                size_t first =
                    buildingName.find_first_not_of(" \t\r\n\f\v");
                size_t last =
                    buildingName.find_last_not_of(" \t\r\n\f\v");

                string savedName =
                    buildingName.substr(first, last - first + 1);

                buildingList.push_back(savedName);
            }
        }

        buildingFile.close();
    }

    // Load connections second.
    ifstream connectionFile(dataDirectory + "/connections.txt");
    string connectionLine;

    if (!connectionFile.is_open()) {
        cout << "Error: Could not open connections.txt" << endl;
    }
    else {
        while (getline(connectionFile, connectionLine)) {
            if (connectionLine.empty() || connectionLine[0] == '#') {
                continue;
            }

            size_t separator = connectionLine.find('|');

            if (separator == string::npos) {
                cout << "Invalid connection: "
                     << connectionLine << endl;
                continue;
            }

            string buildingA = connectionLine.substr(0, separator);
            string buildingB = connectionLine.substr(separator + 1);

            campus.addConnection(buildingA, buildingB);
        }

        connectionFile.close();
    }

    // Load facilities third.
    ifstream facilityFile(dataDirectory + "/facilities.txt");
    string facilityLine;
    int accepted = 0;
    int rejected = 0;

    if (!facilityFile.is_open()) {
        cout << "Error: Could not open facilities.txt" << endl;
    }
    else {
        while (getline(facilityFile, facilityLine)) {
            if (!facilityLine.empty() && facilityLine.back() == '\r') {
                facilityLine.pop_back();
            }

            if (facilityLine.empty() || facilityLine[0] == '#') {
                continue;
            }

            int separatorCount = 0;

            for (char character : facilityLine) {
                if (character == '|') {
                    ++separatorCount;
                }
            }

            if (separatorCount != 3) {
                ++rejected;
                cout << "Wrong number of fields." << endl;
                continue;
            }

            stringstream record(facilityLine);
            Facility newFacility;

            getline(record, newFacility.facilityId, '|');
            getline(record, newFacility.facilityName, '|');
            getline(record, newFacility.buildingName, '|');
            getline(record, newFacility.category, '|');

            if (newFacility.facilityName.empty() ||
                newFacility.buildingName.empty() ||
                newFacility.category.empty()) {
                ++rejected;
                cout << "Missing facility information." << endl;
                continue;
            }

            if (facilities.addFacility(newFacility)) {
                ++accepted;
            }
            else {
                ++rejected;
                cout << "Could not add facility: "
                     << newFacility.facilityId << endl;
            }
        }

        facilityFile.close();
    }

    cout << "facilities.txt: " << accepted << " accepted, "
         << rejected << " rejected" << endl;

    cout << "Table size: " << facilities.getTableSize()
         << " | Facilities: " << facilities.getFacilityCount()
         << " | Collisions: " << facilities.getCollisionCount()
         << endl;

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
                campus.displayReachable(startingBuilding);
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
