// Purpose: Save facilities and find them by ID.
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include "Facility.h"
using namespace std;

class HashTable{
    private:
    int collisionCount;
    int facilityCount;
    int tableSize;
    vector<vector<Facility>> facilityGroups; // Groups used to find a facility
    vector<Facility> facilityList; // Facilities in the order they were added
    int getGroupNumber(string& facilityId); // Find the group number for an ID


public:
    HashTable(int size = 11); // Make an empty table
    bool addFacility(Facility& facility); // Return false for a bad ID or an ID already saved
    bool findFacility(string facilityId, Facility& result);
    void displayFacilities();
    bool checkId(string& facilityId);
    int getTableSize();
    int getCollisionCount();
    int getFacilityCount();

};



#endif