#include "Query.h"

#include <iostream>

void Predicate::print(){
    std::cout << left << ' ' << rel << ' ' << right << '\n';
} 

void Query::print(){
    std::cout << "ATTRIBUTES:\n";

    for (int i = 0; i < selattrs.size(); ++i)
        std::cout << selattrs[i] << '\n';
    
    std::cout << "TABLES:\n";

    for (int i = 0; i < from.size(); ++i)
        std::cout << from[i] << '\n';

    if (where.size() != 0){
        std::cout << "PREDICATE:\n";

        for (int i = 0; i < where.size(); ++i)
            where[i].print();
    }
}