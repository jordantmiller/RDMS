/*********************************************************
 *   AUTHOR        : Jordan Miller
 * 
 *   PROJECT       : Relational Database
 * 
 *   PURPOSE       : Relatinal database management system
 *                   using B+ Tree indexing with SQL command
 *                   interface
 * 
 *   Copyright (c) 2019, Jordan Miller
 ********************************************************
FILE: BPTree.h
TEMPLATE CLASS PROVIDED: BPTree<Item> (Balanced+ Tree)

CONSTRUCTORS:
    BPTree(bool dups = false)
        Postcondition: An empty BPTree has been created.

    BPTree(const BPTree<Item>& other)
        Precondition: other is a valid BPTree with the same template type.
        Postcondition: The calling BPTree has been created containing the 
                       same information as other.

ASSIGNMENT:
    BPTree<Item>& operator =(const BPTree<Item>& rhs)
        Precondition: rhs is a valid BPTree with the template Item.
        Postcondition: The calling BPTree now contains the same
                       data as rhs.

MUTATORS:
    void insert(const Item& entry)
        Precondition: entry is of type Item.
        Postcondition: The calling BPTree now contains entry
                       and remains a valid BPTree.  If an Item
                       which was equal to entry already existed
                       in the BPTree, then it has been replaced.

    bool remove(const Item& entry)
        Postcondition: If entry existed in the BPTree,
                       it has been removed and the BPTree
                       is still valid.
    
    void clear_tree()
        Postcondition: The calling BPTree now contains no data.
                       The memory which may have been allocated for
                       the children of the BPTree has been realsed to
                       the heap.

    void copy_tree(const BPTree<Item>& other)
        Precondition: other is a valid BPTree
        Postcondition: The calling tree now contains the same data
                       as other.

// ACCESSORS

    bool contains(const Item& target)
        Postcondition: a bool has been returned indicating the existance
                       of target in the BPTree.

    Item& get(const Item& target)
        Postcondition: Returns a reference to the item equal to target.

    Item* find(const Item& target)
        Postcondition: Returns a pointer to the item equal to target.

    int size() const
        Postcondition: Returns the number of items in the BPTree.

    bool empty() const
        Postcondition: Returns a bool indicating if the tree contains no Items
        
    void print_tree(int level = 0, std::ostream &outs = std::cout) const
        Postcondition: The tree has been printed in a format which indicates
                       the parent child relationships of the BPTree. 

*/
#ifndef BPTREE_H
#define BPTREE_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "./array_functions.h"

template <typename Item>
class BPTree
{
public:
    class Iterator
    {
    public:
        friend class BPTree;
        Iterator(BPTree<Item>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}

        Item operator *()
        {
            // std::cout << "Key_ptr: " << key_ptr << " data_count: " << it->data_count << std::endl;
            assert(key_ptr < it->data_count);
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used)
        {
            Iterator temp;
            temp = *this;

            ++(*this);
            return temp;
        }

        Iterator operator++()
        {
            if (key_ptr < it->data_count - 1)
                ++key_ptr;
            else
            {
                it = it->next;
                key_ptr = 0;
            }
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return (lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr);
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return (lhs.it != rhs.it || lhs.key_ptr != rhs.key_ptr);
        }
        void print_Iterator(){
        }
        bool is_null(){return !it;}
    private:
        BPTree<Item>* it;
        int key_ptr;
    };


    // CONSTRUCTORS
    BPTree(bool dups = false);
    BPTree(const BPTree<Item>& other);

    // DESTRUCTOR
    ~BPTree();

    // ASSIGNEMENT
    BPTree<Item>& operator =(const BPTree<Item>& rhs);

    // MUTATORS
    void insert(const Item& entry);              
    bool remove(const Item& entry);             
    void clear_tree();                        
    void copy_tree(const BPTree<Item>& other, BPTree<Item>*& last_leaf = NULL);     

    // ACCESSORS
    bool contains(const Item& target) const;             
    Item& get(const Item& target);                     
    Item* find(const Item& target);                
    int size() const;                       
    bool empty() const { return data_count == 0; }                        
    void print_tree(int level = 0, std::ostream &outs = std::cout) const;
    bool is_valid(); 
    friend std::ostream& operator<<(std::ostream& outs, const BPTree<Item>& print_me)
    {
        print_me.print_tree(0, outs);
        return outs;
    }

