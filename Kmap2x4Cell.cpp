#include <algorithm>
#include "Kmap2x4Cell.hpp"
#include "Map2x4.hpp"

Kmap2x4Cell::Kmap2x4Cell(Map2x4& kmapObject, int cellIndex)
        : kmapObject(kmapObject), cellIndex(cellIndex) {}

void Kmap2x4Cell::findPairs()
{
    int rowOffset = this->cellIndex < 4 ? 1 : -1;

    for (const auto offset : {-1, 1})
    {
        int neighbourCellIndex = getCellIndex_Map2x4(this->cellIndex, 0, offset);
        Value neighbourCellValue = kmapObject.getCellValue(neighbourCellIndex);

        if (neighbourCellValue == Value::one)
        {
            pairs.push_back(neighbourCellIndex);
            ++this->pairsNumber;
        }
    }

    int neighbourCellIndex = getCellIndex_Map2x4(this->cellIndex, rowOffset, 0);
    Value neighbourCellValue = kmapObject.getCellValue(neighbourCellIndex); 

    if (neighbourCellValue == Value::one)
    {
        pairs.push_back(neighbourCellIndex);
        ++this->pairsNumber;
    }
}

void Kmap2x4Cell::removePairContainingGivenCellIndex(int cellIndex)
{
    const auto pairsLength = pairs.size();
    auto newEnd = std::remove(pairs.begin(), pairs.end(), cellIndex);
    pairs.erase(newEnd, pairs.end());
    
    if (pairs.size() != pairsLength)
    {
        --this->pairsNumber;
    }
}

void Kmap2x4Cell::reset()
{
    done = false;
    pairsNumber = 0;
    pairs.clear();
}
