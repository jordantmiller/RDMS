/*********************************************************
 *   AUTHOR        : Jordan Miller
 *   Assignment    : BPTree class
 *   CLASS         : CS 008
 *   SECTION       : MTWR 4:00p
 *   Due Date      : 11/17/2019
 ********************************************************/

#include <iostream>
#include <cstdlib>
#include "./headers/BPTree.h"

int main(void)
{
    BPTree<int> tree;
    BPTree<int> assign;

    typename BPTree<int>::Iterator iter;
    typename BPTree<int>::Iterator upper;
    typename BPTree<int>::Iterator lower;

    std::cout << "\n\n\nINSERTION TEST:" << std::endl;
    std::cout << "============================================\n"
              << "============================================\n\n";

    for (int i = 1; i < 20; i++)
    {
        tree.insert(rand() % 50 + 1);
        std::cout << "Inserting: " << std::endl << "--------------------------\n\n";
        std::cout << tree;
        std::cout << "\n\n";

        for (iter = tree.begin(); iter != tree.end(); iter++)
            std::cout << *iter << " ";

        std::cout << "\n\n";
        std::cout << std::endl;
    }



    std::cout << "\n\n" << tree << std::endl;
    BPTree<int> copy(tree);
    assign = tree;

    std::cout << "\n\n\nCOPY CONSTRUCTOR / ASSIGNEMTN OPERATOR TEST:" << std::endl;
    std::cout << "============================================\n"
              << "============================================\n\n";

    std::cout << "\n\nOriginal:\n---------------------\n\n" << tree << std::endl;

    for (iter = tree.begin(); iter != tree.end(); iter++)
    {
        std::cout << *iter << " ";
        std::cout << "\n\n";
        std::cout << std::endl;
    }

    std::cout << "Copy:\n---------------------\n\n" << copy << std::endl;

    for (iter = copy.begin(); iter != copy.end(); iter++)
    {
        std::cout << *iter << " ";
        std::cout << "\n\n";
        std::cout << std::endl;
    }

    std::cout << "Assigned:\n---------------------\n\n" << assign << std::endl;
    for (iter = assign.begin(); iter != assign.end(); iter++)
    {
        std::cout << *iter << " ";
        std::cout << "\n\n";
        std::cout << std::endl;
    }

    // std::cout << "\n\n\nGET / FIND / CONTAINS TEST:" << std::endl;
    // std::cout << "============================================\n"
    //           << "============================================\n\n";



    // for (int i = 1; i < 50; i++)
    //     std::cout << "get( " << i << " ): " << tree.get(i) << std::endl;

    // for (int i = 1; i < 50; i++)
    //     std::cout << "find( " << i << " ): " << *tree.find(i) << std::endl;

    // for (int i = 1; i < 20; i++)
    //     std::cout << "contains( " << i << " ): " << tree.contains(i) << std::endl;



    std::cout << "\n\n\nITERATOR / LINK TEST:" << std::endl;
    std::cout << "============================================\n"
              << "============================================\n\n";
    std::cout << "Keys Iterated: \n";
    for (iter = tree.begin(); iter != tree.end(); iter++)
    {
        std::cout << *iter << " "; // std::endl; //" it->data_count: " << iter.it->data_count << std::endl;
    }
    std::cout << std::endl;



    for (int i = 1; i < 50; i++)
    {
        upper = tree.upper_bound(i);

        std::cout << "upper_bound( " << i << " )";
        if (upper != NULL)
            std::cout << *upper << std::endl;
        else
        {
            std::cout << "NULL" << std::endl;
        }
    
    }

    for (int i = 1; i < 50; i++)
    {
        lower = tree.lower_bound(i);

        std::cout << "lower_bound( " << i << " )";
        if (lower != NULL)
            std::cout << *lower << std::endl;
        else
        {
            std::cout << "NULL" << std::endl;
        }
    
    }
        
        
        // std::cout << "\nlower_bound( " << i << " )" << *tree.lower_bound(i) << std::endl;
    

    // int high;
    // int low;

    // low = 8;
    // high = 37;
    // std::cout << "\n\n\nat() TEST:" << std::endl;
    // std::cout << "============================================\n"
    //           << "============================================\n\n";

    // std::cout << "Keys in range [" << low << ", " << high << "): ";
    // for (it = tree.at(low); it != tree.at(high); it++)
    //     std::cout << *it << " ";
    
    std::cout << "\n\nSUCCESS\n\n";
}