    // ITERATOR FUNCTIONS
    typename BPTree<Item>::Iterator begin();
    typename BPTree<Item>::Iterator at(Item target);
    typename BPTree<Item>::Iterator end();
    typename BPTree<Item>::Iterator upper_bound(const Item& key);
    typename BPTree<Item>::Iterator lower_bound(const Item& key);


// private:

    // STATIC MEMBER VARIABLES
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    // PRIVATE MEMBER VARIABLES
    bool dups_ok;                                   
    int data_count;                               
    Item data[MAXIMUM + 1];                         
    int child_count;                               
    BPTree<Item>* subset[MAXIMUM + 2];
    BPTree<Item>* next;                    

    // PRIVATE MEMBER FUNCTIONS
    BPTree<Item>* get_smallest_node();
    bool is_leaf() const { return child_count == 0; }   
    void loose_insert(const Item& entry);             
    void fix_excess(int i);                        
    bool loose_remove(const Item& entry);
    bool remove_sister(const Item& entry);             
    void fix_shortage(int i);                      
    void remove_biggest(Item& removed_entry);                
    void rotate_left(int index);                       
    void rotate_right(int index);                      
    void merge_with_next_subset(int index);             

};

// CONSTRUCTORS
template<typename Item>
BPTree<Item>::BPTree(bool dupes)
{
    data_count = 0;
    child_count = 0;
    next = NULL;
    dups_ok = dupes;
}

template<typename Item>
BPTree<Item>::BPTree(const BPTree<Item>& other)
{
    this->dups_ok = other.dups_ok;
    data_count = 0;
    child_count = 0;
    BPTree<Item>* ptr = NULL;
    copy_tree(other, ptr);
}

// DESTRUCTOR
template<typename Item>
BPTree<Item>::~BPTree()
{
    this->clear_tree();
}

// ASSIGNEMENT
template<typename Item>
BPTree<Item>& BPTree<Item>::operator=(const BPTree<Item>& rhs)
{
    this->clear_tree();
    BPTree<Item>* ptr = NULL;
    this->copy_tree(rhs, ptr);
    return *this;
}

// MUTATORS
template<typename Item>
void BPTree<Item>::insert(const Item& entry)
{
    loose_insert(entry);

    if (data_count > MAXIMUM)
    {
        BPTree<Item>* new_child;
        new_child = new BPTree<Item>(dups_ok);

        // Copy data into new child
        copy_array(new_child->data, data, new_child->data_count, data_count);
        // Copy subset into new child
        copy_array(new_child->subset, subset, new_child->child_count, child_count);

        data_count = 0;
        child_count = 1;

        subset[0] = new_child;

        fix_excess(0);
    }
}

template<typename Item>
bool BPTree<Item>::remove(const Item& entry)
{
    if (!loose_remove(entry))
        return false;
    
    if (data_count == 0 && child_count == 1)
    {
        BPTree<Item>* temp;
        temp = this->subset[0];

        copy_array(this->data,
                   temp->data,
                   this->data_count,
                   temp->data_count);

        copy_array(this->subset,
                   temp->subset,
                   this->child_count,
                   temp->child_count);

        for (int i = 0; i < temp->child_count; i++)
            temp->subset[i] = NULL;

        temp->child_count = temp->data_count = 0;

        delete temp;        
    }
    return true;
}

template<typename Item>
void BPTree<Item>::clear_tree()
{
    for (int i = 0; i < child_count; i++)
    {
        subset[i]->clear_tree();
        delete subset[i];
    }

    // Clears the data for the root, redundantly applied to
    // the dynamic nodes which are about to be deleted.
    for (int i = 0; i < MAXIMUM + 2; i++)
        subset[i] = NULL;
    
    data_count = child_count = 0;
}


template<typename Item>
void BPTree<Item>::copy_tree(const BPTree<Item>& other, BPTree<Item>*& last_leaf)
{
    this->clear_tree();

    copy_array(this->data, other.data, this->data_count, other.data_count);
    this->child_count = other.child_count;

    if (!other.is_leaf())
    {
        for (int i = 0; i < other.child_count; i++)
        {
            subset[i] = new BPTree<Item>(dups_ok);            
            subset[i]->copy_tree(*other.subset[i], last_leaf);
        }
    }
    else
    {
        if (last_leaf != NULL)
        {
            last_leaf->next = this;
            last_leaf = last_leaf->next;
        }
        else
            last_leaf = this;
    }
}

