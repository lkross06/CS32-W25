#include "fm.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

//custom comparison operators for flight segments via departure time
bool operator<(const FlightSegment& fs1, const FlightSegment& fs2){
    if (fs1.departure_time == fs2.departure_time){
        //If they depart on the same day they must have different flight numbers
        return fs1.flight_no < fs2.flight_no;
    }
    
    return fs1.departure_time < fs2.departure_time;
}

bool FlightManager::load_flight_data(std::string filename){
    //A lot of this code references AirportDB::load_airport_data()
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Can't open flight database: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)){
        
        std::istringstream ss(line);
        std::string airline, fn, source_airport, destination_airport, dep, arr, dur;
        
        if (!std::getline(ss, airline, ',') ||
            !std::getline(ss, fn, ',') ||
            !std::getline(ss, source_airport, ',') ||
            !std::getline(ss, destination_airport, ',') ||
            !std::getline(ss, dep, ',') ||
            !std::getline(ss, arr, ',') ||
            !std::getline(ss, dur, ','))
            continue;
        
        //convert and add all of our data to a new FlightSegment
        //NOTE: arrival time is never used (it can be inferred by departure_time + duration_sec
        int flight_number = std::stoi(fn);
        int departure_time = std::stoi(dep);
        int duration = std::stoi(dur);
        
        FlightSegment fs(airline, flight_number, source_airport, destination_airport, departure_time, duration);
        
        //insert our new (source airport, Flight Segment) pair into our table
        if (_departures.find(source_airport) == _departures.end()){
            //dynamically-allocated new BST set if it doesn't exist
            _departures[source_airport] = new BSTSet<FlightSegment>;
        }
        _departures[source_airport]->insert(fs);
    }
    
    return true;
}

std::vector<FlightSegment> FlightManager::find_flights(std::string source_airport, int start_time, int end_time) const{
    
    std::vector<FlightSegment> rtn;
    if (_departures.find(source_airport) == _departures.end() || end_time <= start_time) return rtn;
    
    BSTSet<FlightSegment>* deps = _departures.at(source_airport);
    
    //make a proxy flights with start/end times
    FlightSegment start("", 0, "", "", start_time, 0);
    FlightSegment end("", 9999, "", "", end_time - 1, 0); //subtract 1 because end_time is EXCLUSIVE
    
    auto it_start = deps->find_first_not_smaller(start);
    auto it_end = deps->find_first_not_smaller(end).get_and_advance();
    
    const FlightSegment* curr;
    
    while((curr = it_start.get_and_advance()) != it_end){
        rtn.push_back(*curr);
    }
    
    return rtn;
}
