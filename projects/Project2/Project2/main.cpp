#include "Sequence.h"
#include <iostream>
#include <cassert>

int main()
{
    //test cases use ItemType = unsigned long
    unsigned long val; //this will be set and used temporarily by various calls of Sequence::get()
    
    Sequence s1; //default constructor
    assert(s1.empty() == true); //test empty()
    assert(s1.size() == 0); //test size() initially
    for (unsigned long i = 0; i <= 15; i+= 2){ //should be 0 2 4 ... 12 14
        s1.insert(i); //test insert()
        assert(s1.size() == (i/2) + 1); //test size() changing
        
        s1.get(s1.size() - 1, val); //test get()
        assert(val == i); //test that insert() puts increasing values at end of sequence
    }
    
    s1.insert(5); //now its 0 2 4 5 6 ... 12 14
    assert(s1.size() == 9); //test insert() into middle of sequence
    s1.get(3, val);
    assert(val == 5); //test insert() into middle of sequence
    s1.get(4, val);
    assert(val == 6); //test insert() into middle of sequence
    
    Sequence s2;
    for (unsigned long i = 1; i <= 19; i+= 2){ //should be 1 3 5 ... 13 15 17 19
        s2.insert(i);
    }
    
    s2.insert(2, 15); //now its 1 3 15 5 ... 13 15 17 19
    s2.get(2, val);
    assert(val == 15); //test insert() with position
    s2.get(3, val);
    assert(val == 5); //test insert() with position
    
    assert(s2.size() == 11);
    int nrem = s2.remove(15); //now its 1 3 5 ... 13 17 19
    assert(nrem == 2); //test remove()
    assert(s2.size() == 9); //test remove()
    s2.get(2, val);
    assert(val == 5); //test remove()
    
    s2.erase(s2.size() - 2); //now its 1 3 5 ... 13 19
    assert(s2.size() == 8); //test erase()
    s2.get(7, val);
    assert(val == 19); //test erase()
    
    s2.set(7, 15); //now its 1 3 5 ... 13 15
    s2.get(7, val);
    assert(val == 15); //test set()
    
    Sequence *s3 = new Sequence;
    assert(s3->get(2, val) == false); //test get() handling empty dll
    assert(s3->erase(3) == false); //test erase() handling empty dll
    assert(s3->remove(45) == false); //test remove() handling empty dll
    assert(s3->set(3, 45) == false); //test set() handling empty dll
    
    delete s3; //test destructor
    
    s1.swap(s2); //now s2 = 0 2 4 5 6 ... 12 14, s1 = 1 3 5 ... 13 15
    
    assert(s1.size() == 8 && s2.size() == 9); //test swap()
    assert(s2.remove(4) == 1); //test swap()
    s2.set(2, 4);
    
    //at this point s1 = 1 3 5 7 9 11 13 15, s2 = 0 2 4 6 8 10 12 14
    Sequence s4;
    s4.erase(0); //test erase() on empty sequence
    s4.insert(66); //this should get removed by zipper
    zipper(s2, s1, s4); //now s4 = 0 1 2 3 4 5 ... 11 12 13 14 15
    for (int i = 0; i <= 15; i++){ //0 1 2 3 4 5 ... 11 12 13 14 15
        s4.get(i, val);
        assert(val == i); //test zipper()
    }
    
    Sequence s5;
    s5.insert(11);
    s5.insert(12);
    s5.insert(13); //now s5 = 11 12 13
    assert(subsequence(s4, s5) == 11); //test subsequence()
    
    s5.set(1, 444); //now s5 = 11 444 12 13
    assert(subsequence(s4, s5) == -1); //test subsequence() handling no subsequence
    
    while (!s5.empty()) s5.erase(0); //clear s5
    s5.insert(15);
    assert(subsequence(s4, s5) == 15); //test subsequence() where s2's only element = s1's last element
    s5.erase(0);
    assert(subsequence(s4, s5) == -1); //test subsequence() handling empty seq2
    
    
    std::cerr << "Passed all tests." << std::endl;
}
