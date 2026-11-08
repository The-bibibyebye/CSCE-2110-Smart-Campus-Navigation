// Purpose: Save the details of one facility

#ifndef FACILITY_H
#define FACILITY_H
using namespace std;
#include <string>


struct Facility {
    string facilityId; // Use capital letters, like F004
    string facilityName; 
    string buildingName;
    string category;
};

#endif