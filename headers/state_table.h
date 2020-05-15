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
FILE: my_token.h
    Provides functions used for making a state table for an STokenizer
    object.  No general use otherwise.

 */


#ifndef STATE_TABLE_H
#define STATE_TABLE_H

#define ROWS 32
#define COLS 128
#define MACHINE_START 0
#define WORD_MACHINE 1
#define NUMBER_MACHINE 5
#define PUNCT_MACHINE 10
#define WHITE_MACHINE 15
#define MACHINE_SEPARATER 5

char PUNCT[] = ",./\\;:\'\"!@#$%^&*()+=-_*?><[]{}|`~";
char WHITE[] = " \t\n\b";

void mark_range(int table[][COLS], int row, char start, char end, int mark);
void mark_success(int table[][COLS], int row);
void mark_fail(int table[][COLS], int row);
void table_init(int table[][COLS]);
void mark_cstr(int table[][COLS], char string[], int row, int mark);
void mark_char(int table[][COLS], char character, int row, int mark);

void _make_table(int table[][COLS])
{
    table_init(table);
    mark_fail(table, 0);
    
    // WORDS MACHINE
    mark_range(table, MACHINE_START, 'a', 'z', WORD_MACHINE);
    mark_range(table, MACHINE_START, 'A', 'Z', WORD_MACHINE);
    mark_success(table, WORD_MACHINE);
    mark_range(table, WORD_MACHINE, 'a', 'z', WORD_MACHINE);
    mark_range(table, WORD_MACHINE, 'A', 'Z', WORD_MACHINE);

    // NUMBER MACHINE
    // Allows for integers, decimal, and comma denoted i.e. 123,456,789
    // or both (123,456,789.123).  Doesn't allow for invalid numbers such as
    // 123.123.123 or 123.123,123.
    // Not able to do negative numbers yet.
    mark_range(table, MACHINE_START, '0', '9', NUMBER_MACHINE);
    mark_success(table, NUMBER_MACHINE);
    mark_range(table, NUMBER_MACHINE, '0', '9', NUMBER_MACHINE);
    mark_char(table, ',', NUMBER_MACHINE, NUMBER_MACHINE + 1);
    mark_fail(table, NUMBER_MACHINE + 1);
    mark_range(table, NUMBER_MACHINE + 1, '0', '9', NUMBER_MACHINE);
    mark_char(table, '.', NUMBER_MACHINE, NUMBER_MACHINE + 2);
    mark_fail(table, NUMBER_MACHINE + 2);
    mark_range(table, NUMBER_MACHINE + 2, '0', '9', NUMBER_MACHINE + 3);
    mark_range(table, NUMBER_MACHINE + 3, '0', '9', NUMBER_MACHINE + 3);
    mark_success(table, NUMBER_MACHINE + 3);

    // PUNCUATION MACHINE
    // Takes each occurance of a punctuation character as its own token
    mark_cstr(table, PUNCT, MACHINE_START, PUNCT_MACHINE);
    mark_success(table, PUNCT_MACHINE);

    // WHITESPACE MACHINE
    // each occurance of a whitespace character will be taken as its own
    // token
    mark_cstr(table, WHITE, MACHINE_START, WHITE_MACHINE);
    mark_success(table, WHITE_MACHINE);



}

void mark_range(int table[][COLS], int row, char start, char end, int mark)
{
    for (char i = start; i <= end; i++)
    {
        table[row][i] = mark;
    }
}

void mark_success(int table[][COLS], int row)
{
    table[row][0] = 1;
}

void mark_fail(int table[][COLS], int row)
{
    table[row][0] = 0;
}

void table_init(int table[][COLS])
{
    for (int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS; j++)
        {
            table[i][j] = -1;
        }
}

void mark_cstr(int table[][COLS], char string[], int row, int mark)
{
    for (int i = 0; string[i] != '\0'; i++)
        table[row][string[i]] = mark;
}

void mark_char(int table[][COLS], char character, int row, int mark)
{
    table[row][character] = mark;
}



#endif
