#include <iostream>
#include <cstdlib>
#include "./headers/parser.h"
#include "./headers/Table.h"

int main(void)
{
    char string[] = "select * from student where lname = Yang or major = CS and age < 23 or company = Google";
    Parser p(string);

    p.print_ptree();

    std::cout << "\n\nOriginal Conditions Vector: " << p.parse_tree()["conditions"] << std::endl;
    std::cout << "\nRPN Conditions: " << t.get_rpn(p.parse_tree()["conditions"]) << std::endl;

}
