#include "Map1x2.hpp"

void Map1x2::findAlgebraicMinterm()
{
    if (ones == 2)
    {
        algebraicMinterm = "1";
        return;
    }

    if (ones == 0)
    {
        algebraicMinterm = "0";
        return;
    }

    if (kmap[0] == Value::one)
    {
        algebraicMinterm = "A\'";
        singleGroups.push_back(0);
        return;
    }

    algebraicMinterm = "A";
    singleGroups.push_back(1);
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
        return true;
    }

    --ones;

    return true;
}

void Map1x2::reset()
{
    ones = 0;
    algebraicMinterm = "";
    singleGroups.clear();
    kmap[0] = Value::zero;
    kmap[1] = Value::zero;
}
