#include "Map2x2.hpp"

void Map2x2::find2x1Groups()
{
    for (int8_t i = 0; i < 2; ++i)
    {
        if (is2x1Group(i, 0, 1, 2))
        {
            add2x1Group(i, 0, 1, 2);
        }

        if (is2x1Group(0, i, 2, 1))
        {
            add2x1Group(0, i, 2, 1);
        }
    }   
}

bool Map2x2::is2x1Group(int row, int col, int height, int width)
{
    for (int8_t i = row; i < row+height; ++i)
    {
        for (int8_t j = col; j < col+width; ++j)
        {
            if (kmap[i][j] != Value::one)
                return false;            
        }
    }

    return true;
}

void Map2x2::add2x1Group(int row, int col, int height, int width)
{
    int k = 0;
    std::array<std::pair<int, int>, 2> newGroup;

    for (int i = row; i < row+height; ++i) 
    {
        for (int j = col; j < col+width; ++j)
        {
            newGroup[k++] = std::make_pair(i, j);
        }
    }

    rect2x1Groups.push_back(std::move(newGroup));
}

void Map2x2::find1x1Groups()
{
    for (int i : {0, 1})
    {
        for (int j : {0, 1})
        {
            if (kmap[i][j] == Value::one)
                _1x1Groups.push_back(std::make_pair(i, j));
        }
    }
}

void Map2x2::findGroups()
{
    find2x1Groups();
    
    if (not rect2x1Groups.empty())
        return;

    find1x1Groups();
}

void Map2x2::initializeKmapWith(std::array<std::array<Value, 2>, 2>& kmap)
{
    this->kmap = kmap;
}

void Map2x2::findAlgebraicMinterms()
{
    std::string product = "";

    for (const auto& group : _1x1Groups)
    {
        product += group.first == 0 ? "B\'" : "B";
        product += group.second == 0 ? "A\'" : "A";
        algebraicMinterms.push_back(product);
        product = "";
    }
}

void Map2x2::findAlgebraicMintermsFor2x1Groups()
{
    std::string product = "";
    int onesA = 0;
    int onesB = 0;

    for (const auto& group : rect2x1Groups)
    {
        for (const auto& cell : group)
        {
            if (cell.first == 1)
                ++onesB;

            if (cell.second == 1)
                ++onesA;
        }

        product += onesA == 2 ? "A" : "";
        product += onesA == 0 ? "A\'" : "";
        product += onesB == 2 ? "B" : "";
        product += onesB == 0 ? "B\'" : "";
        algebraicMinterms.push_back(product);
        
        onesA = 0;
        onesB = 0;
        product = "";
    }
}

void Map2x2::findAlgebraicMinterms()
{
    algebraicMinterms.clear();
    if (zeroes == 4)
    {
        algebraicMinterms.push_back("0");
        return;
    }

    if (ones == 4)
    {
        algebraicMinterms.push_back("0");
        return;
    }
    
    findAlgebraicMintermsFor2x1Groups();
    findAlgebraicMintermsFor1x1Groups();
}

void Map2x2::solve() 
{
    findGroups();
    findAlgebraicMinterms();
}
