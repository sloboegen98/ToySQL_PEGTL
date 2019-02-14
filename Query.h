#pragma once

#include <vector>
#include <string>

struct Predicate
{
    std::string left;
    std::string right;
    char rel;

    void print();    
};


struct Query
{
    std::vector <std::string> selattrs;
    std::vector <std::string> from;
    std::vector <Predicate> where;

    void print();
};
