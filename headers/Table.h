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

 */

#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include "./BTree.h"
#include "./Record.h"
#include "./map.h"
#include "./mmap.h"
#include "./stack.h"

typedef Map<std::string, MMap<std::string, std::size_t> > mmap_map;
typedef MMap<std::string, size_t>::Iterator mmap_iter;

bool file_exists(const std::string file);

class Table
{
public:
    // CONSTRUCTORS
    Table(const std::string& name, const Vectorstr fields);
    Table(const std::string& name);

    // MUTATORS
    std::size_t insert_into(const Vectorstr values);
    Table select(const Vectorstr columns, const Vectorstr rows);
    Table select_all();


    // PRINT FUNCTION
    friend std::ostream& operator<<(std::ostream& outs,
                                    const Table& print_me)
    {
        std::fstream fs;
        Vectorstr fields;
        Record reader;
        std::size_t recno;
        int counter;
        counter = 1;
        
        fs.open(print_me.file_name.c_str(), 
                std::fstream::in | std::fstream::out | std::fstream::binary);

        std::cout << "\n\nTable name: " << print_me.table_name 
                  << ", records: " << print_me.record_number << std::endl << std::endl;

        std::cout << std::setw(11) << std::left << "  Record ";

        for (int i = 0; i < print_me.field_names.size(); i++)
            std::cout << std::setw(16) << std::left << print_me.field_names[i];

        std::cout <<  "\n  -------------------------------------------------------------------------------------"
                  << std::endl;

        std::cout << std::endl;
        recno = 1;

        while (recno <= print_me.record_number)
        {
            recno += reader.read(fs, recno);
            fields = reader.get_fields();
            std::cout << std::setw(7) << std::right << counter++ << std::setw(4) << std::left << ".";

            for (int i = 0; i < fields.size(); i++)
                std::cout << std::setw(16) << std::left << fields[i];
            
            std::cout << std::endl;
        }

        std::cout << "\n\n" << std::endl;
        return outs;
    }
    
 private:
    static std::size_t serial;
    Map<std::string, int> precedence;
    std::string table_name;
    std::string file_name;
    mmap_map indices;
    Vectorstr field_names;
    Map<std::string, std::size_t> field_indices;
    bool empty;
    std::size_t record_number;

    void set_prec();
    void set_fields(const Vectorstr& field_names);
    jmiller::Vector<std::size_t> get_conditional_indices(const Vectorstr& conditions);
    jmiller::Vector<std::size_t> get_simple_indices(Vectorstr& s_conditions);
    Vectorstr get_rpn(Vectorstr conditions);
    jmiller::Vector<std::size_t> and_vector(jmiller::Vector<std::size_t> v1,
                                            jmiller::Vector<std::size_t> v2);
    jmiller::Vector<std::size_t> or_vector(jmiller::Vector<std::size_t> v1,
                                            jmiller::Vector<std::size_t> v2);
};

Table::Table(const std::string& name, const Vectorstr fields)
{
    table_name = name;
    file_name = ".\\bin\\" + name + ".tbl";
    record_number = 0;
    set_fields(fields);

    std::fstream fs;
    Record field_record(fields);

    // Creates the file if it doesn't exist
    fs.open(file_name.c_str(), std::fstream::out | std::fstream::binary);
    fs.close();

    // Opens the file for read / write
    fs.open(file_name.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary);

    // Write the field names as the first record in the table
    field_record.write(fs);

}

Table::Table(const std::string& name)
{
    std::fstream fs;
    Vectorstr values;
    std::size_t recno;
    Record fields_record;

    table_name = name;
    file_name = ".\\bin\\" + name + ".tbl";
    record_number = 0;

    fs.open(file_name.c_str(), std::fstream::out | std::fstream::in | std::fstream::binary);
    recno = 1;

    if (fs.fail())
        std::cout << file_name << " does not exist." << std::endl;
    else
    {
        fields_record.read(fs, 0);
        values = fields_record.get_fields();
        set_fields(values);
    }

    // Creates the indices tree structure from the file
    while (fields_record.read(fs, recno) != 0)
    {
        values = fields_record.get_fields();

        for (int i = 0; i < values.size(); i++)
        {   
            if (indices[field_names[i]].contains(values[i]))
                indices[field_names[i]][values[i]].push_back(recno);
            else
            {
                indices[field_names[i]].create_key(values[i]);
                indices[field_names[i]][values[i]].push_back(recno);
            }
        }

        recno++;
        record_number++;
    }  
}

