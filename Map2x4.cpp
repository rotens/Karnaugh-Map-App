#include "Map2x4.hpp"
#include "Kmap2x4Cell.hpp"
#include <algorithm>
#include <numeric>
#include <string>

Map2x4::Map2x4()
{
    int i = 0;
    for (auto& elem : kmap)
    {
        elem = new Kmap2x4Cell(*this, i);
        ++i;
    }
}

Map2x4::~Map2x4()
{
    for (auto& elem : kmap)
    {
        delete elem;
    }
}

void Map2x4::findGroups()
{
    if (ones == 8)
    {
        return;
    }

    if (ones == 0)
    {
        return;
    }

    findSquareQuads();
    if (hasAllCellsGrouped()) return;

    findRectQuads();
    if (hasAllCellsGrouped()) return;

    findPossiblePairs();
    pairCells(1);
    if (hasAllCellsGrouped()) return;

    pairCells(1);
    if (hasAllCellsGrouped()) return;

    pairCells(2);
    pairCells(1);
    if (hasAllCellsGrouped()) return;

    findSingleGroups();
}

bool Map2x4::hasAllCellsGrouped()
{
    int groupedCells = std::count_if(
        kmap.begin(), kmap.end(), [](Kmap2x4Cell* cell) { return cell->isDone(); });
    return groupedCells == ones;
}

void Map2x4::findSquareQuads()
{
    int oneValues = 0;

    for (const auto cellIndex : {0, 1, 2, 3})
    {
        if (kmap[cellIndex]->getCellValue() == Value::zero)
        {
            continue;
        }

        for (const auto offset : {1, 4, 5})
        {
            if (kmap[getRealIndex_Map2x4(cellIndex, offset)]->getCellValue() == Value::one)
            {
                ++oneValues;
            }
        }

        if (oneValues != 3)
        {
            oneValues = 0;
            continue;
        }

        std::vector<int> quad;
        quad.reserve(4);
        quad.push_back(kmap[cellIndex]->getIndex());
        kmap[cellIndex]->setDone();

        for (const auto offset : {1, 4, 5})
        {   
            int index = getRealIndex_Map2x4(cellIndex, offset);
            quad.push_back(index);
            kmap[index]->setDone();
        }

        squareQuads.push_back(std::move(quad));

        oneValues = 0;
    }
}

void Map2x4::findRectQuads()
{
    int oneValues = 0;

    for (const auto cellIndex : {0, 4})
    {
        if (kmap[cellIndex]->getCellValue() == Value::zero)
        {
            continue;
        }

        for (const auto offset : {1, 2, 3})
        {
            if (kmap[cellIndex + offset]->getCellValue() == Value::one)
            {
                ++oneValues;
            }
        }

        if (oneValues != 3)
        {
            oneValues = 0;
            continue;
        }
        
        std::vector<int> quad(4);
        std::iota(quad.begin(), quad.end(), kmap[cellIndex]->getIndex());
        std::for_each(quad.begin(), quad.end(), [this](int index) { kmap[index]->setDone(); });
        rectQuads.push_back(std::move(quad));
        oneValues = 0;
    }
}

void Map2x4::findPossiblePairs()
{
    for (auto& cell : kmap)
    {   
        if (not cell->isDone() and cell->getCellValue() == Value::one)
        {
            cell->findPairs();
        }
    }
}

void Map2x4::pairCells(int pairsNumber)
{
    for (auto& cell : kmap)
    {
        if (cell->isDone()) continue;

        if (cell->getPairsNumber() == pairsNumber)
        {
            cell->setDone();
            int index = pairsNumber == 1 ? 0 : 1;
            int secondCellIndex = cell->getPairs()[index];
            kmap[secondCellIndex]->setDone();
            
            std::vector<int> pair{cell->getIndex(), secondCellIndex};   
            justGroupedCells.insert({cell->getIndex(), secondCellIndex});         
            pairs.push_back(std::move(pair));

            pairFound = true;
            decrementGroupingPossibilities();
            if (pairsNumber == 2) break;
        }
    }
}

void Map2x4::decrementGroupingPossibilities()
{
    for (auto cellIndex : justGroupedCells)
    {
        for (auto& cell : kmap)
        {
            if (cell->isDone() or cell->getCellValue() == Value::zero) 
                continue;

            if (cell->getPairsNumber() > 1)
            {
                cell->removePairContainingGivenCellIndex(cellIndex);
            }
        } 
    }

    justGroupedCells.clear();
}

