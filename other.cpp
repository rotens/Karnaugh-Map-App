#include "other.hpp"

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
