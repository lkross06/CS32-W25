#include "TurnoutList.h"
#include <cassert>
#include <iostream>
using namespace std;

int main(){
    TurnoutList tl;
    
    assert(tl.minimum() == NO_TURNOUTS && tl.maximum() == NO_TURNOUTS);
    
    assert(tl.add(944));
    assert(tl.add(1231));
    assert(tl.add(1566));
    assert(!tl.add(1990));
    assert(!tl.add(-231));
    assert(tl.add(197));
    assert(tl.add(1700));
    
    assert(tl.remove(1231));
    assert(!tl.remove(1550));
    
    assert(tl.maximum() == 1700);
    assert(tl.size() == 4);
    tl.remove(1700);
    assert(tl.maximum() == 1566);
    
    assert(tl.minimum() == 197);
    assert(tl.size() == 3);
    
    TurnoutList tl2 = tl; //test copy constructor
    
    assert(tl2.remove(944));
    
    tl = tl2; //test assignment operator
    
    assert(!tl.remove(944));
    
    cout << "All tests passed" << endl;
}
