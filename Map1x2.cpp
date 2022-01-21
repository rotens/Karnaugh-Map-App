#include "Map1x2.hpp"

std::string& Map1x2::getAlgebraicMinterm()
{
    return algebraicMinterm;
}

void Map1x2::findAlgebraicMinterm()
{
    if (zeroes == 2)
    {
        algebraicMinterm = "0";
        return;
    }

    if (ones == 2)
    {
        algebraicMinterm = "1";
        return;
    }

    if (kmap[0] == Value::one)
    {
        algebraicMinterm = "A\'";
        return;
    }

    algebraicMinterm = "A";
}

bool Map1x2::changeValue(int index, Value value)
{
    if (index < 0 or index > 1)
        return false;

    if (kmap[index] == value)
        return true;

    kmap[index] = value;

    if (value == Value::one)
    {
        ++ones;
        --zeroes;
        return true;
    }

    ++zeroes;
    --ones;

    return true;
}
