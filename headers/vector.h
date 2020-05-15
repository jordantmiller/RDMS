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
FILE: vector.h

CLASS PROVIDED: List
    A general list class using linked list functions.

VALUE SEMANTICS:
    The Copy constructor and assignment operator may be used with
    this class. 

CONSTRUCTORS:
    Vector()
        Postcondition: An empty vector has been instantiated.

    Vector(const Vector<Item>& other)
        Precondition: other is a Vector<Item> object.
        Postcondition: The calling object is now an identical but distinct copy of
                       other.

ACCESSORS:
    std::size_t size()
        Postcondition: The number of elements stored in the calling object
                       has been returned as a std::size_t type.

    std::size_t size()
        Postcondition: The current element capacity of the calling object has been returned
                       as a std::size_t type.
                       
    std::size_t max_capacity()
        Postcondition: The maximum number of elements it's possible to store in a 
                       Vector has been returned as std::size_t

    bool empty()
        Postcondition: A bool indicating whether or not the vector conains
                       any elements has been returned.  true is the vector
                       contains 0 elements, false otherwise. 

    int& operator[](int index) *has const varient*
        Postcondition: The item stored at _data[index] has been returned.
                       The item can be altered (v[index] = 42) or used by 
                       a const Vector.

MUTATORS:
    void reserve(std::size_t n)
        Postcondition: The vector now contains enough capacity to hold
                       at least n elements. Can be used to increase efficiency
                       by avoiding excessive resizing.

    void push_back(Item new_item)
        Postcondition: _data[size - 1] now contains new_item. _size has been 
                       incremented to reflect the added element.

    void swap(unsigned int first, unsigned int second)
        Precondition: first and second are less than _size.
        Postcondition: The items stored in _data[first] and _data[second]
                       have been swaped.

    Item pop_back()
        Postcondition: The element at _data[size - 1] has been returned and
                       removed from the Vector.

    
*/
#ifndef VECTOR_H
#define VECTOR_H

#define MIN_CAP 8

#include <cstdlib>
#include <assert.h>
#include <algorithm>


namespace jmiller
{
    template <class Item>
    class Vector
    {
        public:
            // CONSTRUCTORS
            Vector(): _size(0), _capacity(MIN_CAP), _data(new Item [MIN_CAP]) {}
            Vector(const Vector<Item>& other);

            // ASSIGNMENT OPERATOR
            Vector<Item>& operator =(const Vector<Item>& rhs);

            // DESTRUCTOR
            ~Vector();
            
            // ACCESSORS
            unsigned int size() const { return _size; }
            unsigned int capacity() const { return _capacity; }
            unsigned int max_capacity() const { return SIZE_MAX; }
            bool empty() { return _size == 0; }
            Item& operator[](int index) { return _data[index]; }
            const Item& operator[](int index) const { return _data[index]; }

            // MUTATORS
            void clear() { _size = 0; }
            void reserve(std::size_t n);
            void push_back(Item new_item);
            void operator +=(Item new_item);
            void operator +=(Vector<Item> new_vector);
            void swap(unsigned int first, unsigned int second);
            Item pop_back();

            friend std::ostream& operator<<(std::ostream& outs,
                                            const Vector<Item>& print_me)
            {
                outs << "{ ";
                for (int i = 0; i < print_me.size(); i++)
                    outs << print_me[i] << "  ";
                
                outs << "} ";

                return outs;
            }

        private:
            // MEMBER VARIABLES
            std::size_t _size;
            std::size_t _capacity;
            Item* _data;

            // HELPER FUNCTIONS
            void resize();
            void shrink_to_fit();

    };

    template<class Item>
    jmiller::Vector<Item>::Vector(const Vector<Item>& other)
    {
        _size = other._size;
        _capacity = other._capacity;

        _data = new Item[_capacity];

        std::copy(other._data, other._data + _size, _data);
    }

    template<class Item>
    jmiller::Vector<Item>& jmiller::Vector<Item>::operator =(const Vector<Item>& rhs)
    {
        delete[] this->_data;

        this->_size = rhs._size;
        this->_capacity = rhs._capacity;

        this->_data = new Item[_capacity];

        std::copy(rhs._data, rhs._data + _size, _data);
    }

    template<class Item>
    jmiller::Vector<Item>::~Vector()
    {
        delete[] _data;
    }

    template<class Item>
    void jmiller::Vector<Item>::push_back(Item new_item)
    {
        if (_size == _capacity)
            this->resize();

        _data[_size++] = new_item;
    }

    template<typename Item>
    void jmiller::Vector<Item>::operator+=(Item new_Item)
    {
        push_back(new_Item);
    }

    template<typename Item>
    void jmiller::Vector<Item>::operator+=(Vector<Item> new_vector)
    {
        for(int i = 0; i < new_vector.size(); i++)
            push_back(new_vector[i]);
    }

    template<class Item>
    Item jmiller::Vector<Item>::pop_back()
    {
        assert(_size > 0);

        if (_size <= _capacity / 4 && _capacity > MIN_CAP)
            this->shrink_to_fit();

        return _data[--_size];
    }

    template<class Item>
    void jmiller::Vector<Item>::reserve(std::size_t n)
    {
        while (n > _capacity)
            _capacity *= 2;

        // Creat new temporary array
        Item* temp;
        temp = new Item[_capacity];

        // Copy current Vector elements into temp
        std::copy( _data, _data + _size, temp);

        // Dealocate memory used by _data and set _data to temp
        delete[] _data;
        _data = temp;
    }

    template<class Item>
    void jmiller::Vector<Item>::resize()
    {
        // Double the vectors capacity
        _capacity *= 2;

        // Creat new temporary array
        Item* temp;
        temp = new Item[_capacity];

        // Copy current Vector elements into temp
        std::copy( _data, _data + _size, temp);

        // Dealocate memory used by _data and set _data to temp
        delete[] _data;
        _data = temp;
    }

    template<class Item>
    void jmiller::Vector<Item>::shrink_to_fit()
    {
        while (_capacity / 2 > _size && _capacity / 2 > MIN_CAP)
            _capacity /= 2;        

        Item* temp;
        temp = new Item[_capacity];

        std::copy( _data, _data + _size, temp);

        delete[] _data;
        _data = temp;
    }

    template<class Item>
    void jmiller::Vector<Item>::swap(unsigned int first, unsigned int second)
    {
        assert(first < _size && second < _size);

        Item temp;
        temp = _data[first];

        _data[first] = _data[second];
        _data[second] = temp;
    }
}

#endif
