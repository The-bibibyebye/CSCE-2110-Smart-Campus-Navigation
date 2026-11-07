#include "../include/CampusGraph.h"
#include <iostream>

int main(){

    CampusGraph campus;

    //Add Buildings
    campus.addBuilding("Library");
    campus.addBuilding("Student Union");
    campus.addBuilding("Frisco Campus");
    campus.addBuilding("Discovery Park");

    //Add Connections
    campus.addConnection("Library", "Student Union");
    campus.addConnection("Library", "Frisco Campus");

    //Display Graph
    campus.displayGraph();

    // Test building names
    std::cout << "\nTesting building names:\n";
    std::cout << campus.addBuilding("library") << "\n";
    std::cout << campus.addBuilding("  LIBRARY  ") << "\n";
    std::cout << campus.addBuilding("New Building") << "\n";
    // Empty and whitespace-only building names
    std::cout << campus.addBuilding("") << "\n";
    std::cout << campus.addBuilding("   ") << "\n";
    std::cout << campus.addBuilding("\t\n") << "\n";

    
    std::cout << "\nTesting invalid connections:\n";
    // Connection to a building that doesn't exist
    std::cout << campus.addConnection("Library", "Unknown") << "\n";
    // Connecting a building to itself
    std::cout << campus.addConnection("Library", "Library") << "\n";
    // Duplicate connection
    std::cout << campus.addConnection("Library", "Student Union") << "\n";
    // Duplicate connection in reverse order
    std::cout << campus.addConnection("Student Union", "Library") << "\n";

    //Testing empty graph
    std::cout << "\nTesting empty graph:\n";
    CampusGraph emptyCampus;
    emptyCampus.displayGraph();

    //Testing graph statistics
    std::cout << "\nTesting graph statistics:\n";
    campus.displayStatistics();
    //Testing empty graph statistics
    std::cout << "\nTesting empty graph statistics:\n";
    emptyCampus.displayStatistics();

    //Testing reachable buildings from a given starting building
    std::cout << "\nTesting BFS from Library:\n";
    campus.displayReachable("Library");
    //Testing reachable buildings isolated building
    std::cout << "\nTesting BFS from isolated building:\n";
    campus.displayReachable("Discovery Park");
    //Testing reachable buildings from a building that doesn't exist
    std::cout << "\nTesting BFS from unknown building:\n";
    campus.displayReachable("Unknown Building");


    // Test BFS with longer paths
    std::cout << "\nTesting BFS with longer paths:\n";
    // Add new connections
    campus.addConnection("Student Union", "Discovery Park");
    campus.addConnection("Discovery Park", "New Building");
    // Run BFS again using the updated graph
    campus.displayReachable("Library");

    
    std::cout << "\nTesting case-insensitive BFS:\n";
    campus.displayReachable("  library  ");
    std::cout << "\nTesting BFS on empty graph:\n";
    emptyCampus.displayReachable("Library");

    return 0;
}