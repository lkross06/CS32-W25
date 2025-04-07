void removeBad(vector<Movie*>& v){
    vector<Movie*>::iterator it = v.begin(); //make an iterator
    while(it != v.end()){
        if ((*it)->rating() < 50){
            delete (*it); //delete dynamically allocated movies
            it = v.erase(it); //erase dangling pointer
        }
        else it++; //only increment if we did not erase a value
    }
}
