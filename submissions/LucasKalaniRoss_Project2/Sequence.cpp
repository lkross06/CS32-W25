#include "Sequence.h"
#include <cassert>
#include <iostream>

//========================
// Sequence Implementation
//========================

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
    if (pos < 0 || pos > m_size) return -1;
    
    m_dll.insert(pos, value);
    m_size++;
    
    return pos;
}

int Sequence::insert(const ItemType& value){
    int pos = 0;
    while (pos < m_size){
        if (value <= m_dll.get(pos)){
            m_dll.insert(pos, value);
            m_size++;
            return pos;
        }
        pos++;
    }
    m_dll.add(value);
    m_size++;
    return pos;
}

bool Sequence::erase(int pos){
    if (pos < 0 || pos > m_size) return false;
    
    if (m_dll.del(pos)){
        m_size--;
        return true;
    }
    return false;
}

int Sequence::remove(const ItemType& value){
    int removed = 0;
    int p = 0;
    
    while (p < m_size){
        if (m_dll.get(p) == value){
            m_dll.del(p);
            m_size--;
            removed++;
        }
        else p++;
    }
    
    return removed;
}

bool Sequence::get(int pos, ItemType& value) const{
    if (pos < 0 || pos >= m_size) return false;
    
    value = m_dll.get(pos);
    return true;
}

bool Sequence::set(int pos, const ItemType& value){
    if (pos < 0 || pos >= m_size) return false;
    
    return m_dll.set(pos, value);
}

int Sequence::find(const ItemType& value) const{
    for (int p = 0; p < m_size; p++){
        if (m_dll.get(p) == value) return p;
    }
    return -1;
}

void Sequence::swap(Sequence& other){
    //swap sizes
    int temp_size = other.m_size;
    other.m_size = m_size;
    m_size = temp_size;
    
    DoublyLinkedList temp_dll = other.m_dll; //copy constructor
    other.m_dll = m_dll; //assignment operator
    m_dll = temp_dll; //assignment operator
    //temp_dll destructor called
}

void Sequence::dump() const{
    //write to cerr
    for (int pos = 0; pos < m_size; pos++) std::cerr << m_dll.get(pos) << " ";
    std::cerr << std::endl;
}

Sequence::~Sequence(){
}

Sequence::Sequence(const Sequence &src){
    m_dll = src.m_dll; //copy constructor
    m_size = src.m_size;
}

Sequence& Sequence::operator=(const Sequence& other){
    m_dll = other.m_dll; //assignment operator
    m_size = other.m_size;
    
    return *this;
}

//========================
// Sequence Algorithms
//========================

bool Sequence::DoublyLinkedList::is_last_node(Node *n) const{
    return ( n == nullptr || n->next == nullptr);
}

void Sequence::DoublyLinkedList::insert_after(Node *n, ItemType value){
    //initialize new node
    Node *nn;
    nn = new Node;
    
    nn->value = value;
    nn->prev = n;
    nn->next = n->next;
    
    //edit other nodes
    if (!is_last_node(n)) n->next->prev = nn; // we might be at the end of the dll
    n->next = nn;
}

void Sequence::DoublyLinkedList::del_after(Node *n){
    Node *ntd = n->next; //node to delete
    
    //edit other nodes
    if (is_last_node(ntd)){
        n->next = nullptr;
    } else {
        n->next = ntd->next;
        ntd->next->prev = n;
    }
    
    delete ntd;
}

Sequence::DoublyLinkedList::DoublyLinkedList(){
    m_dummy.next = nullptr;
}

Sequence::DoublyLinkedList::~DoublyLinkedList(){
    Node *curr = m_dummy.next;
    
    while(!is_last_node(curr)){
        curr = curr->next;
        delete curr->prev;
    }
    
    delete curr;
}

Sequence::DoublyLinkedList::DoublyLinkedList(const DoublyLinkedList &src){
    m_dummy.next = nullptr;
    
    Node *src_curr = src.m_dummy.next;
    if (src_curr == nullptr) return;
    
    insert_after(&m_dummy, src_curr->value);
    
    //traverse the source dll and insert each node into this dll
    while(!is_last_node(src_curr)){
        src_curr = src_curr->next;
        add(src_curr->value);
    }
}

