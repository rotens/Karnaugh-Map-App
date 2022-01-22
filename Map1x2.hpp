#pragma once
#include <array>
#include "other.hpp"

class Map1x2 
{
    int zeroes{2};
    std::array<Value, 2> kmap;
    std::string algebraicMinterm{"0"};

public:
    void findAlgebraicMinterm();
    std::string& getAlgebraicMinterm();
    bool changeValue(int, Value);
};
