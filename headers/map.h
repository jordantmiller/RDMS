/*********************************************************
 *   AUTHOR        : Jordan Miller
 *   Assignment    : Final Project
 *   CLASS         : CS 008
 *   SECTION       : MTWR 4:00p
 *   Due Date      : 12/06/2019
 ********************************************************
FILE: map.h
CLASSES CONTAINED: 
    Map:
        a class for storing key value pairs
        using a BTree as the underlying structure.

    Pair: 
        a struct to store the key, value pair stored in
        the map.

CONSTRUCTOR:
    Map()
        Postcondition: An empty map has been constructed

ACCESSORS:
    int size() const
        Postcondition: The number of elements in the Map
                       has been returned.

    bool empty() const
        Postcondition: A bool indicating if the Map is empty
                       has been returned.

    V& operator[](const K& key) 
        Postcondition: A reference to the value paired with
                       key has been returned.

    
    V& at(const K& key) *** has const version ***
        Postcondition: Alternate syntax for [].            

    bool contains(const Pair<K, V>& target) const
        Postcondition: A bool indicating the existance of the pair with
                       target in the Map.

    V get(const K& key)
        Postcondition: Returns a non-reference value of the pair with key

MUTATORS:
    void insert(const K& k, const V& v)
        Postcondition: If there already exists a pair with the key == k,
                       then the value is updated to v.  Else, a new pair
                       is created with the key = k and value = v.

    void erase(const K& key)
        Postcondition: There is no longer a Pair in the Map
                       with key.

    void clear()
        Postcondition: The Map no contains no entries.    



*/



#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "./BPTree.h"

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K())
    {
        key = k;
    }
    Pair(const K& k, const V& v)
    {
        key = k;
        value = v;
    }

    friend std::ostream& operator <<(std::ostream& outs, const Pair<K, V>& print_me)
    {
        outs << print_me.key << " : " << print_me.value;
        return outs;
    }

    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return lhs.key == rhs.key;
    }
    friend bool operator !=(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return lhs.key != rhs.key;
    }

    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return lhs.key < rhs.key;
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return lhs.key > rhs.key;
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return lhs.key <= rhs.key;
    }
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return lhs.key >= rhs.key;
    }
};

template< typename K, typename V>
class Map
{
public:
    typedef BPTree<Pair<K, V> > map_base;

    class Iterator
    {
    public:
        friend class Map;
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
            ++(this->_it);
            return *this;            
        }
        Pair<K, V> operator *()
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

    Map();

    int size() const { return key_count; }
    bool empty() const { return map.empty(); }

//  Element Access
    V& operator[](const K& key);
    V& at(const K& key);
    const V& at(const K& key) const;

//  Modifiers
    void create_key(const K& k);
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    bool contains(const Pair<K, V>& target) const;

    friend std::ostream& operator<<(std::ostream& outs, const Map<K, V>& print_me){
        outs << print_me.map << std::endl;
        return outs;
    }

    // Iterator Functions
    Iterator begin();
    Iterator end();
    Iterator upper_bound(const K& key);
    Iterator lower_bound(const K& key);

private:
    
    int key_count;
    BPTree<Pair<K, V> > map;
};

template<typename K, typename V>
Map<K, V>::Map()
{
    key_count = 0;
}

template<typename K, typename V>
V& Map<K, V>:: operator[](const K& key)
{
    return map.get(Pair<K, V>(key)).value;
}

template<typename K, typename V>
V& Map<K, V>::at(const K& key) 
{
    return map.get(Pair<K, V>(key)).value;
}

template<typename K, typename V>
const V& Map<K, V>::at(const K& key) const
{
    return map.get(Pair<K, V>(key)).value;
}

template<typename K, typename V>
void Map<K, V>::create_key(const K& k)
{
    map.insert(Pair<K, V>(k));
}

template<typename K, typename V>
void Map<K, V>::insert(const K& key, const V& value)
{
    map.insert(Pair<K, V>(key, value));
}

template<typename K, typename V>
void Map<K, V>::erase(const K& key)
{
    map.remove(Pair<K, V>(key));
}

template<typename K, typename V>
void Map<K, V>::clear()
{
    map.clear_tree();
    key_count = 0;
}

template<typename K, typename V>
V Map<K, V>::get(const K& key)
{
    return map.get(Pair<K, V>(key)).value;
}

template<typename K, typename V>
bool Map<K, V>::contains(const Pair<K, V>& target) const
{
    return map.contains(target);
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::begin()
{
    return Iterator(map.begin());
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end()
{
    return Iterator(map.end());
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::upper_bound(const K& key)
{
    return Iterator(map.upper_bound(Pair<K, V>(key)));
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::lower_bound(const K& key)
{
    return Iterator(map.lower_bound(Pair<K, V>(key)));
}


#endif
