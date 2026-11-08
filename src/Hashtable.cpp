// Purpose: Save facilities and find them by ID.
#include "Hashtable.h"
#include <iostream>           // For printing
using namespace std;

// Constructor (save the size and start both counts at zero)
HashTable::HashTable(int size){
    if (size <= 0) {
        size = 11;
    }
    tableSize = size;
    collisionCount = 0;
    facilityCount = 0;
    facilityGroups.resize(tableSize);
}

// Getter functions (just return the value)
int HashTable::getTableSize(){
    return tableSize;
}
int HashTable::getCollisionCount(){
    return collisionCount;
}
int HashTable::getFacilityCount(){
    return facilityCount;
}

// Check for a space, tab, or line break
static bool isSpace(char character){
    if (character == ' ' || character == '\t' || character == '\r') {
        return true;
    }
    if (character == '\n' || character == '\f' || character == '\v') {
        return true;
    }
    return false;
}

// Remove spaces at both ends and change small letters to capital letters
static string fixId(string facilityId){
    // Remove spaces and tabs from the front
    while (facilityId.empty() == false) {
        char firstChar = facilityId[0];
        if (isSpace(firstChar) == false) {
            break;
        }
        facilityId.erase(0, 1);
    }

    // Remove spaces and tabs from the end
    while (facilityId.empty() == false) {
        char lastChar = facilityId.back();
        if (isSpace(lastChar) == false) {
            break;
        }
        facilityId.pop_back();
    }

    // Change small letters to capital letters, like f004 to F004
    for (size_t i = 0; i < facilityId.size(); i++){
        if (facilityId[i] >= 'a' && facilityId[i] <= 'z') {
            facilityId[i] = facilityId[i] - 'a' + 'A';
        }
    }
    return facilityId;
}

// Find the group number for this ID
int HashTable::getGroupNumber(string& facilityId){
    unsigned int number = 0;
    for (size_t i = 0; i < facilityId.size(); i++){
        int charValue = facilityId[i];
        number = number * 31;
        number = number + charValue;
        number = number % tableSize;
    }
    return number;
}

// Check that the ID starts with F and has three digits, like F004
bool HashTable::checkId(string& facilityId){
    facilityId = fixId(facilityId);

    if (facilityId.size() != 4) {
        return false;
    }
    if (facilityId[0] != 'F') {
        return false;
    }
    for (int i = 1; i < 4; i++){
        if (facilityId[i] < '0' || facilityId[i] > '9') {
            return false;
        }
    }
    return true;
}

// Add a facility to the table
bool HashTable::addFacility(Facility& facility){
    // Make a copy so we do not change the original facility
    Facility copyFacility = facility;
    bool valid = checkId(copyFacility.facilityId);
    if (valid == false) {
        return false;
    }

    int groupNumber = getGroupNumber(copyFacility.facilityId);

    // Check if this ID is already in the group
    for (size_t i = 0; i < facilityGroups[groupNumber].size(); i++){
        string savedId = facilityGroups[groupNumber][i].facilityId;
        if (savedId == copyFacility.facilityId) {
            return false;
        }
    }

    // Count one collision if this group already has a facility
    // The same ID does not add to the count
    if (facilityGroups[groupNumber].empty() == false) {
        collisionCount = collisionCount + 1;
    }
    facilityGroups[groupNumber].push_back(copyFacility);
    facilityList.push_back(copyFacility);
    facilityCount = facilityCount + 1;
    return true;
}

// Find the facility and save it in result
bool HashTable::findFacility(string facilityId, Facility& result){
    bool valid = checkId(facilityId);
    if (valid == false) {
        return false;
    }

    int groupNumber = getGroupNumber(facilityId);
    for (size_t i = 0; i < facilityGroups[groupNumber].size(); i++){
        string savedId = facilityGroups[groupNumber][i].facilityId;
        if (savedId == facilityId) {
            result = facilityGroups[groupNumber][i];
            return true;
        }
    }
    return false;
}

// Print every facility in the order it was added
void HashTable::displayFacilities(){
    if (facilityList.empty()) {
        cout << "No facilities found." << endl;
    }

    for (size_t i = 0; i < facilityList.size(); i++){
        Facility facility = facilityList[i];
        cout << facility.facilityId << " | " << facility.facilityName
             << " | " << facility.buildingName << " | " << facility.category << endl;
    }
    cout << "Total facilities: " << facilityCount << endl;
}

