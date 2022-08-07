#include "other.hpp"

std::pair<int8_t, int8_t> getRealIndices(int8_t row, int8_t col)
{
    row %= 4;
    col %= 4;

    if (row < 0)
        row = 4 - row;
    
    if (col < 0)
        col = 4 - col;

    return std::make_pair(row, col);
}

int8_t translateIndices(int8_t row, int8_t col)
{
    row %= 4;
    col %= 4;
    return 4*row + col;
}

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

int getRealIndex(int startIndex)
{
    int col = startIndex % 4;
    int row = startIndex / 4;
    return (col + 1) % 4 + row*4;
}
