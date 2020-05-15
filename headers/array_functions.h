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
 
 A set of array functions used by several classes in this
 project
 
*/

#ifndef ARRAY_FUNCTIONS_H
#define ARRAY_FUNCTIONS_H

#include <iostream>
#include <cstdlib>
#include <assert.h>


template <class Item>
Item maximal(const Item& a, const Item& b)
{
    if (a > b)
        return a;
    
    return b;
}

template <class Item>
void swap(Item& a, Item& b)
{
    Item temp;
    temp = a;
    a = b;
    b = temp;
}

template <class Item>
int index_of_maximal(Item data[ ], int n)
{
    Item min;
    int index;
    min = data[0];

    for (int i = 1; i < n; i++)
        if (data[i] < min)
        {
            min = data[i];
            index = i;
        }
    return index;
}

template <class Item>
int first_ge(const Item data[ ], int n, const Item& entry) 
{
    int index;
    index = 0;

    while (index < n)
    {
        if (data[index] >= entry)
            return index;
        index++;
    }
    return index;
}   

template <class Item>
void insert_item(Item data[ ], int index, int& n, Item entry)
{
    assert(index <= n);

    for (int i = n; i > index; i--)
        data[i] = data[i - 1];

    data[index] = entry;
    n++;
}   
 
template <class Item>
void ordered_insert(Item data[ ], int& n, Item entry)   
{
    insert_item(data, first_ge(data, n, entry), n, entry);
}        

template <class Item>
void delete_item(Item data[ ], int index, int& n, Item& entry)
{
    assert(index < n);

    entry = data[index];

    for (int i = index; i < n - 1; i++)
        data[i] = data[i + 1];

    n--;
}   

template <class Item>
void merge(Item data1[ ], int& n1, Item data2[ ], int& n2)
{
    for (int i = n2; i > 0; i--)
        data1[n1++] = data2[n2 - i];

    n2 = 0;
}   

template <class Item>
void split(Item data1[ ], int& n1, Item data2[ ], int& n2)
{
    int mid = n1 / 2;

    if (n1 % 2 != 0)
        for (int i = 0; i < mid; i++)
            data2[i] = data1[mid + 1 + i];
    else
        for (int i = 0; i < mid; i++)
            data2[i] = data1[mid + i];

    n1 = n2 = mid;
}   

template <class Item>
void copy_array(Item dest[], const Item src[],
                int& dest_size, int src_size)
{
    for (int i = 0; i < src_size; i++)
    {
        dest[i] = src[i];
    }
    dest_size = src_size;
}        

template<typename Item>
bool is_sorted(Item* array, int size)
{
    if (size < 2)
        return true;
    else
    {
        for (int i = 1; i < size; i++)
            if (array[i] < array[i - 1])
                return false;
    }

    return true;   
}

template<typename Item>
void print_array(Item* a, int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << a[i] << " ";
    }

    std::cout << std::endl;
}


#endif