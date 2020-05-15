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
/*
    FILE: linked_list_functions.h

    STRUCT PROVIDED:
        Node:
            A templated node to be used in a linked list
    
    FUNCTIONS PROVIDED: Provides all functions needed for classes based on linked lists
        Node<T>* _init_head(Node<T>*& head)
            Precondition: head is the head pointer for a class which uses a linked list
            Postcondition: head has been set to NULL. A Node<T> pointer is returned.

        bool _is_empty(const Node<T>* head)
            Precondition: head is the head pointer for a class which uses a linked list
            Postcondition: a bool has been returned. True if head is NULL, else false.

        Node<T>* _insert_head(Node<T>* head, const T &value)
            Precondition: head is the head pointer for a class which uses a linked list
                          and value is of type T.
            Postcondition: A Node containing value has been placed at the start of the linked list.
                           a pointer pointing to this new first node is returned. To set head to the
                           new pointer the function should be called as such: 
                           head = _insert_head(head, value);

        Node<T>* _insert_after(Node<T>* previous_ptr, const T &value)
            Precondition: previous_ptr is a Node<T> pointer for a linked list. value 
                          is of type T.
            Postcondition: A new node has been placed in the position after previous_ptr
                           and all new connections are made.  Can be used on an empty list.
                           If used for setting a new tail the function should be called as such:
                           tail = _insert_after(tail, value);

        Node<T>* _insert_before(Node<T>* after_ptr, const T &value)
            Precondition: after_ptr is a Node<T> pointer. value is of type T.
            Postcondition: A new node has been placed before the position of the
                           given pointer.  Currently not usable on an empty list.
        
        T _delete_head(Node<T>*& head)
            Precondition: head is the head pointer for a class which uses a linked list
            Postcondition: The first node in a linked list has been deleted properly and
                           head is now set to the new first node in the list.

        T _delete_head(Node<T>*& head, Node<T>*& tail)
            Precondition: head and tail are pointers to Nodes in a linked list.
            Postcondition: The first node in a linked list has been deleted properly
                           and the pointers head and tail are set to the correct positions
                           for all cases.

        T _delete_after(Node<T>*& previous_ptr, Node<T>*& head)
            Precondition: previous_ptr is a Node pointer and head is a Node pointer
                          to the first item in a linked list. 
            Postcondition: The node after previous_ptr has been deleted and head is set to
                           the correct postition in cases where it is required.

        void _delete_all(Node<T>*& head)
            Precondition: head is a pointer to the first item in a linked list.
            Postcondition: All nodes of a linked list have been deleted and memory released.

        Node<T>* _copy_list(const Node<T>* source, Node<T>*& dest)
            Precondition: source and dest are pointers to the first nodes in linked lists.
            Postcondition: All nodes of dest are deleted.  Dest now is a separate copy of source.
                           the last node in the list pointed to by dest has been returned. 
                           If a head and tail are being maintained it is reccomended that the
                           functions be called as such:
                           tail = _copy_list(source, head);

        std::ostream& print_list_forward(const Node<T>* head, std::ostream& outs=std::cout)
            Precondition: head is the pointer to the first node in a linked list or a nullptr.
            Postcondition: A linked list has been printed with formatting from head to the end of
                           the list.

        std::ostream& print_list_backward(const Node<T>* tail, std::ostream& outs=std::cout)
            Precondition: tail is the pointer to the last node in a linked list or a nullptr.
            Postcondition: A linked list has been printed with formatting from tail to head of
                           the list.





*/

#ifndef LINKED_LIST_FUNCTIONS_H
#define LINKED_LIST_FUNCTIONS_H

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <assert.h>

namespace jmiller
{

    template <class T>
    struct Node
    {
        T _item;
        Node<T>* _next;
        Node<T>* _prev;

        Node<T>(
            const T& item = T(), 
            Node<T>* next = NULL, 
            Node<T>* prev = NULL
        ):_item(item), _next(next), _prev(prev){}

        friend std::ostream& operator <<(std::ostream& outs,
                                         const Node<T>& print_me)
        {
            outs << "[" << print_me._item << "]";
            return outs;
        }
    };

    template <class T>
    Node<T>* _init_head(Node<T>*& head)
    {
        return (head = NULL);
    }

