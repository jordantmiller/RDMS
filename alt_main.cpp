/*********************************************************
 *   AUTHOR        : Jordan Miller
 *   Assignment    : Final Project
 *   CLASS         : CS 008
 *   SECTION       : MTWR 4:00p
 *   Due Date      : 12/06/2019
 ********************************************************

 */

#include "./headers/Table.h"
#include "./headers/parser.h"

int main(void)
{   
    std::string fields_arr[10] = {"fname", "lname", "dept."};
    std::string entry_arr[10] = {"10", "Doe", "CS"};
    std::string entry2_arr[10] = {"20", "Miller", "Math"};
    std::string entry3_arr[10] = {"30", "Harrison", "English"};
    std::string entry4_arr[10] = {"40", "Danson", "Geology"};
    std::string cols[10] = {"lname", "dept."};
    std::string rows[20] = {"dept.", "=", "Math", "or", "lname", "=", "Danson", "and", "dept.", "=", "English" };

    Vectorstr colv;
    Vectorstr rowv;
    Vectorstr fields;
    Vectorstr entry1, entry2, entry3, entry4;

    std::string name = "test";

    for (int i = 0; i < 3; i++)
    {
        fields.push_back(fields_arr[i]);
        entry1.push_back(entry_arr[i]);
        entry2.push_back(entry2_arr[i]);
        entry3.push_back(entry3_arr[i]);
        entry4.push_back(entry4_arr[i]);
    }

    std::cout << "Constructing table" << std::endl;

    Table t(name, fields);

    std::cout << "Constructed Table." << std::endl;
    t.insert_into(entry1);
    t.insert_into(entry2);
    t.insert_into(entry3);
    t.insert_into(entry4);

    for (int i = 0; i < 2; i++)
        colv.push_back(cols[i]);

    for (int i = 0; i < 11; i++)
        rowv.push_back(rows[i]);



    std::cout << "select_all()\n===============================\n\n";
    std::cout << "\n\n\n" << t.select_all() << std::endl;

    std::cout << "\n\n\nselect()\n===============================\n\n";
    std::cout << t.select(colv, rowv) << std::endl;


    std::cout << "SUCCESS" << std::endl;
}