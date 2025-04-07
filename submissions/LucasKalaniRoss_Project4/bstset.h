#ifndef BST_SET_H

#define BST_SET_H

template <typename T>
class BSTSet{
private:
    struct SetNode{ //initialize this first to use below
        SetNode(T v, SetNode* p = nullptr, SetNode* l = nullptr, SetNode* r = nullptr) : val(v), parent(p), left(l), right(r){}
        
        T val;
        SetNode* parent; //we need to point to the parent node so we can iterate up the BST
        SetNode* left;
        SetNode* right;
    };
    
    SetNode* _root;
    
    void PostOrderHelper(SetNode* curr){
        if (curr == nullptr) return;
        
        PostOrderHelper(curr->left);
        PostOrderHelper(curr->right);
        
        curr->left = nullptr;
        curr->right = nullptr;
        
        delete curr; //free the node after freeing its subtrees
        
    }
public:
    class SetIterator{
    public:
        SetIterator(SetNode* root){
            if (root == nullptr){
                _curr = nullptr;
                return;
            }
            
            //Find the smallest element
            _curr = root;
            while (_curr->left != nullptr) _curr = _curr->left;
        }
        
        ~SetIterator(){} //Everything gets freed by BSTSet
        
        //Returns a pointer to the current element that the iterator points at and advances the iterator to point to the next larger element in the set
        //must run in O(log N)
        const T* get_and_advance(){
            if (_curr == nullptr) return nullptr;
            
            T* rtn = &_curr->val;
            
            //If the current node has a right subtree, we go once to the right then all the way to the left
            if (_curr->right != nullptr){
                _curr = _curr->right;
                while (_curr->left != nullptr) _curr = _curr->left;
                return rtn;
            }
            //If the current node does not have a right subtree, we go up until the previously-current node is in a left subtree (it may never happen -> return nullptr)
            for (;;){
                SetNode* subtree = _curr;
                _curr = _curr->parent;
                
                if (_curr == nullptr) break; //reached end of iteration
                if (_curr->left == subtree) break;
            }
            
            return rtn;
        }
    private:
        SetNode* _curr;
    };
    
    //Initialize an empty set
    BSTSet() : _root(nullptr) {}
    
    //Release any internal resources
    ~BSTSet(){
        //Perform a post-order traversal to delete everything
        PostOrderHelper(_root);
        _root = nullptr; //prevent dangling pointer
    }
    
    //Insert value into the set if it is not already present
    //Runs in O(log N)
    void insert(const T& value){
        if (_root == nullptr){
            _root = new SetNode(value);
            return;
        }
        
        SetNode* parent_node = nullptr;
        SetNode* curr = _root;
        //traverse the BST to its corresponding spot
        while (curr != nullptr){
            parent_node = curr;
            if (value < curr->val) curr = curr->left;
            else if (curr->val < value) curr = curr->right; //MAKE SURE WE ONLY USE < SO WE ONLY HAVE TO OVERLOAD < LATER
            else {
                curr->val = value; //Replace the existig value with the new value
                return;
            }
        }
        
        //compare to the parent one last time
        SetNode* _ = new SetNode(value, parent_node);
        
        if (value < parent_node->val) parent_node->left = _;
        else parent_node->right = _;
        //no need to handle == case (see loop)
    }
    
    //Returns an iterator pointing to an element that is equal to value
    SetIterator find(const T& value) const{
        if (_root == nullptr) return SetIterator(nullptr); //value not found
        
        //lag one iterator behind the other so that the second one will point to the value once the first one has found and surpassed it
        SetIterator si1(_root);
        SetIterator si2(_root); //we will return this
        
        for (;;){
            const T* curr = si1.get_and_advance();
            
            if (curr == nullptr) return SetIterator(nullptr); //value not found
            
            if (*curr == value) break;
            else si2.get_and_advance();
        }
        
        return si2;
    }
    
    //Returns an iterator pointing to the smallest element in the set that is not smaller than value
    SetIterator find_first_not_smaller(const T& value) const{
        if (_root == nullptr) return SetIterator(nullptr); //value not found
        
        //lag one iterator behind the other so that the second one will point to the value once the first one has found and surpassed it
        SetIterator si1(_root);
        SetIterator si2(_root); //we will return this
        
        for (;;){
            const T* curr = si1.get_and_advance();
            
            if (curr == nullptr) return SetIterator(nullptr); //value not found
            
            if (!(*curr < value)) break; //THIS IS THE ONLY CHANGE LOL
            else si2.get_and_advance();
        }
        
        return si2;
    }
};


#endif //for BST_SET_H