// ACCESSORS

template<typename Item>
bool BPTree<Item>::contains(const Item& target) const
{
    int index;
    bool found;
    index = first_ge(data, data_count, target);
    found = (index < data_count && data[index] == target);
    if (found && this->is_leaf())
        return true;
    else if (found && !this->is_leaf())
        return subset[index + 1]->contains(target);
    else if (!found && !this->is_leaf())
        return subset[index]->contains(target);
    else
        return false;
}

template<typename Item>
Item* BPTree<Item>::find(const Item& target)
{
    assert(this->contains(target));

    Item* ptr;

    ptr = &this->get(target);
    return ptr;
}

template<typename Item>
int BPTree<Item>::size() const
{
    int size;
    size = data_count;

    for (int i = 0; i < child_count; i++)
        size += subset[i]->size();
    
    return size;
}

template<typename Item>
Item& BPTree<Item>::get(const Item& target)
{
    int index;
    bool found;
    index = first_ge(data, data_count, target);
    found = (data[index] == target && index < data_count);

    if (found && this->is_leaf())
        return data[index];
    else if (found && !this->is_leaf())
        return subset[index + 1]->get(target);
    else if (!found && !this->is_leaf())
        return subset[index]->get(target);
    else
        std::cout << "Item is not in tree." << std::endl;
}

template<typename Item>
void BPTree<Item>::print_tree(int level, std::ostream& outs) const
{
    if (!is_leaf())
    {
        for (int i = 1; i <= data_count; i++)
        {
            subset[child_count - i]->print_tree(level + 1, outs);
            outs << std::setw(level * 4) << "" << data[data_count - i] << std::endl;
        }
            subset[0]->print_tree(level + 1, outs);
    }
    else
    {
        for (int i = data_count - 1; i >= 0; i--)
            outs << std::setw(level * 4) << "" << data[i] << std::endl;
    }
}

// PRIVATE HELPER FUNCTIONS

template<typename Item>
bool BPTree<Item>::loose_remove(const Item& target)
{
    Item hold;
    int index;
    bool found;
    index = first_ge(data, data_count, target);

    if (child_count == 0 && data[index] != target || 
        child_count == 0 && index >= data_count)
    {
        return false;
    }
    else if (child_count == 0 && data[index] == target)
    {
        delete_item(data, index, data_count, hold);
        return true;
    }
    else if(child_count > 0 && data[index] != target)
    {
        found = subset[index]->loose_remove(target);
        
        if (subset[index]->data_count < MINIMUM)
            fix_shortage(index);
        
        return found;
    }
    else if(child_count > 0 && data[index] == target)
    {
        subset[index + 1]->loose_remove(target);
        if (subset[index + 1]->data_count < MINIMUM)
            fix_shortage(index + 1);
        
        return true;
    }
    else
        std::cout << "Something went wrong in loose_remove()" << std::endl;   
}

template<typename Item>
void BPTree<Item>::remove_biggest(Item& removed_entry)
{
    if (child_count == 0)
    {
        removed_entry = data[data_count - 1];
        data_count--;
    }
    else
    {
        subset[child_count - 1]->remove_biggest(removed_entry);
        if (subset[child_count - 1]->data_count < MINIMUM)
            fix_shortage(child_count - 1);
    }
}

