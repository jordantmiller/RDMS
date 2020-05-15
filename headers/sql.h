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
 An SQL class to direct user input to a Parser
 and use the ptree from the parser to interact with 
 a database.
 
 */

#ifndef MY_SQL_H
#define MY_SQL_H

#include <cstdlib>
#include <string.h>
#include <iostream>
#include <fstream>
#include "./Table.h"
#include "./parser.h"

class SQL
{
public:
    SQL();
    void run();
    void run_command(char* command);
    void run_batch(char* command);

};

SQL::SQL()
{

}

void SQL::run()
{
    char command[256];
    char batch[] = "batch";
    bool done = false;

    std::cout << "-------- SQL ---------\n\n" << std::endl;

    while (!done)
    {
        std::cout << "> ";
        std::cin.getline(command, 256);

        if (std::strncmp(command, "q", 1) == 0)
            done = true;
        else if (std::strcmp(command, batch) == 0)
            run_batch(command);
        else
            run_command(command);
    }
}

void SQL::run_command(char* command)
{
    Parser p(command);
    if (!p.valid())
        std::cout << "Invalid command" << std::endl;
    else
    {
        if (p.parse_tree()["command"][0] == "make" || 
                p.parse_tree()["command"][0] == "create")
        {
            Table t(p.parse_tree()["table"][0], p.parse_tree()["fields"]);
        }
        else if(p.parse_tree()["command"][0] == "insert")
        {
            Table t(p.parse_tree()["table"][0]);
            t.insert_into(p.parse_tree()["fields"]);
        }
        else
        {
            Table t(p.parse_tree()["table"][0]);
            std::cout << t.select(p.parse_tree()["fields"], p.parse_tree()["conditions"]) << std::endl;
        }
    }
}

void SQL::run_batch(char* command)
{
    std::cout << "Running batch" << std::endl;
    std::ifstream file;
    char line[256];
    int counter;
    counter = 0;

    file.open("./batch/batch1.txt");

    if (file.fail())
        std::cout << "failed to open file." << std::endl;
    else
    {
        file.getline(line, 255);
        while (!file.eof())
        {
            if (line[0] == '/' || strlen(line) == 0)
                std::cout << line << std::endl << std::endl;
            else
            {
                std::cout << "[" << counter++ << "]  " << line << std::endl << std::endl;    
                run_command(line);                
            }
            file.getline(line, 256);
        }
    }
} 
#endif