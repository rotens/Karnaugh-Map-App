#pragma once
#include <array>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>
#include "other.hpp"

class Map1x2 
{
    int zeroes{2};
    int ones{0};
    std::array<Value, 2> kmap;
    std::string algebraicMinterm{"0"};

public:
    void findAlgebraicMinterm();
    std::string& getAlgebraicMinterm();
    bool changeValue(int, Value);
};
