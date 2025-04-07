#include "Sequence.h"
#include <cassert>
#include <iostream>

int main()
{
    Sequence s;
    assert(s.empty());
    ItemType x = 97;
    assert( !s.get(68, x)  &&  x == 97); // x unchanged by get failure
    assert(s.find(42) == -1);
    assert(s.insert(42) == 0);
    assert(s.size() == 1  &&  s.find(42) == 0);
    assert(s.get(0, x)  &&  x == 42);
    
    std::cout << "Passed all tests" << std::endl;
}
