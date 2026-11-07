#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

// Graph implementation for campus navigation

#include <string>
#include <vector>

class CampusGraph {
    private:
        std::vector<std::string> buildings; // List of buildings on campus
        std::vector<std::vector<int>> adjacencyList; // Adjacency list representation of the graph
        int findBuildingIndex(const std::string& name) const; // Helper function to find the index of a building


    public:
        bool addBuilding(const std::string& name); // Add a building to the graph
        bool addConnection(const std::string& buildingA, const std::string& buildingB); // Add a connection between two buildings
        void displayGraph() const; // Display the graph 
        void displayStatistics() const; // Display graph statistics
        void displayReachable(const std::string& startingBuilding) const; // Display reachable buildings from a given building
};

#endif