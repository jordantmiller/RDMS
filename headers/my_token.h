/*********************************************************
 *   AUTHOR        : Jordan Miller
 *   Assignment    : FTokenizer
 *   CLASS         : CS 008
 *   SECTION       : MTWR 4:00p
 *   Due Date      : 09/22/2019
 ********************************************************
FILE: my_token.h

CLASS PROVIDED: Token
    A class to store a string token.

VALUE SEMANTICS:
    The Copy constructor and assignment operator may be used with
    this class. 

FUNCTIONS PROVIDED:
    int type() const
        Precondition: A Token object has been initialized
        Postcondition: The integer indicating the type of Token has
                       been returned.

    std::string type_string() const
        Precondition: A Token object has been initialized
        Postcondition: The string representing the type of token has
                       been returned.

    std::string token_str() const
        Precondition: A Token object has been initialized
        Postcondition: The string token has been returned.
    


*/
#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <iomanip>


class Token
{
public:

    // CONSTRUCTORS
    Token();
    Token(std::string str, int type);

    // ACCESSORS
    friend std::ostream& operator <<(std::ostream& outs, const Token& t);
    int type() const {return _type;}
    std::string type_string() const;
    std::string token_str() const {return _token;}

private:

    // MEMBER VARIABLES
    std::string _token;
    int _type;
};

Token::Token()
{
    _token = "";
    _type = -1;
}

Token::Token(std::string str, int type)
{
    _token = str;
    _type = type;
}

std::ostream& operator <<(std::ostream& outs, const Token& t)
{
    switch(t.token_str()[0])
    {
    case '\t':
        outs << "|\\t|";
        break;
    case '\n':
        outs << "|\\n|";
        break;
    case '\b':
        outs << "|\\b|";
        break;
    default:
        outs << "|" << t.token_str() << "|";
        break;

    }
    return outs;
}

std::string Token::type_string() const
{
    switch(_type)
    {
    default:
        return "UNKNOWN";
    case 0:
        return "ALPHA";
    case 1:
        return "NUMBER";
    case 2:
        return "PUNCT";
    case 3:
        return "WHITE";
    }
}

#endif