#include "TurnoutList.h"

TurnoutList::TurnoutList(){
    
}

bool TurnoutList::add(unsigned long turnout){
    //don't need to check turnout < 0 because unsigned means > 0
    if (turnout > MAXIMUM_TURNOUT) return false;
    
    return m_sequence.insert(turnout) != -1;
}
 

bool TurnoutList::remove(unsigned long turnout){
    //cant use m_sequence.remove(), since it removes all instances
    int pos = m_sequence.find(turnout);
    
    if (pos != -1) return m_sequence.erase(pos);
    return false;
}

int TurnoutList::size() const{
    return m_sequence.size();
}

unsigned long TurnoutList::minimum() const{
    if (m_sequence.size() == 0) return NO_TURNOUTS;
    
    unsigned long min;
    m_sequence.get(0, min);
    
    for (int i = 1; i < m_sequence.size(); i++){
        unsigned long curr;
        m_sequence.get(i, curr);
        
        if (curr < min) min = curr;
    }
    
    return min;
}

unsigned long TurnoutList::maximum() const{
    if (m_sequence.size() == 0) return NO_TURNOUTS;
    
    unsigned long max;
    m_sequence.get(0, max);
    
    for (int i = 1; i < m_sequence.size(); i++){
        unsigned long curr;
        m_sequence.get(i, curr);
        
        if (curr > max) max = curr;
    }
    
    return max;
}