Sequence::DoublyLinkedList& Sequence::DoublyLinkedList::operator=(const DoublyLinkedList& other){
    Node *curr = m_dummy.next;
    m_dummy.next = nullptr;
    
    while(!is_last_node(curr)){
        curr = curr->next;
        delete curr->prev;
    }
    
    delete curr;
    
    //repopulate this dll with the other dll values
    Node *other_curr = other.m_dummy.next;
    if (other_curr == nullptr) return *this;
    
    insert_after(&m_dummy, other_curr->value);
    
    //traverse the source dll and insert each node into this dll
    while(!is_last_node(other_curr)){
        other_curr = other_curr->next;
        add(other_curr->value);
    }
    
    return *this;
}

void Sequence::DoublyLinkedList::add(ItemType value){
    //otherwise go to the last node and add a new one
    Node *curr = &m_dummy;
    
    while (!is_last_node(curr)){
        curr = curr->next;
    }
    
    insert_after(curr, value);
}
  // Adds node with value to end of doubly-linked list
bool Sequence::DoublyLinkedList::insert(int pos, ItemType value){
    
    Node *curr = &m_dummy;
    
    while(!is_last_node(curr) && pos > 0){
        curr = curr->next;
        pos--;
    }
    
    insert_after(curr, value);
    return true;
}
  // Adds a node with value to position pos. Returns true if successful, false otherwise
bool Sequence::DoublyLinkedList::del(int pos){
    if (m_dummy.next == nullptr) return false;
    Node *curr = m_dummy.next;
    
    while(!is_last_node(curr) && pos > 0){
        curr = curr->next;
        pos--;
    }
    
    del_after(curr->prev);
    return true;
}
  // Deletes the the node at position pos. Return true if successful, false otherwise
bool Sequence::DoublyLinkedList::set(int pos, ItemType value) const{
    Node *curr = m_dummy.next;
    
    while(!is_last_node(curr) && pos > 0){
        curr = curr->next;
        pos--;
    }

    curr->value = value;
    return true;
}
  // Sets the value of the node at position pos to value. Return true if successful, false otherwise.
ItemType Sequence::DoublyLinkedList::get(int pos) const{
    Node *curr = m_dummy.next;
    
    while(!is_last_node(curr) && pos != 0){
        curr = curr->next;
        pos--;
    }

    return curr->value;
}
  // Returns the value of the node at position pos.
void Sequence::DoublyLinkedList::dump() const{
    Node *curr = m_dummy.next;
    
    //find the last node in the dll
    while (!is_last_node(curr)){
        std::cerr << curr->value << " ";
        curr = curr->next;
    }
    
    std::cerr << curr->value << std::endl;
}
  // Prints out the values of the doubly-linked list in order

//========================
// Sequence Algorithms
//========================

int subsequence(const Sequence& seq1, const Sequence& seq2){
    if (seq2.empty()) return -1;
    for (int k = 0; k < seq1.size(); k++){
        ItemType v1;
        ItemType v2;
        
        int n = 0;
        //match every k + nth item in seq1 to every nth item in seq2
        while (n < seq2.size() && n + k < seq1.size()){
            seq1.get(n+k, v1);
            seq2.get(n, v2);
            
            if (v1 == v2) n++;
            else break;
        }
        //did the while loop go through seq2 without breaking?
        if (n == seq2.size()) return k;
    }
    return -1;
}

void zipper(const Sequence& seq1, const Sequence& seq2, Sequence& result){
    //first we clear result
    while(!result.empty()) result.erase(0);
    
    //now we iterate through both seq1 and seq2, only "zippering" if the sequences have nodes left
    for (int i = 0; i < seq1.size() || i < seq2.size(); i++){
        if (i < seq1.size()){
            ItemType v1;
            seq1.get(i, v1);
            
            result.insert(result.size(), v1);
        }
        if (i < seq2.size()){
            ItemType v2;
            seq2.get(i, v2);
            
            result.insert(result.size(), v2);
        }
    }
}
