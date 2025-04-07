void removeOdd(vector<int>& v){
    auto it = v.begin(); //make an iterator
    while(it != v.end()){
        if ((*it) % 2 == 1) it = v.erase(it); //make sure to reset iterator so its not dangling
        else it++; //only increment if we did not erase a value
    }
}