std::size_t Table::insert_into(const Vectorstr values)
{
    std::fstream fs;
    Record new_record(values);
    std::size_t recno;

    // Open the binary file that stores the Records
    fs.open(file_name.c_str(), std::fstream::binary | std::fstream::in | std::fstream::app);

    // Write the record and store its location in the file
    // in recno
    recno = new_record.write(fs);

    // For each value in values: 
    //  1. If there already exists a key in the mmap for
    //     values[i] then add the recno to the vector.
    //  2. If there isn't a key in the mmap for values[i]
    //     one is created and the recno pushed back.
    for (int i = 0; i < values.size(); i++)
    {   
        if (indices[field_names[i]].contains(values[i]))
            indices[field_names[i]][values[i]].push_back(recno);
        else
        {
            indices[field_names[i]].create_key(values[i]);
            indices[field_names[i]][values[i]].push_back(recno);
        }
    }  

    record_number = recno;
}

void Table::set_fields(const Vectorstr& fields)
{
    for (int i = 0; i < fields.size(); i++)
    {
        // Creates a map of the fields to their index
        // i.e. {first_name : 0, last_name : 1, dept : 2}
        field_names.push_back(fields[i]);

        // Creates a map with the field mapped to its index
        field_indices.insert(fields[i], i);

        // Creates a Map with each key being a field from 
        // the vector passed into the function. 
        // Each key maps to a multimap whose keys are the
        // values for each field, and whose values are vectors
        // of all record numbers associated with those values.
        // i.e. { first_name : { John : {12, 23, 43, 76}, Joe : {24, 35} } }
        indices.create_key(fields[i]);
    }
}

Table Table::select_all()
{
    Table t(this->table_name);
    return t;
}

Table Table::select(const Vectorstr columns, const Vectorstr conditions)
{
    jmiller::Vector<std::size_t> col_indices;
    jmiller::Vector<std::size_t> row_indices;
    Vectorstr act_columns;
    Vectorstr raw_values;
    Vectorstr selected_values;
    Record reader;

    std::string temp_table_name;

    temp_table_name = "temp\\" + table_name + "_temp";

    if (columns[0] == "*")
        act_columns = field_names;
    else
        act_columns = columns;

    Table t(temp_table_name, act_columns);
    
    // Creates a vector storing the indices of the
    // selected fields.
    for (int i = 0; i < act_columns.size(); i++)
        col_indices.push_back(field_indices[act_columns[i]]);

    if (conditions.size() == 0)
        for (std::size_t i = 1; i <= record_number; i++)
            row_indices.push_back(i);
    else
        row_indices = get_conditional_indices(conditions);

    // Opens a fstream object for the binary file
    std::fstream fs;
    fs.open(file_name.c_str(), 
            std::fstream::out | std::fstream::in | std::fstream::binary);

    // Loop to read the records accoring to the condition,
    // build a vector based on the selected fields, and
    // insert that vector into the new table.
    for (int i = 0; i < row_indices.size(); i++)
    {
        reader.read(fs, row_indices[i]);
        raw_values = reader.get_fields();

        for (int j = 0; j < col_indices.size(); j++)
            selected_values.push_back(raw_values[col_indices[j]]);

        t.insert_into(selected_values);
        selected_values.clear();
    }

    return t;
}

jmiller::Vector<std::size_t> Table::and_vector(jmiller::Vector<std::size_t> v1,
                                               jmiller::Vector<std::size_t> v2)
{
    jmiller::Vector<std::size_t> anded;
    BTree<std::size_t> tree;

    // Inserts every item in v2 into the tree
    for (int i = 0; i < v2.size(); i++)
        tree.insert(v2[i]);

    // Checks every item in v1 agains the tree.
    // If the tree contains the same item as v1
    // then it belongs to the set (v1 and v2)
    // and it added to the vector which will be returned
    for (int i = 0; i < v1.size(); i++)
    {
        if (tree.contains(v1[i]))
            anded.push_back(v1[i]);
    }

    return anded;
}                                               

jmiller::Vector<std::size_t> Table::or_vector(jmiller::Vector<std::size_t> v1,
                                              jmiller::Vector<std::size_t> v2)
{
    jmiller::Vector<std::size_t> ored;
    BTree<std::size_t> tree;

    // Inserts every item in v2 into the tree
    for (int i = 0; i < v2.size(); i++)
        tree.insert(v2[i]);

    // Adds every item that isn't in v2 to
    // ored. So, every item in v1 that isn't
    // also in v2 has been added to ored.
    for (int i = 0; i < v1.size(); i++)
    {
        if (!tree.contains(v1[i]))
            ored.push_back(v1[i]);
    }

    // Adds the items in v2 to the ored.
    // By doing this ored now contains
    // every item which is in either v1 or
    // v2 without any duplicates
    ored += v2;

    return ored;
}                                               
                                              