template<typename Item>
void BPTree<Item>::fix_shortage(int i)
{
    Item hold;
    BPTree* tree_hold;

    // Case 1: Transfer extra item from subset[i - 1]
    if(i > 0 && subset[i - 1]->data_count > MINIMUM)
    {
        if (subset[i]->is_leaf())
        {
            insert_item(subset[i]->data, 0, 
                        subset[i]->data_count, 
                        subset[i - 1]->data[subset[i - 1]->data_count - 1]);

            data[i - 1] = subset[i]->data[0]; 

            subset[i - 1]->data_count--;
        }
        else
        {
            insert_item(subset[i]->data, 0, 
                        subset[i]->data_count, 
                        data[i - 1]);

            // This convoluted line sets data[i - 1] to the last data item in
            // subset[i - 1]
            data[i - 1] = subset[i - 1]->data[subset[i - 1]->data_count - 1];

            // Effectively removes last item from subset[i - 1]->data
            subset[i - 1]->data_count--;

            if (subset[i - 1]->child_count > 0)
            {
                insert_item(subset[i]->subset, 0, 
                            subset[i]->child_count,
                            subset[i - 1]->subset[subset[i - 1]->child_count - 1]);

                subset[i - 1]->child_count--;
            }
        }
    }
    // Case 2: Transfer extra Item from subset [i + 1]
    else if (i + 1 < child_count && subset[i + 1]->data_count > MINIMUM)
    {
        if (subset[i]->is_leaf())
        {
            insert_item(subset[i]->data, 
                        subset[i]->data_count,
                        subset[i]->data_count,
                        data[i]);

            delete_item(subset[i + 1]->data,
                        0, subset[i + 1]->data_count,
                        hold);

            data[i] = subset[i + 1]->data[0];
        }
        else
        {
            insert_item(subset[i]->data, 
                        subset[i]->data_count,
                        subset[i]->data_count,
                        data[i]);

            data[i] = subset[i + 1]->data[0];

            delete_item(subset[i + 1]->data,
                        0, subset[i + 1]->data_count,
                        hold);


            if (subset[i + 1]->child_count > 0)
            {
                insert_item(subset[i]->subset,
                            subset[i]->child_count,
                            subset[i]->child_count,
                            subset[i + 1]->subset[0]);

                delete_item(subset[i + 1]->subset, 0,
                            subset[i + 1]->child_count,
                            tree_hold);
            }
        }
    }
    // Case 3: Combine subset[i] with subset[i - 1]
    else if (i > 0 && subset[i - 1]->data_count == MINIMUM)
    {
        if (subset[i]->is_leaf())
        {
            delete_item(data, i - 1, data_count, hold);

            merge(subset[i - 1]->data,
                  subset[i - 1]->data_count,
                  subset[i]->data,
                  subset[i]->data_count);

            subset[i - 1]->next = subset[i]->next;

            delete_item(subset, i, child_count, tree_hold);
            delete tree_hold;

        }
        else
        {
            delete_item(data, i - 1, data_count, hold);
            subset[i - 1]->data[subset[i - 1]->data_count] = hold;
            subset[i - 1]->data_count++;

            merge(subset[i - 1]->data,
                subset[i - 1]->data_count,
                subset[i]->data,
                subset[i]->data_count);

            merge(subset[i - 1]->subset,
                subset[i - 1]->child_count,
                subset[i]->subset,
                subset[i]->child_count);

            delete_item(subset, i, child_count, tree_hold);
            delete tree_hold;
        }      
    }
    // Case 4: Combine subset[i] with subset[i + 1]
    else if (i + 1 < child_count && subset[i + 1]->data_count == MINIMUM)
    {

        if (subset[i]->is_leaf())
        {
            delete_item(data, i, data_count, hold);

            merge(subset[i]->data,
                subset[i]->data_count,
                subset[i + 1]->data,
                subset[i + 1]->data_count);

            delete_item(subset, i, child_count, tree_hold);
            delete tree_hold;

            if (i > 0)
                subset[i - 1]->next = subset[i];
        }
        else
        {
            delete_item(data, i, data_count, hold);
            subset[i]->data[subset[i]->data_count] = hold;
            subset[i]->data_count++;

            merge(subset[i]->data,
                subset[i]->data_count,
                subset[i + 1]->data,
                subset[i + 1]->data_count);

            merge(subset[i]->subset,
                subset[i]->child_count,
                subset[i + 1]->subset,
                subset[i + 1]->child_count);

            delete_item(subset, i + 1, child_count, tree_hold);
            delete tree_hold;
        }
        
    }
    else
    {
        std::cout << "Something went wrong in fix_shortage()" << std::endl;
    }
}

template<typename Item>
void BPTree<Item>::fix_excess(int index)
{
    Item mid;
    mid = subset[index]->data[(MAXIMUM + 1) / 2];

    BPTree<Item>* new_child;
    new_child = new BPTree<Item>(dups_ok);

    // Split the data array between the 2 children
    split(subset[index]->data,
          subset[index]->data_count, 
          new_child->data, 
          new_child->data_count);

    // Split the subset array between the 2 children
    split(subset[index]->subset,
          subset[index]->child_count,
          new_child->subset,
          new_child->child_count);

    new_child->next = subset[index]->next;

    // Inserting the new child to the subset array
    insert_item(subset, index + 1, child_count, new_child);

    // Inserting the element passed up from the split into the data array
    insert_item(data, index, data_count, mid);

    if (subset[index]->is_leaf())
    {
        insert_item(subset[index + 1]->data, 0, subset[index + 1]->data_count, mid);

        for (int i = 0; i < child_count - 1; i++)
            subset[i]->next = subset[i + 1];
    }
}

