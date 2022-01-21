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