// Needs testing
jmiller::Vector<std::size_t> Table::get_conditional_indices(const Vectorstr& conditions) 
{
    Vectorstr rpn_conditions;
    Vectorstr simple_rpn;
    jmiller::Vector<std::size_t> v1;
    jmiller::Vector<std::size_t> v2;
    Stack<jmiller::Vector<std::size_t> > vstack;
    rpn_conditions = get_rpn(conditions);

    for (int i = 0; i < rpn_conditions.size(); i++)
    {
        if (precedence.contains(rpn_conditions[i]))
        {
            if (rpn_conditions[i] == "and")
            {
                v1 = vstack.pop();
                v2 = vstack.pop();
                vstack.push(and_vector(v1, v2));
            }
            else if (rpn_conditions[i] == "or")
            {
                v1 = vstack.pop();
                v2 = vstack.pop();
                vstack.push(or_vector(v1, v2));
            }
            else
            {
                // Creates a vector of one condition in rpn
                // ex. { fname, John, = }
                for (int j = 2; j >= 0; j--)
                    simple_rpn.push_back(rpn_conditions[i - j]);

                // Gets the indices based on the condition and stores
                // them in v1
                v1 = get_simple_indices(simple_rpn);

                // Pushes v1 onto a stack
                vstack.push(v1);

                // Clears simple_rpn to take the next condition.
                simple_rpn.clear();
            }
        }
    }  

    if (!vstack.is_empty())
        return vstack.pop();
    else
        return jmiller::Vector<std::size_t>();
      
}

jmiller::Vector<std::size_t> Table::get_simple_indices(Vectorstr& s_conditions)
{
    jmiller::Vector<std::size_t> row_indices;

    if (s_conditions[2] == "=")
    {
        if (indices[s_conditions[0]].contains(s_conditions[1]))
        row_indices = indices[s_conditions[0]][s_conditions[1]];
    }
    else if (s_conditions[2] == ">")
    {
        mmap_iter it;
        for (it = indices[s_conditions[0]].upper_bound(s_conditions[1]); 
             it != indices[s_conditions[0]].end(); ++it)
            row_indices += (*it).value_list;
    }
    else if (s_conditions[2] == ">=")
    {
        mmap_iter it;
        for (it = indices[s_conditions[0]].lower_bound(s_conditions[1]); 
             it != indices[s_conditions[0]].end(); ++it)
            row_indices += (*it).value_list;
    }
    else if (s_conditions[2] == "<")
    {
        mmap_iter it;
        for (it = indices[s_conditions[0]].begin(); 
             it != indices[s_conditions[0]].lower_bound(s_conditions[1]); ++it)
            row_indices += (*it).value_list;
    }
    else if (s_conditions[2] == "<=")
    {
        mmap_iter it;
        for (it = indices[s_conditions[0]].begin(); 
             it != indices[s_conditions[0]].upper_bound(s_conditions[1]); ++it)
            row_indices += (*it).value_list;
    }
    else
        std::cout << "Invalid command got through in get_simple_indices()" << std::endl;

    return row_indices;
}

Vectorstr Table::get_rpn(Vectorstr conditions)
{
    Stack<std::string> stack;
    Vectorstr postfix;

    set_prec();

    // An implementation of the shunting yard algorithm
    // for an SQL statement
    for (int i = 0; i < conditions.size(); i++)
    {
        if(!precedence.contains(conditions[i]))
            postfix.push_back(conditions[i]);
        else
        {
            while (!stack.is_empty() && precedence[conditions[i]] <= precedence[stack.top()])
                postfix.push_back(stack.pop());

            stack.push(conditions[i]);
        }
    }

    while (!stack.is_empty())
        postfix.push_back(stack.pop());

    return postfix;
}

void Table::set_prec()
{
    // Creates a map mapping operator strings to their
    // precedence
    std::string strings[6] = { "=", "<", ">", "<=", ">=" };

    for (int i = 0; i < 5; i++)
        precedence.insert(strings[i], 5);

    precedence.insert("and", 3);
    precedence.insert("or", 2);
}

#endif
