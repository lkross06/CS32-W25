void removeBad(list<Movie*>& li){
    list<Movie*>::iterator it = li.begin(); //make an iterator
        while(it != li.end()){
            if ((*it)->rating() < 50){
                delete (*it); //delete dynamically allocated movies
                it = li.erase(it); //erase dangling pointer
            }
            else it++; //only increment if we did not erase a value
        }
}
