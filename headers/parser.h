/*********************************************************
 *   AUTHOR        : Jordan Miller
 *   Assignment    : Parser Class
 *   CLASS         : CS 008
 *   SECTION       : MTWR 4:00p
 *   Due Date      : 11/17/2019
 ********************************************************
 The Parser class takes as input an SQL command, determines
 its validity, and provides a tree which breaks the command
 into its separate parts.
 
 */

#ifndef PARSER_H
#define PARSER_H

#define PROWS 100
#define PCOLS 128

#include <iostream>
#include <cstdlib>
#include "./map.h"
#include "./mmap.h"
#include "./queue.h"
#include "./my_stokenizer.h"
#include "./state_table.h"
#include "./my_token.h"

int STokenizer::_table[ROWS][COLS];

class Parser
{
public:
    Parser(char* s);

    MMap<std::string, std::string> parse_tree();
    bool get_parse_tree();
    void init_ptree();
    int get_column(std::string key);
    void make_p_table();
    void build_keyword_map();
    void tokenize_string();
    void print_ptree();
    bool valid() { return fail_flag; }


private:
    MMap<std::string, std::string> ptree;
    Map<std::string, int> keywords_map;
    STokenizer stk;
    int adj_table[PROWS][PCOLS];
    bool fail_flag;
    Queue<std::string> input_queue;
    enum keywords { ZERO, 
                    CREATE, 
                    MAKE, 
                    SELECT,
                    STAR,
                    COMMA,
                    QUOTE, 
                    INSERT, 
                    FROM,
                    INTO, 
                    WHERE, 
                    LOGICAL, 
                    RELATIONAL, 
                    SYMBOL,
                    VALUES,
                    FIELDS,
                    TABLE };
};

Parser::Parser(char* s)
{
    stk.set_string(s);
    make_p_table();
    build_keyword_map();
    tokenize_string();
    init_ptree();
    fail_flag = get_parse_tree();

}

bool Parser::get_parse_tree()
{
    std::string string;
    int state;
    state = 0;

    while(!input_queue.is_empty())
    {
        string = input_queue.pop();
        state = adj_table[state][get_column(string)];

        switch (state)
        {
        case -1:
            std::cout << "State = -1. Something went wrong in the adj_table." << std::endl;
            break;
        case 1:
        case 20:
        case 30:
            ptree["command"] += string;
            break;
        case 2:
        case 11:
        case 24:
        case 34:
            ptree["fields"] += string;
            break;
        case 3:
        case 4:
        case 21:
        case 23:
        case 25:
        case 31:
        case 33:
        case 35:
            break;
        case  5:
        case 22:
        case 32:
            ptree["table"] += string;
            break;
        case 6:
            ptree["where"] += "true";
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            ptree["conditions"] += string;
            break;
        }
    }

    if (adj_table[state][0] > 0)
        return true;
    
    return false;
}

void Parser::init_ptree()
{
    std::string strs[10] = { "command",
                             "table",
                             "fields",
                             "where",
                             "conditions" };

    for (int i = 0; i < 5; i++)
        ptree.create_key(strs[i]);
}

void Parser::print_ptree()
{
    std::cout << "\n" << ptree << std::endl;
}

int Parser::get_column(std::string key)
{
    if (keywords_map.contains(key))
        return keywords_map[key];
    else
        return SYMBOL;
}

void Parser::tokenize_string()
{
    Token t;
    std::string str;

    while(stk.more())
    {
        stk >> t;
        if (t.token_str() == "\"")
        {
            t = Token();
            str = "";
            stk >> t;
            while (t.token_str() != "\"")
            {
                str += t.token_str();
                t = Token();
                stk >> t;
            }
            input_queue.push(str);
        }
        else if (t.token_str() == ">" || t.token_str() == "<")
        {
            str = t.token_str();
            t = Token();
            stk >> t;

            if (t.token_str() == "=")
                str += t.token_str();

            input_queue.push(str);
            
            if (t.type_string() != "WHITE" && t.token_str() != "=")
                input_queue.push(t.token_str());

            str = "";
        }
        else if (t.type_string() != "WHITE")
            input_queue.push(t.token_str());

        t = Token();
    }
}
void Parser::make_p_table()
{
    table_init(adj_table);

    // SELECT MACHINE
    adj_table[0][SELECT] = 1;
    adj_table[1][SYMBOL] = 2;
    adj_table[1][STAR] = 11;
    adj_table[11][FROM] = 4;
    adj_table[2][COMMA] = 3;
    adj_table[3][SYMBOL] = 2;
    adj_table[2][FROM] = 4;
    adj_table[4][SYMBOL] = 5;
    adj_table[5][ZERO] = 1;  // Success state
    adj_table[5][WHERE] = 6;
    adj_table[6][SYMBOL] = 7;
    adj_table[7][RELATIONAL] = 8;
    adj_table[8][SYMBOL] = 9;
    adj_table[9][ZERO] = 1; // Success state
    adj_table[9][LOGICAL] = 10;
    adj_table[10][SYMBOL] = 7;

    // MAKE / CREATE MACHINE
    adj_table[0][CREATE] = 20;
    adj_table[20][TABLE] = 21;
    adj_table[21][SYMBOL] = 22;
    adj_table[22][FIELDS] = 23;
    adj_table[23][SYMBOL] = 24;
    adj_table[24][ZERO] = 1; // success state
    adj_table[24][COMMA] = 25;
    adj_table[25][SYMBOL] = 24;

    // INSERT MACHINE
    adj_table[0][INSERT] = 30;
    adj_table[30][INTO] = 31;
    adj_table[31][SYMBOL] = 32;
    adj_table[32][VALUES] = 33;
    adj_table[33][SYMBOL] = 34;
    adj_table[34][ZERO] = 1; // success state
    adj_table[34][COMMA] = 35;
    adj_table[35][SYMBOL] = 34;
}

void Parser::build_keyword_map()
{
    std::string words[20] = { "create", 
                              "make", 
                              "select", 
                              "insert", 
                              "from", 
                              "where",
                              "into",
                              "and",
                              "or",
                              "=",
                              "<",
                              ">",
                              "<=",
                              ">=",
                              "\"",
                              ",",
                              "*",
                              "values",
                              "fields",
                              "table" };

    for (int i = 0; i < 20; i++)
        keywords_map.create_key(words[i]);

    keywords_map[words[0]] = CREATE;
    keywords_map[words[1]] = CREATE;
    keywords_map[words[2]] = SELECT;
    keywords_map[words[3]] = INSERT;
    keywords_map[words[4]] = FROM;
    keywords_map[words[5]] = WHERE;
    keywords_map[words[6]] = INTO;
    keywords_map[words[7]] = LOGICAL;
    keywords_map[words[8]] = LOGICAL;
    keywords_map[words[9]] = RELATIONAL;
    keywords_map[words[10]] = RELATIONAL;
    keywords_map[words[11]] = RELATIONAL;
    keywords_map[words[12]] = RELATIONAL;
    keywords_map[words[13]] = RELATIONAL;
    keywords_map[words[14]] = QUOTE;
    keywords_map[words[15]] = COMMA;
    keywords_map[words[16]] = STAR;
    keywords_map[words[17]] = VALUES;
    keywords_map[words[18]] = FIELDS;
    keywords_map[words[19]] = TABLE;

}

MMap<std::string, std::string> Parser::parse_tree()
{
    return this->ptree;
}

#endif