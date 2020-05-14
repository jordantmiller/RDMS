/*********************************************************
 *   AUTHOR        : Jordan Miller
 *   Assignment    : Final Project
 *   CLASS         : CS 008
 *   SECTION       : MTWR 4:00p
 *   Due Date      : 12/06/2019
 ********************************************************
FILE: mmap.h
CLASSES CONTAINED: 
    MMap:
        a class for storing key value pairs
        using a BTree as the underlying structure.

    MPair: 
        a struct to store the key, value pair stored in
        the MMap.

CONSTRUCTOR:
    MMap()
        Postcondition: An empty MMap has been constructed

ACCESSORS:
    int size() const
        Postcondition: The number of elements in the MMap
                       has been returned.

    bool empty() const
        Postcondition: A bool indicating if the MMap is empty
                       has been returned.

    V& operator[](const K& key) *** const varient exists ***
        Postcondition: A reference to the value paired with
                       key has been returned.
    
    bool contains(const Pair<K, V>& target) const
        Postcondition: A bool indicating the existance of the pair with
                       target in the MMap.

    jmiller::Vector<V> &get(const K& key)
        Postcondition: Returns a reference to the vector keyed with key.

MUTATORS:
    void insert(const K& k, const V& v)
        Postcondition: If there already exists a pair with the key == k,
                       then the value is updated to v.  Else, a new pair
                       is created with the key = k and value = v.

    void erase(const K& key)
        Postcondition: There is no longer a Pair in the MMap
                       with key.

    void clear()
        Postcondition: The MMap no contains no entries.    

*/
#ifndef MMAP_H
#define MMAP_H

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "./vector.h"
#include "./BPTree.h"

template <typename K, typename V>
struct MPair{
    K key;
    jmiller::Vector<V> value_list;

    MPair(const K& k=K())
    {
        key = k;
    }
    MPair(const K& k, const V& v)
    {
        key = k;
        value_list.push_back(v);
    }
    MPair(const K& k, const jmiller::Vector<V>& vlist)
    {
        key = k;
        value_list = vlist;
    }

    friend std::ostream& operator <<(std::ostream& outs, const MPair<K, V>& print_me)
    {
        outs << print_me.key << " : [  ";
        for (int i = 0; i < print_me.value_list.size(); i++)
            outs << print_me.value_list[i] << "  ";

        outs << "]";

        return outs;
    }
    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key == rhs.key;
    }
    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key != rhs.key;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key < rhs.key;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key <= rhs.key;
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key > rhs.key;
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key >= rhs.key;
    }

};

template <typename K, typename V>
class MMap
{
public:
    typedef BPTree<MPair<K, V> > map_base;

    class Iterator
    {
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it = NULL): _it(it){}

        Iterator operator ++(int unused)
        {
            Iterator temp;
            temp = *this;

            ++(*this);
            return temp;
        }
        Iterator operator ++()
        {
            ++this->_it;
            return *this;            
        }
        MPair<K, V> operator *()
        {
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return (lhs._it == rhs._it);
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return (lhs._it != rhs._it);
        }
    private:
        typename map_base::Iterator _it;

    };

    MMap();

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    const jmiller::Vector<V>& operator[](const K& key) const;
    jmiller::Vector<V>& operator[](const K& key);

//  Modifiers
    void create_key(const K& k);
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

//  Operations:
    bool contains(const K& key) const ;
    jmiller::Vector<V> &get(const K& key);

    int count(const K& key);

    // Iterator Functions
    Iterator begin();
    Iterator end();
    Iterator upper_bound(const K& key);
    Iterator lower_bound(const K& key);


    friend std::ostream& operator<<(std::ostream& outs, const MMap<K, V>& print_me){
        outs << print_me.mmap << std::endl;
        return outs;
    }

// private:
    int key_count;
    BPTree<MPair<K, V> > mmap;
};

template<typename K, typename V>
void MMap<K, V>::create_key(const K& k)
{
    if (mmap.contains(k))
        return;
    else
        mmap.insert(MPair<K, V>(k));
}

template<typename K, typename V>
void MMap<K, V>::insert(const K& k, const V& v)
{
    if (mmap.contains(k))
        mmap.get(k).value_list.push_back(v);
    else
        mmap.insert(MPair<K, V>(k, v)); 
}

template<typename K, typename V>
void MMap<K, V>::erase(const K& key)
{
    mmap.remove(MPair<K, V>(key));
}

template<typename K, typename V>
void MMap<K, V>::clear()
{
    mmap.clear_tree();
}

template<typename K, typename V>
bool MMap<K, V>::contains(const K& key) const
{
    return mmap.contains(MPair<K, V>(key));
}

template<typename K, typename V>
MMap<K, V>::MMap()
{
    key_count = 0;
}

template<typename K, typename V>
int MMap<K, V>::size() const
{
    return key_count;
}

template<typename K, typename V>
bool MMap<K, V>::empty() const
{
    return key_count == 0;
}

template<typename K, typename V>
const jmiller::Vector<V>& MMap<K, V>::operator[](const K& key) const
{
    return mmap.get(MPair<K, V>(key)).value_list;
}

template<typename K, typename V>
jmiller::Vector<V>& MMap<K, V>::operator[](const K& key)
{
    if (mmap.contains(MPair<K, V>(key)))
        return mmap.get(MPair<K, V>(key)).value_list;
    else
    {
        this->create_key(key);
        return mmap.get(MPair<K, V>(key)).value_list;
    }
    
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::begin()
{
    return Iterator(mmap.begin());
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::end()
{
    return Iterator(mmap.end());
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::upper_bound(const K& key)
{
    return Iterator(mmap.upper_bound(MPair<K, V>(key)));
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::lower_bound(const K& key)
{
    return Iterator(mmap.lower_bound(MPair<K, V>(key)));
}


#endif