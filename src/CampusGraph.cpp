// Author: Deepsan Shrestha
// Purpose: Implements campus graph operations, statistics, and BFS.

#include "../include/CampusGraph.h"
#include <string>
#include <cctype>
#include <iostream>
#include <queue>

//trims spaces and line breaks
std::string trim(const std::string& name) {
    //finds the first non-whitespace character in the name
    size_t first= name.find_first_not_of(" \t\n\r");

    // Returns an empty string if the name is only whitespace
    if (first== std::string::npos){
        return ""; 
    }

    //finds the last non-whitespace character in the name
    size_t last= name.find_last_not_of(" \t\n\r");

    // Extracts the substring 
    return name.substr(first,last-first+1);
}

//standardizes the building name for comparison
std::string normalizeName(const std::string& name) {
    
    // Trims the name 
    std::string result= trim(name); // Trims the name

    // Converts the substring to lowercase
    for (char& c: result){
       c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    
    return result; // Returns the standardized name
}

//finds the index of a building by its name
int CampusGraph::findBuildingIndex(const std::string& name) const {

    std::string target= normalizeName(name); // Normalizes the building name

    for (int i=0; i< static_cast<int>(buildings.size()); i++){
        if(normalizeName(buildings[i])==target){
            return i; // Returns the index if found
        }
    
    } 

    return -1; // Returns -1 if not found
}


//adds a building to the graph
bool CampusGraph::addBuilding(const std:: string& name){

    std::string trimmedName= trim(name); // Trims the building name
    
    if(trimmedName.empty()){
        return false; // Returns false if the trimmed name is empty
    }

    if(findBuildingIndex(trimmedName)!= -1){
        return false; // Returns false if the building already exists
    }

    buildings.push_back(trimmedName); // Adds the building name to the list
    adjacencyList.push_back({}); // Adds an empty adjacency list for the new building


    return true;
}

//adds a connection between two buildings
bool CampusGraph::addConnection(const std::string& buildingA, const std::string& buildingB) {
    int indexA= findBuildingIndex(buildingA); // Finds the index of building A
    int indexB= findBuildingIndex(buildingB); // Finds the index of building B

    if(indexA== -1 || indexB== -1){
        return false; // Returns false if either building does not exist
    }

    if (indexA== indexB){
        return false; // Returns false if the buildings are the same
    }

    for (int neighbor: adjacencyList[indexA]){
        if(neighbor== indexB){
            return false; // Returns false if the connection already exists
        }
    }

    // Adds the connection in both directions (undirected graph)
    adjacencyList[indexA].push_back(indexB);
    adjacencyList[indexB].push_back(indexA);

    return true;
}

// Displays the graph
void CampusGraph::displayGraph() const {

    //Checks if there are no buildings in the graph
    if(buildings.empty()){
        std::cout<< "Campus graph is empty.\n";
        return;
    }

    //goes through each building and display its connections
    for (int i=0; i< static_cast<int>(buildings.size()); i++){

        std::cout<< buildings[i]<< ": ";

        // Checks if the building has no connections
        if(adjacencyList[i].empty()){
            std::cout<< "(no connections)"; // Displays if there are no connections
        } 
        
        else {
            for (int j=0; j< static_cast<int>(adjacencyList[i].size()); j++){
                int neighborIndex= adjacencyList[i][j];
                std::cout<< buildings[neighborIndex]; // Displays the connected building

                if(j< static_cast<int>(adjacencyList[i].size())-1){
                    std::cout<< ", "; // Adds a comma between connections
                }
            }
        }

        std::cout<< "\n"; // New line after each building's connections
    }   
}


// Displays graph statistics
void CampusGraph::displayStatistics() const {
    // Total number of buildings
    int totalBuildings= static_cast<int>(buildings.size()); 
    std::cout<< "Total buildings: "<< totalBuildings<< "\n";

    int totalDegree=0; // Initialize total degree

    //for degree of each building
    std::cout<<"\nDegree of each building:\n";
    for (int i=0; i< totalBuildings; i++){
        int degree= static_cast<int>(adjacencyList[i].size()); // Degree of the building
        totalDegree+= degree; // Add to total tally
        std::cout<< buildings[i]<< ": "<< degree<< "\n"; // Displays the degree
    }
    
    //Total number of connections (edges) in the graph
    int totalConnections= totalDegree/2; // handshake principle
    std::cout<< "\nTotal connections: "<< totalConnections<< "\n"; // Displays the total connections
}

// Displays reachable buildings from a given starting building
void CampusGraph::displayReachable(const std::string& startingBuilding) const{

    //Finds index of the starting building
    int startIndex= findBuildingIndex(startingBuilding);

    //Runs if the starting building doesn't exist in the graph
    if (startIndex== -1){
        std::cout<< "Building not found.\n";
        return;
    }

    //queue to Process buildings in BFS manner
    std::queue<int> q;

    //tracks which buildings have been visited
    std::vector<bool> visited(buildings.size(), false);
    //tracks distance from the starting building
    std::vector<int> distance(buildings.size(), -1);

    int reachableCount=0; // Initialize reachable buildings count

    //intilializes the starting building
    visited[startIndex]= true;
    distance[startIndex]= 0;
    q.push(startIndex);

    //BFS traversal to find reachable buildings
    while(!q.empty()){
        //Processes the current building
        int current = q.front();
        q.pop();
        reachableCount++;
        std::cout<<buildings[current]<< " - Distance: "<< distance[current]<< "\n"; // Displays the current building and its distance
        

        //Explores all neighbors of the current building
        for(int neighbor: adjacencyList[current]){
            if(!visited[neighbor]){
                visited[neighbor]= true; // Marks the neighbor as visited
                distance[neighbor]= distance[current]+1; // Updates the distance
                q.push(neighbor); // Adds the neighbor to the queue for further exploration
            }
        }
    }

    std::cout<<"\nReachable buildings: "<< reachableCount << " out of "<< buildings.size() << "\n"; // Displays the total reachable buildings
}