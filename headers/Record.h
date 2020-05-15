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
 The Record class takes as input a vector of fields
 and can write those fields into a binary file.

 It is also able to read from a binary file and
 return a vector where each element is an item
 in that record.
 
 */

#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include "./vector.h"

typedef jmiller::Vector<std::string> Vectorstr;

class Record
{
public:
    // CONSTRUCTORS
    Record();
    Record(Vectorstr entry);

    // FILE FUNCTIONS
    Vectorstr get_fields();
    long write(std::fstream& outs);
    long read(std::fstream& ins, long recno);

private:
    static const int ROW_MAX = 10;
    static const int COL_MAX = 200;
    char rec_data[ROW_MAX][COL_MAX];

};

Record::Record()
{
    for (int i = 0; i < ROW_MAX; i++)
        rec_data[i][0] = '\0';
}

Record::Record(Vectorstr entry)
{
    for (int i = 0; i < ROW_MAX; i++)
        rec_data[i][0] = '\0';
    
    for (int i = 0; i < entry.size(); i++)
        strcpy(rec_data[i], entry[i].c_str());
}

Vectorstr Record::get_fields()
{
    Vectorstr fields;

    for (int i = 0; i < ROW_MAX; i++)
        if (rec_data[i][0] != '\0')
            fields.push_back(std::string(rec_data[i]));

    return fields;
}

long Record::write(std::fstream& outs)
{
    outs.seekg(0, outs.end);
    long pos = outs.tellp();

    outs.write(&rec_data[0][0], sizeof(rec_data));

    return pos / sizeof(rec_data);
}

long Record::read(std::fstream& ins, long recno)
{
    long pos;
    pos = recno * sizeof(rec_data);

    ins.seekg(pos, std::ios_base::beg);

    ins.read(&rec_data[0][0], sizeof(rec_data));

    return ins.gcount() / sizeof(rec_data);
}





#endif