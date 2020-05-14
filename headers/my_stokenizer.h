/*********************************************************
 *   AUTHOR        : Jordan Miller
 *   Assignment    : FTokenizer
 *   CLASS         : CS 008
 *   SECTION       : MTWR 4:00p
 *   Due Date      : 09/22/2019
 ********************************************************
FILE: my_token.h

CLASS PROVIDED: STokenizer
    A class to store a string token.

VALUE SEMANTICS:
    The Copy constructor and assignment operator may be used with
    this class. 

FUNCTIONS PROVIDED:
    bool done()
        Precondition:   Existing STokenizer object.
        Postcondition:  A bool indicating if there is string left to tokenize
                        True if the string has been tokenized completely

    bool more()
        Precondition:   Existing STokenizer object.
        Postcondition:  A bool indicating if there is string left to tokenize
                        True if the string has not been tokenized completely

    STokenizer& operator >> (STokenizer& stk, Token& t)
        Precondition:   An STokenizer object has been initialized and has a 
                        string to tokenize.  A Token object exists which is
                        prepared to store the token generated by the STokenizer.
        Postcondition:  A token has been determined and placed into the Token object t.

    void set_string(char str[])
        Precondition:   Existing STokenizer object.
        Postcondition:  The string str[] has been placed in the STokenizer's 
                        _buffer[] variable.

    void make_table(int table[][COLS])
        Precondition:   None
        Postcondition:  The static member variable _table has been initialized
                        to work as a state machine for tokenizing strings into
                        words, numbers, punctuation marks, whitespace, and 
                        unknown characters.

    int get_token(std::string& token)
        Precondition:   An STokenizer object exists and has a string stored in 
                        it's _buffer.
        Postcondition:  A token has been generated along with its type 
                        (ALPHA, DIGIT, etc.).

*/


#ifndef STOKENIZER_H
#define STOKENIZER_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <assert.h>
#include "./my_token.h"
#include "./state_table.h"

#define MAX_BUFF 100

class STokenizer
{
public:

    // CONSTRUCTORS
    STokenizer();
    STokenizer(char str[]);

    // ACCESSORS
    bool done() {return (_pos >= strlen(_buffer));}            
    bool more() {return (_pos < strlen(_buffer));}           
    friend STokenizer& operator >> (STokenizer& stk, Token& t);

    // MUTATORS
    void set_string(char str[]);

private:

    // PRIVATE HELPER FUNCTIONS
    void make_table(int table[][COLS]);
    int get_token(std::string& token);

    // MEMBER VARIABLES
    char _buffer[MAX_BUFF];       
    int _pos;                       
    static int _table[ROWS][COLS];
};

STokenizer::STokenizer()
{
    make_table(STokenizer::_table);
    _pos = 0;
}

STokenizer::STokenizer(char str[])
{
    make_table(STokenizer::_table);
    _pos = 0;
    strcpy(_buffer, str);

}

void STokenizer::set_string(char str[])
{
    strcpy(_buffer, str);
    _pos = 0;
}

void STokenizer::make_table(int table[][COLS])
{
    _make_table(table);
}

int STokenizer::get_token(std::string& token)
{
    token = "";
    int state;
    int start_pos = _pos;

    state = 0;

    // If block to handle unknown characters
    // Instead of misprinting several unknown characters
    // I elected to have _pos increment until it encounters
    // a known character
    if (_buffer[_pos] > 127 || _buffer[_pos] < 0)
    {
        token = "????";
        while (_buffer[_pos] < 0 || _buffer[_pos] > 127)
            _pos++;
        return -1;
    }
    
    // Main loop which naviagates the state table to
    // determine the position of the last character in 
    // a valid string.  Breaks if a character of a different
    // token type is found or if an unknown character is found.
    for (int i = _pos; i < strlen(_buffer); i++)
    {
        if (_buffer[i] > 127 || _buffer[i] < 0)
            break;
        else if (_table[state][_buffer[i]] < 0)
            break;
        else
        {
            state = _table[state][_buffer[i]];
            if (_table[state][0] == 1)
                _pos = i;
        }
    }

    // Variable needed for use with strncpy to get a substring of 
    // _buffer
    char c_str_copy[MAX_BUFF];

    // Puts the correct substring of _buffer into c_str_copy
    strncpy(c_str_copy, (_buffer + start_pos), (_pos - start_pos) + 1);

    // Adds the null terminating character
    c_str_copy[(_pos - start_pos) + 1] = '\0';

    // Uses the std::string assignment operator with a c-string
    token = c_str_copy;
    
    // Increments _pos to check the next character when / if 
    // the function is called again
    _pos++;

    // If state doesn't move from 0 then no token was found and 
    // a negative number is returned indicating an UNKNOWN type
    // else an integer is returned indicating the type of token
    // found
    if (state == 0)
        return -1;
    else
        // Uses a constant defined in state_table.h to determine type.
        // Can be done since all machines in the state table begin on
        // sequential multiples of MACHINE_SEPARATER
        return state / MACHINE_SEPARATER;

}

STokenizer& operator >> (STokenizer& stk, Token& t)
{
    std::string token;
    int type;

    // Gets the token and puts it in the token string variable
    // and sets type to the int representing the type of variable
    type = stk.get_token(token);

    // Assigns the Token t to be a Token object based on the token
    // and type generated by the previous statement.
    t = Token(token, type);

    return stk;
}




#endif