void Map2x4::findSingleGroups()
{
    for (auto& cell : kmap)
    {   
        if (cell->isDone() or cell->getCellValue() == Value::zero)
        {
            continue;
        }

        if (not cell->hasPairs())
        {
            singleGroups.push_back(cell->getIndex());
        }
    }
}

void Map2x4::findAlgebraicMintermOfSingleGroup(int cellIndex)
{
    std::string minterm = "";
    int row = cellIndex / 4;
    int col = cellIndex % 4;
    std::string argValues = grayCode[col] + std::to_string(row);

    for (int i = 0; i < 3; ++i)
    {   
        if (argValues[i] == '1')
        {
            minterm += variables[i];
        }
        else
        {
            minterm += "!" + variables[i];
        }
    }

    algebraicMinterms.push_back(std::move(minterm));
}

void Map2x4::findAlgebraicMintermsForGivenGroup(const Groups& groups)
{
    std::string product;
    std::vector<std::string> cellsBinaryNumbers;

    for (const auto& group : groups)
    {
        for (const auto& cell : group)
        {
            int row = cell / 4;
            int col = cell % 4;
            cellsBinaryNumbers.push_back(grayCode[col] + std::to_string(row));
        }

        product = getProduct(cellsBinaryNumbers);
        algebraicMinterms.push_back(product);
        cellsBinaryNumbers.clear();
    }
}

std::string Map2x4::getProduct(std::vector<std::string>& cellsBinaryNumbers)
{
    int ones = 0;
    std::string product = "";

    for (int i = 0; i < 3; ++i)
    {
        for (const auto& value : cellsBinaryNumbers)
        {
            if (value[i] == '1')
                ++ones; 
        }

        if (ones == cellsBinaryNumbers.size())
        {
            product += variables[i];
            ones = 0;
            continue;
        }

        if (ones == 0)
        {
            product += '!' + variables[i];
        }

        ones = 0;
    }
    
    return product;
}

void Map2x4::findAlgebraicMinterms()
{
    if (ones == 8)
    {
        algebraicMinterms.push_back("1");
        return;
    }

    if (ones == 0)
    {
        algebraicMinterms.push_back("0");
        return;
    }

    findAlgebraicMintermsForGivenGroup(rectQuads);
    findAlgebraicMintermsForGivenGroup(squareQuads);
    findAlgebraicMintermsForGivenGroup(pairs);
    
    for (const auto cellIndex : singleGroups)
    {
        findAlgebraicMintermOfSingleGroup(cellIndex);
    }
}

void Map2x4::initializeElementsWithGivenValues(const std::vector<Value>& values)
{
    auto valuesIt = values.begin();
    for (auto& elem : kmap)
    {
        elem->setCellValue(*valuesIt);
        
        if (*valuesIt == Value::one)
            ++ones; 

        ++valuesIt;
    }
}

void Map2x4::printSquareQuads()
{
    for (const auto& quads : squareQuads)
    {
        for (const auto& elem : quads)
        {
            std::cout << elem << " ";
        }
        std::cout << "\n";
    }
}

void Map2x4::printRectQuads()
{
    for (const auto& quads : rectQuads)
    {
        for (const auto& elem : quads)
        {
            std::cout << elem << " ";
        }
        std::cout << "\n";
    }
}

void Map2x4::printPairs()
{
    for (const auto& pair : pairs)
    {
        for (const auto& elem : pair)
        {
            std::cout << elem << " ";
        }
        std::cout << "\n";
    }
}

void Map2x4::printKmap()
{
    for (int i = 0; i < kmap.size(); ++i)
    {
         std::cout << static_cast<char>(kmap[i]->getCellValue()) << " ";
         if (i % 4 == 3) std::cout << "\n";
    }
}

void Map2x4::printAlgebraicMinterms()
{
    for (const auto& product : algebraicMinterms)
    {
        std::cout << product << "+";
    }
    std::cout << std::endl;
}

Value Map2x4::getCellValue(int cellIndex)
{ 
    return kmap[cellIndex % 8]->getCellValue(); 
}

void Map2x4::fillMapWithZeroValues()
{
    std::for_each(kmap.begin(), kmap.end(), 
        [](Kmap2x4Cell* cell) { cell->setCellValue(Value::zero); });
    ones = 0;
}
