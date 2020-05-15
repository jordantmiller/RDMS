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
FILE: my_ftokenizer.h

CLASS PROVIDED: FTokenizer
    A class to tokenize a text file using a string tokenizer

VALUE SEMANTICS:
    The Copy constructor and assignment operator may be used with
    this class. 

FUNCTIONS PROVIDED:

    bool more()
        Precondition: A FTokenizer has been initialized with a file name
        Postcondition: A bool has been returned indicating if there
                       is more of the file to tokenize.

    int pos()
        Precondition: A FTokenizer has been initialized with a file name
        Postcondition: The position within the file indicating the point
                       up to which has been tokenized.

    int block_pos()
        Precondition: A FTokenizer has been initialzed with a file name
        Postcondition: The position in the current block up to which it
                       has already been tokenized.

    friend FTokenizer& operator >> (FTokenizer& ftk, Token& t)
        Precondition: A FTokenizer and a Token object have been initialized
        Postcondition: The next string token has been placed inside of the Token t.

*/

#ifndef FTOKENIZER_H
#define FOKENIZER_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <assert.h>

#include "./my_stokenizer.h"
#include "./my_token.h"
#include "./state_table.h"

class FTokenizer
{
public:
    // STATIC MEMBER CONSTANT
    static const int MAX_BLOCK = MAX_BUFF;

    // CONSTRUCTORS
    FTokenizer(const char* fname);

    // ACCESSORS
    bool more() {return (_more || _stk.more());}        
    int pos() {return _pos;}          
    int block_pos() {return _blockPos;}     
    friend FTokenizer& operator >> (FTokenizer& ftk, Token& t);
private:

    // PRIVATE HELPER FUNCTIONS
    Token next_token();
    bool get_new_block(); 

    // PRIVATE MEMBER VARIABLES
    std::ifstream _f;  
    STokenizer _stk;   
    int _pos;          
    int _blockPos;      
    bool _more;         

};

FTokenizer::FTokenizer(const char* fname)
{
    _f.open(fname, std::ios::in);

    if (!_f)
        std::cout << "Failed to open file " << fname << std::endl;
    else
    {
        // Initializes member variables and 
        // gets the first block of the file.
        _pos = _blockPos = 0;
        _more = true;
        get_new_block();
    }    
}

bool FTokenizer::get_new_block()
{
    char string[MAX_BLOCK];
    int extracted = 0;

    // Gets a block of chars from the file and
    // uses std::ifstream::gcount() to get the number of
    // chars extracted.  Needed for the last block which
    // is likely to be less than MAX_BLOCK - 1.
    extracted = _f.read(string, MAX_BLOCK - 1).gcount();
    string[extracted] = '\0';

    // Sets the new block stored in string as the string for
    // the STokenizer to tokenize.
    _stk.set_string(string);

    // Increments the _pos variable by the number of chars
    // read from the file.  
    _pos += extracted;

    // Sets the _blockpos to 0 for the new block just read
    _blockPos = 0;

    //  If the end of the file is reached
    //  or if it fails to read.  _more is set to false
    //  so the FTokenizer will hault after this block is 
    //  tokenized.
    if (_f.fail() || _f.eof())
        _more = false;
}

Token FTokenizer::next_token()
{
    Token t;

    // See documentation in STokenizer.h but
    // is used to get a new token with the member
    // STokenizer _stk.
    _stk >> t;

    return t;
}

FTokenizer& operator >> (FTokenizer& ftk, Token& t)
{
    // IF the string tokenizer has tokenized its whole
    // string AND there remains more of the file to tokenize.
    if (ftk._stk.done() && ftk.more())
    {
        // Gets new block and resets _blockPos to 0.
        ftk.get_new_block();
        ftk._blockPos = 0;

    }

    // Gets a new token and increments _blockPos by the length
    // of that token.
    t = ftk.next_token();
    ftk._blockPos += t.token_str().length();
    

    return ftk;
}






#endif