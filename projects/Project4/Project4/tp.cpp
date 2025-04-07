#include "tp.h"
#include <queue>
#include <stack>
#include <vector>

bool TravelPlanner::is_preferred_airline(std::string airline) const{
    if (_pref_airlines.empty()) return true;
    return (_pref_airlines.find(airline) != _pref_airlines.end());
}

void TravelPlanner::add_preferred_airline(std::string airline){ _pref_airlines.insert(airline);
}

bool TravelPlanner::plan_travel(std::string source_airport, std::string destination_airport, int start_time, Itinerary& itinerary) const {
    //the queue of flight segment pointers will perform a BFS on all the different path options
    std::queue<FlightSegment*> q;
    
    //keep track of the complete itinerary with the shortest travel duration
    FlightSegment* final_path = nullptr;
    int final_duration = 0;
    
    //parental dictionary (maps a pointer to a flight segment to a pointer to the previous flight segment-- i.e. how it got there)
    std::unordered_map<FlightSegment*, FlightSegment*> connections;
    
    //visited airports and the earliest UNIX time it was visited. helps prune
    std::unordered_map<std::string, int> earliest_visit;
    
    //add a proxy entry so we can simulate our first flight
    FlightSegment* _ = new FlightSegment("", 0, "", source_airport, start_time, 0);
    
    earliest_visit[source_airport] = start_time;
    connections.insert({_, nullptr});
    connections.insert({nullptr, nullptr}); //for when we get to the proxy flight
    q.push(_);
    
    while (!q.empty()){
        FlightSegment* current = q.front();
        q.pop();
        
        int current_duration = 0; //the time its taken to get to this airport
        FlightSegment* temp = current;
    
        //while we havent reached our proxy flight segment
        while (temp != nullptr){
            //1. add the last flight duration
            current_duration += temp->duration_sec;
            
            //2. add the previous layover time
            FlightSegment* previous = connections[temp];
            if (previous != nullptr){ //everything except the first flight out
                current_duration += temp->departure_time - (previous->departure_time + previous->duration_sec);
            }
            
            temp = previous;
        }
        
        if (final_path != nullptr && current_duration <= final_duration) continue; //if this new journey is already longer than the fastest found journey, why even look at flights
        
        int earliest_dep_time = start_time + current_duration;
        int latest_dep_time = start_time + current_duration + get_max_layover();
        std::string current_airport = current->destination_airport;
        
        //if we're not at the starting airport we have to wait to get on a flight
        if (current_airport != source_airport) earliest_dep_time += get_min_connection_time();
       
        //search for all outgoing flights within the minimum connection - maximum layover time range
        std::vector<FlightSegment> outgoing_flights = get_flight_manager().find_flights(current_airport, earliest_dep_time, latest_dep_time);
        
        for (FlightSegment flight : outgoing_flights){
            
            int new_duration = current_duration;
            new_duration += flight.duration_sec; //add the new flight duration
            new_duration += flight.departure_time - (current->departure_time + current->duration_sec); //add the new layover time if its a connecting flight
            
            //if we've already seen the new airport earlier, skip this entry
            if (earliest_visit.find(flight.destination_airport) != earliest_visit.end() && earliest_visit[flight.destination_airport] <= new_duration) continue;
            
            //update the new earliest time to visit this airport
            earliest_visit[flight.destination_airport] = new_duration;
            
            //check if the connection is on a preferred airline
            if (is_preferred_airline(flight.airline)){
                //dynamicaly allocate a copy of the aiport segment (because the vector will reset at the end of each iteration)
                FlightSegment* new_connection = new FlightSegment(flight.airline, flight.flight_no, flight.source_airport, flight.destination_airport, flight.departure_time, flight.duration_sec);
                
                //if we're at our destination and we got there faster, replace the final path
                if (flight.destination_airport == destination_airport){
                    if (final_path == nullptr || new_duration < final_duration){
                        connections.insert({new_connection, current}); //insert into our parent dictionary map
                        final_path = new_connection;
                        final_duration = new_duration;
                    }
                //otherwise, if we're under the maximum duration and we're closer to our destination, add to the queue so we can look for a connecting flight
                } else if (new_duration < get_max_duration()) {
                    connections.insert({new_connection, current}); //insert into our parent dictionary map
                    q.push(new_connection);
                //nevermind we don't need this connection
                } else {
                    delete new_connection;
                }
            }
        }
    }
    
    if (final_path == nullptr) return false;
    
    //use a stack to reverse the connections
    std::stack<FlightSegment> s;

    //while we havent reached our proxy flight segment
    while (final_path != nullptr){
        s.push(*final_path);
        FlightSegment* previous = connections[final_path];
        final_path = previous;
    }
    
    //Now put all of the flight segments into the Itinerary reference
    std::vector<FlightSegment> flight_path;
    
    //the top element is the proxy node
    s.pop();
    
    while (!s.empty()){
        flight_path.push_back(s.top());
        s.pop();
    }
    
    itinerary.source_airport = source_airport;
    itinerary.destination_airport = destination_airport;
    itinerary.flights = flight_path;
    itinerary.total_duration = final_duration;
    
    //free all of the dynamically allocated Flight Segments
    for (auto it = connections.begin(); it != connections.end(); it++){
        delete it->first; //since theres only one occurence of each FlightSegment in the first slot (whereas a FlightSegment may appear multiple times in second slots) we just delete every first slot
    }
    connections.clear();
    
    return true;
}
