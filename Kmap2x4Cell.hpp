#pragma once
#include <iostream>
#include <numeric>
#include <set>
#include <vector>
#include "other.hpp"

class Map2x4;
class MapTest;

class Kmap2x4Cell
{
public:
    Kmap2x4Cell(Map2x4& kmapObject, int cellIndex);
    void setCellValue(Value cellValue) { this->cellValue = cellValue; }
    Value getCellValue() { return this->cellValue; }
    void setDone() { done = true; }
    bool isDone() { return this->done; }
    int getPairsNumber() { return this->pairsNumber; }
    int getIndex() { return this->cellIndex; }
    std::vector<int>& getPairs() { return this->pairs; }
    void findPairs();
    void removePairContainingGivenCellIndex(int);
    bool hasPairs() { return not pairs.empty(); }

    friend class MapTest;

private:
    int cellIndex;
    Map2x4& kmapObject;
    Value cellValue{Value::zero};
    bool done{false};
    int pairsNumber{0};
    std::vector<int> pairs;
};
