#include "Map2x2.hpp"

void Map2x2::findPairs()
{
    for (const auto i : {0, 3})
    {
        for (const auto j : {1, 2})
        {
            if (isPair(i, j))
            {
                addPair(i, j);
            }
        }
    }
}

bool Map2x2::isPair(int firstCell, int secondCell)
{
    if (kmap[firstCell] != Value::one)
        return false;

    if (kmap[secondCell] != Value::one)
        return false;

    return true;
}

void Map2x2::addPair(int firstCell, int secondCell)
{
    std::vector<int> newPair{firstCell, secondCell};
    pairs.push_back(std::move(newPair));
}

void Map2x2::findSingleGroups()
{
    for (const auto cellIndex : {0, 1, 2, 3})
    {
        if (kmap[cellIndex] == Value::one)
        {
            singleGroups.push_back(cellIndex);
        }
    }  
}

void Map2x2::findGroups()
{
    pairs.clear();
    singleGroups.clear();

    findPairs();
    
    if (not pairs.empty())
        return;

    findSingleGroups();
}

void Map2x2::initializeKmapWith(std::vector<Value>& kmap)
{
    this->kmap = kmap;
    for (const auto cellValue : kmap)
    {
        if (cellValue == Value::one)
            ++ones;
    }
}

void Map2x2::findAlgebraicMintermsForSingleGroups()
{
    std::string product = "";

    for (const auto cellIndex : singleGroups)
    {
        int row = cellIndex / 2;
        int col = cellIndex % 2;
        product += row == 0 ? "!B" : "B";
        product += col == 0 ? "!A" : "A";
        algebraicMinterms.push_back(product);
        product = "";
    }
}

void Map2x2::findAlgebraicMintermsForPairs()
{
    std::string product = "";
    int onesA = 0;
    int onesB = 0;

    for (const auto& pair : pairs)
    {
        for (const auto cellIndex : pair)
        {
            int row = cellIndex / 2;
            int col = cellIndex % 2;

            if (row == 1)
                ++onesB;

            if (col == 1)
                ++onesA;
        }

        product += onesA == 2 ? "A" : "";
        product += onesA == 0 ? "!A" : "";
        product += onesB == 2 ? "B" : "";
        product += onesB == 0 ? "!B" : "";
        algebraicMinterms.push_back(product);
        
        onesA = 0;
        onesB = 0;
        product = "";
    }
}

void Map2x2::findAlgebraicMinterms()
{
    algebraicMinterms.clear();

    if (ones == 0)
    {
        algebraicMinterms.push_back("0");
        return;
    }

    if (ones == 4)
    {
        algebraicMinterms.push_back("1");
        return;
    }
    
    findAlgebraicMintermsForPairs();
    findAlgebraicMintermsForSingleGroups();
}

bool Map2x2::changeValue(int cellIndex, Value value)
{
    if (cellIndex < 0 or cellIndex > 3)
        return false;

    if (kmap[cellIndex] == value)
        return true;

    kmap[cellIndex] = value;

    if (value == Value::one)
    {
        ++ones;
        return true;
    }

    --ones;

    return true;
}

std::vector<std::string>& Map2x2::getAlgebraicMinterms()
{
    return algebraicMinterms;
}