template<typename Item>
void BPTree<Item>::loose_insert(const Item& entry)
{
    int i;
    bool found;
    i = first_ge(data, data_count, entry);
    found = (i < data_count && data[i] == entry);

    // Found and is a leaf
    if (found && this->is_leaf())
    {
        data[i] = entry;     
    }
    // Not found and is a leaf 
    else if (!found && this->is_leaf())
        insert_item(data, i, data_count, entry);   
    // Not found and not a leaf 
    else if (!found && !this->is_leaf())
    {
        subset[i]->loose_insert(entry);
        if (subset[i]->data_count > MAXIMUM)
            this->fix_excess(i);
    }
    // Found and not a leaf
    else if (found && !this->is_leaf())
    {
        subset[i + 1]->loose_insert(entry);
        if (subset[i + 1]->data_count > MAXIMUM)
            this->fix_excess(i);
    }
    // Shouldn't happen
    else
    {
        std::cout << "Something went wrong in loose_insert()" << std::endl;
    }
}

template<typename Item>
bool BPTree<Item>::is_valid()
{
    bool valid;
    valid = true;

    if(!is_sorted(data, data_count))
        return false;

    if(data_count > MAXIMUM)
        return false;

    if(!this->is_leaf() && child_count < data_count)
        return false;

    if(!this->is_leaf())
    {
        for (int i = 0; i < child_count; i++)
        {
            valid = subset[i]->is_valid();

            if (i < child_count - 1 && subset[i]->data[subset[i]->data_count - 1] > data[i])
                return false;
        }
        if (subset[child_count - 1]->data[0] < data[data_count - 1])
            return false;
    }
    return valid;
}

template<typename Item>
BPTree<Item>* BPTree<Item>::get_smallest_node()  
{
    if (this->is_leaf())
        return this;
    else
        return subset[0]->get_smallest_node();
}

template<typename Item>
typename BPTree<Item>::Iterator BPTree<Item>::begin() 
{
    return Iterator(get_smallest_node());
}

template<typename Item>
typename BPTree<Item>::Iterator BPTree<Item>::at(Item target) 
{
    assert (this->contains(target));

    typename BPTree<Item>::Iterator iter(this);
    int index;

    while (!iter.it->is_leaf())
    {

        index = first_ge(iter.it->data, iter.it->data_count, target);

        if (iter.it->data[index] == target)
            iter = iter.it->subset[index + 1];
        else
            iter = iter.it->subset[index];
    }

    iter.key_ptr = first_ge(iter.it->data, iter.it->data_count, target);
    return iter;

}

template<typename Item>
typename BPTree<Item>::Iterator BPTree<Item>::end() 
{
    return Iterator(NULL);
}

template<typename Item>
typename BPTree<Item>::Iterator BPTree<Item>::upper_bound(const Item& key)
{
    typename BPTree<Item>::Iterator iter(this);
    int index = 0;

    while (!iter.it->is_leaf())
    {

        index = first_ge(iter.it->data, iter.it->data_count, key);

        if (index < iter.it->data_count && iter.it->data[index] == key)
            iter = iter.it->subset[index + 1];
        else
            iter = iter.it->subset[index];
    }

    iter.key_ptr = index = first_ge(iter.it->data, iter.it->data_count, key);
    if (index < iter.it->data_count && iter.it->data[index] != key)
        return iter;
    else
    {
        ++iter;
        return iter;
    }
    
}

template<typename Item>
typename BPTree<Item>::Iterator BPTree<Item>::lower_bound(const Item& key)
{
    typename BPTree<Item>::Iterator iter(this);
    int index = 0;

    while (!iter.it->is_leaf())
    {

        index = first_ge(iter.it->data, iter.it->data_count, key);

        if (index < iter.it->data_count && iter.it->data[index] == key)
            iter = iter.it->subset[index + 1];
        else
            iter = iter.it->subset[index];
    }

    iter.key_ptr = index = first_ge(iter.it->data, iter.it->data_count, key);

    if (index >= iter.it->data_count)
    {
        ++iter;
        return iter;
    }
    else
        return iter;  
}


#endif
