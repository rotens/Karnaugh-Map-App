#pragma once
#include <vector>
#include "other.hpp"

class Map1x2 
{
    int ones{0};
    std::vector<Value> kmap{2, Value::zero};
    std::string algebraicMinterm{"0"};
    std::vector<int> singleGroups;

public:
    void findAlgebraicMinterm();
    std::string& getAlgebraicMinterm() { return algebraicMinterm; }
    std::vector<int>& getSingleGroups() { return singleGroups; }
    bool changeValue(int, Value);
    void reset();
};
