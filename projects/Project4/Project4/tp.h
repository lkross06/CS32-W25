#ifndef TP_H

#define TP_H

#include "fm.h"
#include <string>
#include <unordered_set>

class TravelPlanner : public TravelPlannerBase {
public:
    TravelPlanner(const FlightManagerBase& flight_manager, const AirportDB& airport_db) : TravelPlannerBase(flight_manager, airport_db) {}
    
    void add_preferred_airline(std::string airline);
    
    bool plan_travel(std::string source_airport, std::string destination_airport, int start_time, Itinerary& itinerary) const;
    
    
private:
    std::unordered_set<std::string> _pref_airlines;
    
    bool is_preferred_airline(std::string airline) const;
};

#endif //for TP_H