    template <class T>
    bool _is_empty(const Node<T>* head)
    {
        return (head == NULL);
    }

    template <class T>
    Node<T>* _insert_head(Node<T>* head, const T &value)
    {        
        head = new Node<T>(value, head);
        
        if (head->_next != NULL)
            head->_next->_prev = head;

        return head;
    }

    template <class T>
    Node<T>* _insert_after(Node<T>* previous_ptr, const T &value)
    {   

        if (previous_ptr == NULL)
        {
            previous_ptr = new Node<T>(value);
            return previous_ptr;
        }
        else
        {
        previous_ptr->_next = new Node<T>(value, previous_ptr->_next, previous_ptr);

        if (previous_ptr->_next->_next != NULL)
            previous_ptr->_next->_next->_prev = previous_ptr->_next;

        return previous_ptr->_next;
        }
    }

    template <class T>
    Node<T>* _insert_before(Node<T>* after_ptr, const T &value)
    {
        assert(after_ptr != NULL);

        after_ptr->_prev = new Node<T>(value, after_ptr, after_ptr->_prev);

        if(after_ptr->_prev->_prev != NULL)
            after_ptr->_prev->_prev->_next = after_ptr->_prev;

        return after_ptr->_prev;

    }

    template <class T>
    T _delete_head(Node<T>*& head)
    {
        assert(head != NULL);

        Node<T>* condemned;
        T value;

        condemned = head;
        value = condemned->_item;
        head = head->_next;

        delete condemned;

        if (head != NULL)
            head->_prev = NULL;

        return value;
    }

    template <class T>
    T _delete_head(Node<T>*& head, Node<T>*& tail)
    {
        assert(head != NULL);

        Node<T>* condemned;
        T value;

        condemned = head;
        value = condemned->_item;
        head = head->_next;

        delete condemned;

        if (head == NULL)
        {
            tail = NULL;
            return value;
        }
        
        head->_prev = NULL;
            
        return value;
    }

    template <class T>
    T _delete_after(Node<T>*& previous_ptr, Node<T>*& head)
    {
        assert(previous_ptr != NULL);

        T value;

        if(previous_ptr->_next == NULL)
        {
            value = previous_ptr->_item;
            delete previous_ptr;
            head = previous_ptr = NULL;
        }
        else
        {
            value = previous_ptr->_next->_item;

            Node<T>* condemned;
            condemned = previous_ptr->_next;

            previous_ptr->_next = condemned->_next;

            if(condemned->_next != NULL)
                condemned->_next->_prev = previous_ptr;

            delete condemned;
        }
        
        return value;        
    }

    template <class T>
    void _delete_all(Node<T>*& head)
    {
        while (head != NULL)
            _delete_head(head);
    }

    template <class T>
    void _delete_all(Node<T>*& head, Node<T>*& tail)
    {
        while (head != NULL)
            _delete_head(head, tail);
    }

    template <class T>
    Node<T>* _copy_list(const Node<T>* source, Node<T>*& dest)
    {
        if (source == NULL)
            return (dest = NULL);
        else
        {
            Node<T>* new_head;
            Node<T>* new_tail;

            new_head = dest = new Node<T>(source->_item);

            while ((source = source->_next) != NULL)
                dest = _insert_after(dest, source->_item);

            new_tail = dest;
            dest = new_head;

            return new_tail;
        }
    }

    template <class T>
    std::ostream& print_list_forward(const Node<T>* head, std::ostream& outs=std::cout)
    { 
        if (head == NULL)
            outs << "|||";
        else
        {        
            outs << "|||<-";
            while (head != NULL)
            {
                outs << *head;

                if(head->_next == NULL)
                    outs << "->|||";
                else
                    outs << "<=>";

                head = head->_next;
            }
        }
        return outs;
    }

    template <class T>
    std::ostream& print_list_backward(const Node<T>* tail, std::ostream& outs=std::cout)
    { 
        if (tail == NULL)
            outs << "|||";
        else
        {        
            outs << "|||<-";
            while (tail != NULL)
            {
                outs << *tail;

                if(tail->_prev == NULL)
                    outs << "->|||";
                else
                    outs << "<=>";

                tail = tail->_prev;
            }
        }
        return outs;
    }
}

#endif 