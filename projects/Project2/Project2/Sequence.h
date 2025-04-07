#ifndef SEQUENCE_H

#define SEQUENCE_H

#include <iostream>
#include <string>
using ItemType = unsigned long; //SUBMIT USING STD::STRING

//========================
// Sequence Definition
//========================

class Sequence
{
    public:
        Sequence();    // Create an empty sequence (i.e., one whose size() is 0).
        bool empty() const;  // Return true if the sequence is empty, otherwise false.
        int size() const;    // Return the number of items in the sequence.
        int insert(int pos, const ItemType& value);
          // Insert value into the sequence so that it becomes the item at
          // position pos.  The original item in position pos and those that
          // follow it end up at positions one greater than they were at before.
          // Return pos if 0 <= pos <= size() and the value could be
          // inserted.  (It might not be, if the sequence has a fixed capacity,
          // e.g., because it's implemented using a fixed-size array.)  Otherwise,
          // leave the sequence unchanged and return -1.  Notice that
          // if pos is equal to size(), the value is inserted at the end.

        int insert(const ItemType& value);
          // Let p be the smallest integer such that value <= the item at
          // position p in the sequence; if no such item exists (i.e.,
          // value > all items in the sequence), let p be size().  Insert
          // value into the sequence so that it becomes the item in position
          // p.  The original item in position p and those that follow it end
          // up at positions one greater than before.  Return p if the value
          // was actually inserted.  Return -1 if the value was not inserted
          // (perhaps because the sequence has a fixed capacity and is full).
         
        bool erase(int pos);
          // If 0 <= pos < size(), remove the item at position pos from
          // the sequence (so that all items that followed that item end up at
          // positions one lower than they were at before), and return true.
          // Otherwise, leave the sequence unchanged and return false.
         
        int remove(const ItemType& value);
          // Erase all items from the sequence that == value.  Return the
          // number of items removed (which will be 0 if no item == value).

        bool get(int pos, ItemType& value) const;
          // If 0 <= pos < size(), copy into value the item in position pos
          // of the sequence and return true.  Otherwise, leave value unchanged
          // and return false.

        bool set(int pos, const ItemType& value);
          // If 0 <= pos < size(), replace the item at position pos in the
          // sequence with value and return true.  Otherwise, leave the sequence
          // unchanged and return false.

        int find(const ItemType& value) const;
          // Let p be the smallest integer such that value == the item at
          // position p in the sequence; if no such item exists, let p be -1.
          // Return p.

        void swap(Sequence& other);
          // Exchange the contents of this sequence with the other one.
        void dump() const;
          // Prints out all values sequentially.
        ~Sequence();
          // Destructor
        Sequence(const Sequence &src);
          // Copy constructor
        Sequence& operator=(const Sequence& other);
          // Assignment operator
    private:
        class DoublyLinkedList{
            private:
                struct Node{
                    ItemType value;
                    Node* next;
                    Node* prev;
                };
                Node m_dummy; // Dummy for head node
                bool is_last_node(Node *n) const;
                      // Checks if node pointed at by n is the last node.
                void insert_after(Node *n, ItemType value);
                      // Inserts a node with value after n
                void del_after(Node *n);
                      // Deletes the node after n. Returns true if successful, false otherwise
            public:
                DoublyLinkedList();
                      // Constructor
                ~DoublyLinkedList();
                      // Destructor
                DoublyLinkedList(const DoublyLinkedList &src);
                      // Copy constructor
                DoublyLinkedList& operator=(const DoublyLinkedList& other);
                      // Assignment operator
                void add(ItemType value);
                      // Adds node with value to end of doubly-linked list
                bool insert(int pos, ItemType value);
                      // Adds a node with value to position pos. Returns true if successful, false otherwise
                bool del(int pos);
                      // Deletes the the node at position pos. Return true if successful, false otherwise
                bool set(int pos, ItemType value) const;
                      // Sets the value of the node at position pos to value. Return true if successful, false otherwise.
                ItemType get(int pos) const;
                      // Returns the value of the node at position pos.
                void dump() const;
                      // Prints out the values of the doubly-linked list in order
        };
        int m_size;
        DoublyLinkedList m_dll;
        
};

//========================
// Sequence Algorithms
//========================

int subsequence(const Sequence& seq1, const Sequence& seq2);
void zipper(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif //for SEQUENCE_H
