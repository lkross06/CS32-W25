void removeOdd(list<int>& li)
{
    auto it = li.begin(); //make an iterator
    while(it != li.end()){
        if ((*it) % 2 == 1) it = li.erase(it); //make sure to reset iterator so its not dangling
        else it++; //only increment if we did not erase a value
    }
}
