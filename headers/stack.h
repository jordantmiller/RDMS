/*********************************************************
 *   AUTHOR        : Jordan Miller
 *   Assignment    : Stack Class
 *   CLASS         : CS 008
 *   SECTION       : MTWR 4:00p
 *   Due Date      : 09/02/2019
 ********************************************************
FILE: stack.h

CLASS PROVIDED: Stack
    A container class with FILO functionality.

VALUE SEMANTICS:
    The Copy constructor and assignment operator may be used with
    this class. 
 */
#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <cstdlib>
#include "./linked_list_functions.h"

using namespace jmiller;

template<class T>
class Stack
{
public:

        
    class Iterator
    {
        friend class Stack;
    private:
        Node<T>* current;
    public:
        Iterator() : current(NULL) {}
        Iterator(Node<T>* ptr) : current(ptr) {}
        T& operator*() const { return current->_item; }

        // overloaded prefix increment operator
        Iterator& operator++() 
        {
            assert(current != NULL);
            current = current->_next; 
            return *this;
        }
        // overloaded postfix increment operator
        Iterator operator++(int) 
        {
            assert(current != NULL);
            Iterator temp(this->current);
            current = current->_next;
            return temp;
        }
        // overloaded prefix decrement operator
        Iterator& operator--() 
        {
            assert(current != NULL);
            current = current->_prev; 
            return *this;
        }
        // overaloaded postfix decrement operator
        Iterator operator--(int) 
        {
            assert (current != NULL);
            Iterator temp = *this;
            current = current->_prev;
            return temp;
        }
        bool operator==(const Iterator& right) const
        {
            return this->current == right.current;
        }
        bool operator!=(const Iterator & right) const
        {
            return this->current != right.current;
        }
    };

    // CONSTRUCTORS
    Stack(const Stack<T>& other);
    Stack():_top(NULL){}

    // DESTRUCTOR
    ~Stack();

    // ASSIGNMENT OPERATOR
    Stack<T>& operator =(const Stack<T>& rhs);

    // MUTATORS
    void push(T item);
    T pop();

    // ACCESSORS
    T top() {return _top->_item;}
    bool is_empty() {return (_top == NULL);}
    size_t size() {return _size;}

    // ITERATOR FUNCTIONS
    Iterator begin() {return Iterator(_top);}
    Iterator end() {return Iterator(NULL);}

    // OVERLOADED PRINT OPERATOR
    friend std::ostream& operator <<(std::ostream& outs, const Stack& s)
    {
        return print_list_forward(s._top, outs);
    }

private:
    Node<T>* _top;
    size_t _size;
};

template<class T>
Stack<T>::~Stack()
// Postcondition: all memory released to the stack
{
    _delete_all(_top);
}

template<class T>
Stack<T>::Stack(const Stack<T>& other)
// Precondition: other is a Stack object
// Postcondition: the calling object is a separate copy of other.
{
    _copy_list(other._top, _top);
    _size = other._size;
}

template<class T>
Stack<T>& Stack<T>::operator =(const Stack<T>& rhs)
// Precondition: rhs is a Stack object
// Postcondition: the calling object is a separate copy of rhs
{
    _delete_all(_top);
    _copy_list(rhs._top, _top);
    _size = rhs._size;
    return *this;
}

template<class T>
void Stack<T>::push(T item)
// Precondition: iten is of type T
// Postcondition: A new node containing item has been placed on top of the stack.
{
    _top = _insert_head(_top, item);
}

template<class T>
T Stack<T>::pop()
// Precondition: the calling object is not an empty Stack.
// Postcondition: The top item in a stack has been deleted and its value returned.
{
    assert (_top != NULL);

    T popped;
    popped = _top->_item;
    _delete_head(_top);
    return popped;
}

#endif
