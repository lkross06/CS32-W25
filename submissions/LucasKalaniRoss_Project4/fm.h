#ifndef FM_H

#define FM_H

#include "bstset.h"
#include "provided.h"

class FlightManager : public FlightManagerBase {
public:
    //load the flight data from a given file with the specified format
    bool load_flight_data(std::string filename);
    
    //return a vector of all flights whose departure time is between start_time (inclusive) and end_time (exclusive)
    //The returned flights must be ordered by ascending departure time
    std::vector<FlightSegment> find_flights(std::string source_airport, int start_time, int end_time) const;
    
private:
    //Source Airport points to set of Flight Segments ordered by departure time
    std::unordered_map<std::string, BSTSet<FlightSegment>*> _departures;
};

#endif //for FM_H
