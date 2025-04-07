#include "Sequence.h"

Sequence::Sequence(){
    m_size = 0;
}

bool Sequence::empty() const{
    return m_size == 0;
}

int Sequence::size() const{
    return m_size;
}

int Sequence::insert(int pos, const ItemType& value){
    if (pos < 0 || pos > m_size || m_size == DEFAULT_MAX_ITEMS) return -1;
    
    int i = pos + 1;
    ItemType prev = m_array[pos];
    ItemType curr;
    
    m_array[pos] = value;
    
    //shift everything after pos forward by one
    while (i <= m_size){
        curr = m_array[i];
        m_array[i] = prev;
        prev = curr;
        i++;
    }
    
    m_size++;

    return pos;
}

int Sequence::insert(const ItemType& value){
    int p = 0;
    
    //find smallest position p such that value <= item at position p
    while (value <= m_array[p] && p != m_size) p += 1;
    
    return insert(p, value);
}

bool Sequence::erase(int pos){
    if (pos < 0 || pos >= m_size) return false;
    
    //move every item at [pos + 1] to [pos]
    while (pos + 1 < m_size){
        m_array[pos] = m_array[pos + 1];
        pos++;
    }
    
    m_size--;
        
    return true;
}

int Sequence::remove(const ItemType& value){
    int erased = 0;
    int i = 0;
    
    //note: m_size may change between iterations
    while (i < m_size){
        if (value == m_array[i]){
            erase(i);
            erased++;
        //only increment if array contents dont shift down by one index
        } else i++;
    }
    
    return erased;
}

bool Sequence::get(int pos, ItemType& value) const{
    if (pos < 0 || pos >= m_size) return false;
    
    value = m_array[pos];
    
    return true;
}

bool Sequence::set(int pos, const ItemType& value){
    if (pos < 0 || pos >= m_size) return false;
    
    m_array[pos] = value;
    
    return true;
}

int Sequence::find(const ItemType& value) const{
    //start at beginning of array for smallest integer p
    for(int p = 0; p < m_size; p++){
        if (value == m_array[p]) return p;
    }
    
    //if the loop breaks then it reached the end of the array
    return -1;
}

void Sequence::swap(Sequence& other){
    //iterate through arrays and swap values one by one
    for (int i = 0; i < m_size || i < other.m_size; i++){
        ItemType temp;
        if (i < other.m_size) temp = other.m_array[i];
        else temp = m_array[i];
        //we KNOW that i < m_size so we know that m_array[i] is defined. this is
        //just so there's no warning on line 112 regarding temp being uninitialized
        
        if (i < m_size) other.m_array[i] = m_array[i];
        if (i < other.m_size) m_array[i] = temp;
    }
    
    //swap sizes
    int temp_size = other.m_size;
    other.m_size = m_size;
    m_size = temp_size;
}
