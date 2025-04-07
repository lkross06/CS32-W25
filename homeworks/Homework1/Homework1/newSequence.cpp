#include <iostream>
#include "newSequence.h"

Sequence::Sequence(){
    m_size = 0;
    m_max_items = DEFAULT_MAX_ITEMS;
    m_array = new ItemType[DEFAULT_MAX_ITEMS];
}

Sequence::Sequence(int max_items){
    if (max_items < 0){
        std::cout << "Error: maximum items in sequence cannot be negative!" << std::endl;
        exit(1);
    }

    m_size = 0;
    m_max_items = max_items;
    m_array = new ItemType[m_max_items];
}

bool Sequence::empty() const{
    return m_size == 0;
}

int Sequence::size() const{
    return m_size;
}

int Sequence::insert(int pos, const ItemType& value){
    if (pos < 0 || pos > m_size || m_size == m_max_items) return -1;
    
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
    //swap sizes
    int temp_size = other.m_size;
    other.m_size = m_size;
    m_size = temp_size;
    
    //swap max items
    int temp_max_items = other.m_max_items;
    other.m_max_items = m_max_items;
    m_max_items = temp_max_items;
    
    //swap array pointers
    ItemType* temp_ap = other.m_array;
    other.m_array = m_array;
    m_array = temp_ap;
}

Sequence::~Sequence(){
    delete [] m_array;
}

Sequence::Sequence(const Sequence& old){
    m_max_items = old.m_max_items;
    m_array = new ItemType[m_max_items];
    m_size = old.m_size;
    for (int i = 0; i < m_size; i++){
        m_array[i] = old.m_array[i];
    }
}

Sequence &Sequence::operator=(const Sequence src){
    delete [] m_array;
    
    m_max_items = src.m_max_items;
    m_array = new ItemType[m_max_items];
    m_size = src.m_size;
    for (int i = 0; i < m_size; i++){
        m_array[i] = src.m_array[i];
    }
    
    return *this;
}
