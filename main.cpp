#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "include/CampusGraph.h"


using namespace std;

int main(){
    // Creates a CampusGraph object
    CampusGraph campus;

    // Loads buildings from file
    ifstream buildingFile("data/buildings.txt");
    string buildingName;

    if (!buildingFile.is_open()) {
        cout << "Error: Could not open buildings.txt" << endl;
    }
    else {
        while (getline(buildingFile, buildingName)) {
            if (buildingName.empty() || buildingName[0] == '#') {
                continue;
            }

            campus.addBuilding(buildingName);
        }

        buildingFile.close();
    }
    // Load connections from file
    ifstream connectionFile("data/connections.txt");
    string connectionLine;

    if (!connectionFile.is_open()) {
        cout << "Error: Could not open connections.txt" << endl;
    }
    else {
        while (getline(connectionFile, connectionLine)) {

            // Skips empty lines and comments
            if (connectionLine.empty() || connectionLine[0] == '#') {
                continue;
            }

            // Finds the separator between building names
            size_t separator = connectionLine.find('|');

            if (separator == string::npos) {
                cout << "Invalid connection: " << connectionLine << endl;
                continue;
            }

            string buildingA = connectionLine.substr(0, separator);
            string buildingB = connectionLine.substr(separator + 1);

            campus.addConnection(buildingA, buildingB);
        }

        connectionFile.close();
    }



    int choice= 0;
    cout << "Smart Campus Navigation and Facility Analytics System" << endl;

    // menu loop
    do {
        cout << "\n===== SMART CAMPUS NAVIGATION MENU =====" << endl;
        cout << "1. View all facilities" << endl;
        cout << "2. Search facility by ID" << endl;
        cout << "3. View campus graph" << endl;
        cout << "4. Display graph counts and degrees" << endl;
        cout << "5. Find reachable buildings (BFS)" << endl;
        cout << "6. Add department" << endl;
        cout << "7. Search department" << endl;
        cout << "8. Display department hierarchy" << endl;
        cout << "9. Run preorder traversal" << endl;
        cout << "10. Exit" << endl;

        cout << "Enter your choice: ";
 
        // Checks for invalid input
        string input;
        if (!getline(cin, input)) {
            break;
        }

        stringstream ss(input);
        char extra;

        if (!(ss >> choice) || (ss >> extra)) {
            cout << "Invalid input. Please enter a number from 1 to 10." << endl;
            continue;
        }

        // Handles user choices
        if (choice==3){
            campus.displayGraph();
        }
        
        else if (choice==4){
            campus.displayStatistics();
        }

        else if (choice==5){
            string startingBuilding;
            cout << "Enter the starting building: ";
            getline(cin, startingBuilding);
            

                if (startingBuilding.find_first_not_of(" \t\n\r") == string::npos) {
                    cout << "Error: Building name cannot be blank." << endl;
                }
                else{
                    campus.displayReachable(startingBuilding);
                }
        }

        else if (choice==10){
            cout << "Exiting the program." << endl;
        }

        else if (choice < 1 || choice > 10) {
            cout << "Invalid choice. Please try again." << endl;
        }

        else {
            cout << "This feature will be implemented by another team member." << endl;
        }       

    } while (choice != 10);



    
    return 0;
}