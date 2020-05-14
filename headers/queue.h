/*********************************************************
 *   AUTHOR        : Jordan Miller
 *   Assignment    : Queue Class
 *   CLASS         : CS 008
 *   SECTION       : MTWR 4:00p
 *   Due Date      : 09/02/2019
 ********************************************************
FILE: queue.h

CLASS PROVIDED: Queue
    A container class with FIFO functionality.

VALUE SEMANTICS:
    The Copy constructor and assignment operator may be used with
    this class. 
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include "./linked_list_functions.h"

using namespace jmiller;

template<class T>
class Queue
{   
    
public:
        class Iterator
        {
            friend class Queue;
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
    Queue():head(NULL), tail(NULL) {}
    Queue(const Queue<T>& other);

    // DESTRUCTOR
    ~Queue();

    // OVERLOADED ASSIGNMENT OPERATOR
    Queue<T>& operator =(const Queue<T>& rhs);

    // MUTATORS
    void push(T item);
    T pop();

    // ACCESSORS
    bool is_empty();
    T front();
    size_t size() {return _size;}

    // ITERATOR FUNCTIONS
    Iterator begin() {return Iterator(head);}
    Iterator last() {return Iterator(tail);}
    Iterator end() {return Iterator(NULL);}

    // OVERLOADED PRINT OPERATOR
    friend std::ostream& operator <<(std::ostream& outs, const Queue& q)
    {
        return print_list_forward(q.head, outs);
    }


private:
    Node<T>* head;
    Node<T>* tail;
    size_t _size;
};

template<class T>
Queue<T>::~Queue()
// Precondition: None
// Postcondition: All memory released to heap
{
    _delete_all(head);
}

template<class T>
Queue<T>::Queue(const Queue<T>& other)
// Precondtion: other is a Queue object
// Postcondition: The calling object is not a separate copy of other.
{
    tail = _copy_list(other.head, head);
    _size = other._size;
}

template<class T>
Queue<T>& Queue<T>::operator =(const Queue<T>& rhs)
// Precondition: rhs is a Queue object
// Postcondition: the calling object is a copy of rhs
{
    _delete_all(head);
    tail = _copy_list(rhs.head, head);
    _size = rhs._size;
    return *this;
}

template<class T>
void Queue<T>::push(T item)
// Precondition: item is of type T
// Postcondition: A new node containing item has been placed at the back of the queue
{
    if (head == NULL)
        head = tail = _insert_after(tail, item);
    else
        tail = _insert_after(tail, item);
    
    _size += 1;
}

template<class T>
T Queue<T>::pop()
// Precondition: the calling objext is not an empty queue
// Postcondition: The item at the front of the queue has been returned and deleted
{
    assert(head != NULL);
    
    T popped;
    popped = head->_item;
    _delete_head(head, tail);
    _size -= 1;

    return popped;
}

template<class T>
bool Queue<T>::is_empty()
// Postcondition: A bool has been returned indicating if the list is empty.
{
    return (head == NULL);
}

template<class T>
T Queue<T>::front()
// Postcondition: The value of the first node in the queue has been returned.
{
    assert(head != NULL);

    return head->_item;
}

#endif
