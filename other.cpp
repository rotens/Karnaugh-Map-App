#include "other.hpp"
#include <algorithm>
#include <cmath>

int getCellIndex(int cellIndex, int rowOffset, int colOffset)
{
    int col = (cellIndex % 4 + colOffset) % 4;
    int row = (cellIndex / 4 + rowOffset) % 4;

    if (row < 0)
        row = 4 + row;
    
    if (col < 0)
        col = 4 + col;

    return col + row*4;
}

int getCellIndex_Map2x4(int cellIndex, int rowOffset, int colOffset)
{
    int col = (cellIndex % 4 + colOffset) % 4;
    int row = (cellIndex / 4 + rowOffset) % 2;

    if (row < 0)
        row = 2 + row;
    
    if (col < 0)
        col = 4 + col;

    return col + row*4;
}

int getRealIndex(int startIndex)
{
    int col = startIndex % 4;
    int row = startIndex / 4;
    return (col + 1) % 4 + row*4;
}

int getRealIndex_Map2x4(int cellIndex, int offset)
{
    if (cellIndex < 3)
        return cellIndex + offset;

    if (offset == 1)
        return 0;

    if (offset == 5)
        return 4;

    return 7;
}

bool quadsEqual(const std::vector<int>& quad1, const std::vector<int>& quad2)
{
    return std::equal(quad1.begin(), quad1.end(), quad2.begin());
}

std::vector<int>& getSortedQuad(std::vector<int>& unsortedQuad)
{
    std::sort(unsortedQuad.begin(), unsortedQuad.end());

    for (auto it = properlySortedQuadsIndicesMap.begin();
         it != properlySortedQuadsIndicesMap.end();
         it += 2)
    {
        if (quadsEqual(unsortedQuad, *it))
        {
            return *(++it);
        }
    }

    return unsortedQuad;
}

std::pair<int, int> getSortedPair(const std::vector<int>& pair)
{
    int difference = abs(pair[0] - pair[1]);

    if (difference == 1 or difference == 4)
    {
        return std::make_pair(
            *std::min_element(pair.begin(), pair.end()),
            *std::max_element(pair.begin(), pair.end()));
    }
    else
    {
        return std::make_pair(
            *std::max_element(pair.begin(), pair.end()), 
            *std::min_element(pair.begin(), pair.end()));
    }
}
