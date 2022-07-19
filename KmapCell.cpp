#include <algorithm>
#include "KmapCell.hpp"
#include "Map4x4.hpp"

KmapCell::KmapCell(Map4x4& kmapObject, int cellIndex)
        : kmapObject(kmapObject), cellIndex(cellIndex) {}

bool KmapCell::hasPairsOrQuads()
{
    return getPairsNumber() > 0 or getQuadsNumber() > 0;
}

void KmapCell::findPairs()
{
    for (const auto offset : {-1, 1})
    {
        int neighbourCellIndex = getCellIndex(this->cellIndex, offset, 0);
        Value cellValue = kmapObject.getCellValue(neighbourCellIndex);

        if (cellValue == Value::one)
        {
            pairs.push_back(neighbourCellIndex);
            ++this->pairsNumber;
        }

        neighbourCellIndex = getCellIndex(this->cellIndex, 0, offset);
        cellValue = kmapObject.getCellValue(neighbourCellIndex);

        if (cellValue == Value::one)
        {
            pairs.push_back(neighbourCellIndex);
            ++this->pairsNumber;
        }
    }
}

void KmapCell::findPairsWithSharing()
{
    for (const auto offset : {-1, 1})
    {
        int neighbourCellIndex = getCellIndex(this->cellIndex, offset, 0);
        Value cellValue = kmapObject.getCellValue(neighbourCellIndex);

        if (cellValue == Value::one)
        {
            pairsWithSharing.push_back(neighbourCellIndex);
            ++this->numberOfPairsWithSharing;
        }

        neighbourCellIndex = getCellIndex(this->cellIndex, 0, offset);
        cellValue = kmapObject.getCellValue(neighbourCellIndex);

        if (cellValue == Value::one)
        {
            pairsWithSharing.push_back(neighbourCellIndex);
            ++this->numberOfPairsWithSharing;
        }
    }
}

void KmapCell::findPairsWithoutSharing()
{
    for (const auto offset : {-1, 1})
    {
        int neighbourCellIndex = getCellIndex(this->cellIndex, offset, 0);
        Value cellValue = kmapObject.getCellValue(neighbourCellIndex);
        bool cellDone = kmapObject.isCellDone(neighbourCellIndex);

        if (cellValue == Value::one and not cellDone)
        {
            pairs.push_back(neighbourCellIndex);
            ++this->numberOfPairsWithSharing;
        }

        neighbourCellIndex = getCellIndex(this->cellIndex, 0, offset);
        cellValue = kmapObject.getCellValue(neighbourCellIndex);

        if (cellValue == Value::one)
        {
            pairsWithSharing.push_back(neighbourCellIndex);
            ++this->numberOfPairsWithSharing;
        }
    }
}


void KmapCell::findSquareQuads()
{
    std::vector<int> quad; 

    for (const auto& quadOffsets : quadsOffsets)
    {
        for (const auto& offsetPair : quadOffsets)
        {
            int neighbourCellIndex = getCellIndex(this->cellIndex, offsetPair.first, offsetPair.second); 
            Value neighbourCellValue = kmapObject.getCellValue(neighbourCellIndex);
            if (neighbourCellValue == Value::zero) break;
            quad.push_back(neighbourCellIndex);
        }

        if (quad.size() == 3)
        {
            squareQuads.push_back(quad);
            ++this->squareQuadsNumber;
        }

        quad.clear();
    }
}

void KmapCell::findSquareQuadsWithSharing()
{
    std::vector<int> quad;
    int sharingCounter = 0; 

    for (const auto& quadOffsets : quadsOffsets)
    {
        for (const auto& offsetPair : quadOffsets)
        {
            int neighbourCellIndex = getCellIndex(this->cellIndex, offsetPair.first, offsetPair.second); 
            Value neighbourCellValue = kmapObject.getCellValue(neighbourCellIndex);
            if (neighbourCellValue == Value::zero) 
                break;
            quad.push_back(neighbourCellIndex);
        }

        if (quad.size() == 3)
        {
            squareQuadsWithSharing.push_back(quad);
            ++this->numberOfSquareQuadsWithSharing;
        }

        quad.clear();
    }
}

void KmapCell::findRectQuads()
{
    std::vector<int> verticalQuad; 
    std::vector<int> horizontalQuad; 

    for (const auto offset : {1, 2, 3})
    {   
        int quadCellIndex = getCellIndex(this->cellIndex, offset, 0);
        Value quadCellValue = kmapObject.getCellValue(quadCellIndex);
        if (quadCellValue == Value::one)
            verticalQuad.push_back(quadCellIndex);

        quadCellIndex = getCellIndex(this->cellIndex, 0, offset);
        quadCellValue = kmapObject.getCellValue(quadCellIndex);
        if (quadCellValue == Value::one)
            horizontalQuad.push_back(quadCellIndex);
    }

    if (verticalQuad.size() == 3)
    {
        rectQuads.push_back(verticalQuad);
        ++this->rectQuadsNumber;
    }

    if (horizontalQuad.size() == 3)
    {
        rectQuads.push_back(horizontalQuad);
        ++this->rectQuadsNumber;
    }
}

void KmapCell::removePairContainingGivenCellIndex(int cellIndex)
{
    const auto pairsLength = pairs.size();
    // std::cout << "Pairs(" << cellIndex << "): ";
    // for (const auto pair : pairs) std::cout << pair << " ";
    // std::cout << std::endl;
    auto newEnd = std::remove(pairs.begin(), pairs.end(), cellIndex);
    pairs.erase(newEnd, pairs.end());
    
    if (pairs.size() != pairsLength)
    {
        --this->pairsNumber;
    }

    // std::cout << "pairs before " << pairsLength << std::endl; 
    // std::cout << "pairs after " << pairs.size() << std::endl; 
}

void KmapCell::removeSquareQuadContainingGivenCellIndex(int cellIndex)
{
    for (auto quadIter = squareQuads.begin(); quadIter != squareQuads.end();)
    {
        auto iter = std::find(quadIter->begin(), quadIter->end(), cellIndex);
        if (iter != quadIter->end() and squareQuads.size() > 1)
        {
            quadIter = squareQuads.erase(quadIter);
            --this->squareQuadsNumber;
        }
        else
        {
            ++quadIter;
        }
    }
}

void KmapCell::removeRectQuadContainingGivenCellIndex(int cellIndex)
{
    for (auto quadIter = rectQuads.begin(); quadIter != rectQuads.end(); ++quadIter)
    {
        auto iter = std::find(quadIter->begin(), quadIter->end(), cellIndex);
        if (iter != quadIter->end() and rectQuads.size() > 1)
        {
            quadIter = rectQuads.erase(quadIter);
            --this->rectQuadsNumber;
        }
        else
        {
            ++quadIter;
        }
    }
}