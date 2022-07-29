#include "Map4x4.hpp"
#include "KmapCell.hpp"
#include <algorithm>
#include <cmath>
#include <tuple>

#define __TEST__ std::cout<<"TEST"<<std::endl;

int getRealIndex(int startIndex)
{
    int col = startIndex % 4;
    int row = startIndex / 4;
    return (col + 1) % 4 + row*4;
}

// MAP 4x4

Map4x4::Map4x4()
{
    int i = 0;
    for (auto& elem : kmap)
    {
        elem = new KmapCell(*this, i);
        ++i;
    }
}

Map4x4::~Map4x4()
{
    for (auto& elem : kmap)
    {
        delete elem;
    }
}

Value Map4x4::getCellValue(int cellIndex)
{ 
    return kmap[cellIndex % 16]->getCellValue(); 
}

bool Map4x4::isCellDone(int cellIndex)
{
    return kmap[cellIndex % 16]->isDone();
}

void Map4x4::printKmap()
{
    for (int i = 0; i < kmap.size(); ++i)
    {
         std::cout << static_cast<char>(kmap[i]->getCellValue()) << " ";
         if (i % 4 == 3) std::cout << "\n";
    }
}

void Map4x4::initializeElementsWithGivenValues(const std::vector<Value>& values)
{
    auto valuesIt = values.begin();
    for (auto& elem : kmap)
    {
        elem->setCellValue(*valuesIt);
        ++valuesIt;
        
        if (*valuesIt == Value::one)
            ++ones; 
    }
}

void Map4x4::findHorizontalOctets()
{
    bool zeroValue;

    for (int i = 0; i <= 3*4; i += 4)
    {
        zeroValue = false;

        for (int j = i; j < i+8; ++j)
        {
            if (kmap[j % 16]->getCellValue() == Value::zero)
            {
                zeroValue = true;
                break;
            }
        }

        if (!zeroValue)
        {
            std::vector<int> octet;
            octet.reserve(8);
            for (int k = i; k < i+8; ++k)
            {
                kmap[k % 16]->setDone();
                octet.push_back(k % 16);
            }
            octets.push_back(std::move(octet));
        }
    }
}

void Map4x4::findVerticalOctets()
{
    bool zeroValue;

    for (int i = 0; i < 4; ++i)
    {
        zeroValue = false;

        for (int j = i; j <= (4*3 + i); j += 4)
        {
            if (kmap[j]->getCellValue() == Value::zero
                or kmap[getRealIndex(j)]->getCellValue() == Value::zero)
            {
                zeroValue = true;
                break;
            }
        }

        if (!zeroValue)
        {
            std::vector<int> octet;
            octet.reserve(8);
            for (int k = i; k <= (4*3 + i); k += 4)
            {
                octet.push_back(k);
                octet.push_back(getRealIndex(k));
                kmap[k]->setDone();
                kmap[getRealIndex(k)]->setDone();
            }
            octets.push_back(std::move(octet));
        }
    }
}

void Map4x4::findOctets()
{
    findHorizontalOctets();
    findVerticalOctets();
}

void Map4x4::findPossiblePairs()
{
    for (auto& cell : kmap)
    {
        if (not cell->isDone() and cell->getCellValue() == Value::one)
            cell->findPairs();
    }
}

void Map4x4::findPossiblePairsWithSharing()
{
    for (auto& cell : kmap)
    {
        if (not cell->isDone() and cell->getCellValue() == Value::one)
            cell->findPairsWithSharing();
    }
}

void Map4x4::pairCells(int pairsNumber)
{
    for (auto& cell : kmap)
    {
        if (cell->isDone()) continue;

        if (cell->getPairsNumber() == pairsNumber)
        {
            cell->setDone();
            int secondCellIndex = cell->getPairs()[0];
            kmap[secondCellIndex]->setDone();
            
            std::vector<int> pair{cell->getIndex(), secondCellIndex};   
            justGroupedCells.insert({cell->getIndex(), secondCellIndex});         
            pairs.push_back(std::move(pair));

            pairFound1 = true;
            pairFound2 = true;
        }
    }
}

void Map4x4::pairCellsWithSharing()
{
    for (auto& cell : kmap)
    {
        if (cell->isDone()) continue;

        if (cell->getPairsNumber() == 2)
        {
            cell->setDone();
            int secondCellIndex = cell->getPairs()[0];
            kmap[secondCellIndex]->setDone();
            
            std::vector<int> pair{cell->getIndex(), secondCellIndex};   
            justGroupedCells.insert(cell->getIndex());         
            pairs.push_back(std::move(pair));
        }
    }
}

void Map4x4::repeatPairingCellsWithTwoPossibilities()
{
    do
    {
        pairFound2 = false;
        pairCells(2);
        repeatPairingCellsWithOnePossibility();
    } while (pairFound2);
}

void Map4x4::repeatPairingCellsWithTwoPossibilitiesAndSharing()
{
    do
    {
        pairFound2 = false;
        pairCellsWithSharing();
        repeatPairingCellsWithOnePossibility();
    } while (pairFound2);
}

void Map4x4::repeatPairingCellsWithOnePossibility()
{
    do
    {
        pairFound1 = false;
        pairCells(1);
    } while (pairFound1);
}

void Map4x4::squareQuadCells(KmapCell* cell)
{
    auto& possibleQuad = cell->getSquareQuads()[0];
    for (auto quadedCellIndex : possibleQuad)
    {
        kmap[quadedCellIndex]->setDone();
    }
    
    std::vector<int> quad{possibleQuad};
    quad.insert(quad.begin(), cell->getIndex());
    justGroupedCells.insert(quad.begin(), quad.end());
    squareQuads.push_back(std::move(quad));
}

// void Map4x4::addSquareQuadWithSharing(KmapCell* cell, int index)
// {
//     auto& possibleQuad = cell->getSquareQuadsWithSharing()[index];
//     for (auto quadedCellIndex : possibleQuad)
//     {
//         kmap[quadedCellIndex]->setDone();
//     }
    
//     std::vector<int> quad{possibleQuad};
//     quad.insert(quad.begin(), cell->getIndex());
//     squareQuads.push_back(std::move(quad));
// }

void Map4x4::addSquareQuadWithSharing(KmapCell* cell, int index)
{ 
    auto& possibleQuad = cell->getSquareQuadsWithSharing()[index];
    std::vector<int> quad{possibleQuad};
    quad.insert(quad.begin(), cell->getIndex());
    insertJustGroupedCells(quad);
    squareQuads.push_back(std::move(quad));

    cell->setDone();
    for (const auto quadedCellIndex : possibleQuad)
    {
        kmap[quadedCellIndex]->setDone();
    }
}

void Map4x4::rectQuadCells(KmapCell* cell)
{ 
    auto& possibleQuad = cell->getRectQuads()[0];
    for (const auto quadedCellIndex : possibleQuad)
    {
        kmap[quadedCellIndex]->setDone();
    }

    std::vector<int> quad{possibleQuad};
    quad.insert(quad.begin(), cell->getIndex());
    justGroupedCells.insert(quad.begin(), quad.end());
    rectQuads.push_back(std::move(quad));
}

void Map4x4::addRectQuadWithSharing(KmapCell* cell, int index)
{ 
    auto& possibleQuad = cell->getRectQuadsWithSharing()[index];
    std::vector<int> quad{possibleQuad};
    quad.insert(quad.begin(), cell->getIndex());
    insertJustGroupedCells(quad);
    rectQuads.push_back(std::move(quad));

    cell->setDone();
    for (const auto quadedCellIndex : possibleQuad)
    {
        kmap[quadedCellIndex]->setDone();
    }
}

void Map4x4::findPossibleQuads()
{
    for (auto& cell : kmap)
    {   
        if (not cell->isDone() and cell->getCellValue() == Value::one)
        {
            cell->findRectQuads();
            cell->findSquareQuads();
        }
    }
}

void Map4x4::quadCellsWithOnePossibility()
{
    for (auto& cell : kmap)
    {
        if (cell->isDone()) continue;
    
        if (cell->getQuadsNumber() != 1) continue;
        
        cell->setDone();

        if (cell->getRectQuadsNumber() == 1)
        {
            rectQuadCells(cell);
        }
        else
        {
            squareQuadCells(cell);
        }
    }
}

void Map4x4::findPossibleQuadsWithSharing()
{
    for (auto& cell : kmap)
    {   
        if (not cell->isDone() and cell->getCellValue() == Value::one)
        {
            cell->findRectQuadsWithSharing();
            cell->findSquareQuadsWithSharing();
        }
    }
}

void Map4x4::quadCellsWithTwoPossibilities()
{
    for (auto& cell : kmap)
    {
        if (cell->isDone()) continue;
    
        if (cell->getQuadsNumber() != 2) continue;
        
        cell->setDone();

        if (cell->getRectQuadsNumber() == 1 or cell->getRectQuadsNumber() == 2)
        {
            rectQuadCells(cell);
        }
        else
        {
            squareQuadCells(cell);
        }

        quadFound = true;

        decrementGroupingPossibilities();
    }
}

void Map4x4::repeatQuadingCellsWithTwoPossibilities()
{
    do
    {
        quadFound = false;
        quadCellsWithTwoPossibilities();
    } while (quadFound);
}

void Map4x4::quadCellsWithTwoPossibilitiesAndWithSharing()
{
    for (auto& cell : kmap)
    {
        if (cell->isDone()) continue;
    
        if (cell->getNumberOfQuadsWithSharing() != 2) continue;
        
        // cell->setDone();

        if (cell->getNumberOfRectQuadsWithSharing() == 2)
        {
            auto index = cell->getIndexOfRectQuadWithMinimalSharing();
            addRectQuadWithSharing(cell, index);
        } 
        else if (cell->getNumberOfSquareQuadsWithSharing() == 2)
        {
            auto index = cell->getIndexOfSquareQuadWithMinimalSharing();
            addSquareQuadWithSharing(cell, index);
        }
        else
        {
            auto rectIndex = cell->getIndexOfRectQuadWithMinimalSharing() - 1;
            auto rectCounter = cell->getRectQuadsSharingCounter(rectIndex);

            auto squareIndex = cell->getIndexOfSquareQuadWithMinimalSharing() - 1;
            auto squareCounter = cell->getSquareQuadsSharingCounter(squareIndex);

            // std::cout << "Rect index " << rectIndex << std::endl;
            // std::cout << "Rect counter " << rectCounter << std::endl;
            // std::cout << "Square index " << squareIndex << std::endl;
            // std::cout << "Square counter " << squareCounter << std::endl;

            if (rectCounter <= squareCounter)
            {
                addRectQuadWithSharing(cell, rectIndex);
            }
            else
            {
                addSquareQuadWithSharing(cell, squareIndex);
            }
        }
       
        quadFound = true;
    }
}

void Map4x4::findSingleGroups()
{
    for (auto& cell : kmap)
    {   
        if (cell->isDone() or cell->getCellValue() == Value::zero)
        {
            continue;
        }

        if (not cell->hasPairsOrQuads())
        {
            singleGroups.push_back(cell->getIndex());
        }
    }
}

void Map4x4::decrementGroupingPossibilities()
{
    // __TEST__;
    for (auto cellIndex : justGroupedCells)
    {
        // std::cout << "Cell index " << cellIndex << std::endl;
        for (auto& cell : kmap)
        {
            if (cell->isDone() or cell->getCellValue() == Value::zero) 
                continue;

            if (cell->getPairsNumber() > 1)
            {
                // std::cout << cell->getIndex() << " " << cell->getPairsNumber() << std::endl;
                cell->removePairContainingGivenCellIndex(cellIndex);
            }

            if (cell->getQuadsNumber() > 1)
            {
                if (cell->getSquareQuadsNumber() == 0)
                {
                    cell->removeRectQuadContainingGivenCellIndex(cellIndex);
                    continue;
                }

                if (cell->getRectQuadsNumber() == 0)
                {
                    cell->removeSquareQuadContainingGivenCellIndex(cellIndex);
                    continue;
                }

                // TODO
                if (cell->getRectQuadsNumber() == 1 and cell->getSquareQuadsNumber() == 1)
                {
                    cell->removeSquareQuadContainingGivenCellIndex(cellIndex);
                    continue;
                }
            }
        } 
    }

    justGroupedCells.clear();
}

void Map4x4::insertJustGroupedCells(std::vector<int>& cells)
{
    for (const auto cellIndex : cells)
    {
        if (not this->isCellDone(cellIndex))
        {
            justGroupedCells.insert(cellIndex);
        }
    }
}

void Map4x4::findGroups()
{
    if (ones == 16)
    {
        // algebraicMinterms.push_back("1");
        return;
    }

    if (ones == 0)
    {
        // algebraicMinterms.push_back("0");
        return;
    }
    // __TEST__;

    // STEP 1
    findHorizontalOctets();
    if (hasAllCellsGrouped()) return;

    findVerticalOctets();
    if (hasAllCellsGrouped()) return;

    // STEP 2
    findPossiblePairs();
    pairCells(1);
    decrementGroupingPossibilities();
    if (hasAllCellsGrouped()) return;

    // STEP 3
    findPossibleQuads();
    quadCellsWithOnePossibility();
    decrementGroupingPossibilities();
    if (hasAllCellsGrouped()) return;

    pairCells(1);
    decrementGroupingPossibilities();
    if (hasAllCellsGrouped()) return;

    quadCellsWithOnePossibility();
    decrementGroupingPossibilities();
    if (hasAllCellsGrouped()) return;

    // STEP 4
    repeatQuadingCellsWithTwoPossibilities();
    if (hasAllCellsGrouped()) return;

    // STEP 5
    findPossibleQuadsWithSharing();
    quadCellsWithTwoPossibilitiesAndWithSharing();
    decrementGroupingPossibilities();
    if (hasAllCellsGrouped()) return;

    // STEP 6
    repeatPairingCellsWithTwoPossibilities();
    if (hasAllCellsGrouped()) return;

    // STEP 7
    findPossiblePairsWithSharing();
    repeatPairingCellsWithTwoPossibilitiesAndSharing();
    if (hasAllCellsGrouped()) return;
}


bool Map4x4::hasAllCellsGrouped()
{
    int groupedCells = std::count_if(
        kmap.begin(), kmap.end(), [](KmapCell* cell) { return cell->isDone(); });
    return groupedCells == 16;
}

void Map4x4::printOctets()
{
    for (const auto& octet : octets)
    {
        for (const auto& elem : octet)
        {
            std::cout << elem << " ";
        }
        std::cout << "\n";
    }
}

void Map4x4::printPairs()
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

void Map4x4::printSquareQuads()
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

void Map4x4::printRectQuads()
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

void Map4x4::printEverything()
{
    std::cout << "Octets: \n";
    printOctets();
    std::cout << "\nSquare quads: \n";
    printSquareQuads();
    std::cout << "\nRect quads: \n";
    printRectQuads();
    std::cout << "\nPrint pairs: \n";
    printPairs();
    std::cout << "\nAlgebraic minterms: \n";
    printAlgebraicMinterms();
}

void Map4x4::printAlgebraicMinterms()
{
    for (const auto& product : algebraicMinterms)
    {
        std::cout << product << "+";
    }
    std::cout << std::endl;
}

void Map4x4::findAlgebraicMinterms()
{
    findAlgebraicMintermsForGivenGroup(pairs);
    findAlgebraicMintermsForGivenGroup(rectQuads);
    findAlgebraicMintermsForGivenGroup(squareQuads);
    findAlgebraicMintermsForGivenGroup(octets);

    for (const auto cellIndex : singleGroups)
    {
        findAlgebraicMintermOfSingleGroup(cellIndex);
    }
}

void Map4x4::findAlgebraicMintermsForGivenGroup(const Groups& groups)
{
    std::string product;
    std::vector<std::string> cellsBinaryNumbers;

    for (const auto& group : groups)
    {
        for (const auto& cell : group)
        {
            int row = cell / 4;
            int col = cell % 4;
            cellsBinaryNumbers.push_back(grayCode[col] + grayCode[row]);
        }

        // for (const auto& cell : cellsBinaryNumbers)
        // {
        //     std::cout << cell << std::endl;
        // }

        product = getProduct(cellsBinaryNumbers);
        algebraicMinterms.push_back(product);
        cellsBinaryNumbers.clear();
    }
}

std::string Map4x4::getProduct(std::vector<std::string>& cellsBinaryNumbers)
{
    int ones = 0;
    std::string product = "";

    for (int i = 0; i < 4; ++i)
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

void Map4x4::findAlgebraicMintermOfSingleGroup(int cellIndex)
{
    std::string minterm = "";
    int row = cellIndex / 4;
    int col = cellIndex % 4;
    std::string argValues = grayCode[col] + grayCode[row];

    for (int i = 0; i < 4; ++i)
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

// void Map4x4::addRect4x2Group(int8_t row, int8_t col, int8_t height, int8_t width)
// {
//     int8_t k = 0;
//     std::array<std::pair<int8_t, int8_t>, 8> rect4x2Group;

//     for (int8_t i = row; i < row+height; ++i)
//     {
//         for (int8_t j = col; j < col+width; ++j)
//         {
//             rect4x2Group[k++] = getRealIndices(i, j);
//             cellsInGroup[translateIndices(i, j)] = 1;
//             ++cellsNumberOfGroups[translateIndices(i, j)];
//         }
//     }

//     rect4x2Groups.push_back(std::move(rect4x2Group));
// }

// void Map4x4::solve()
// {
//     countZeroesAndOnes();
//     findGroups();
//     // findAlgebraicMinterms<8>(rect4x2Groups);
//     // findAlgebraicMinterms<4>(rect4x1Groups);
//     // findAlgebraicMinterms<4>(square2x2Groups);
//     // findAlgebraicMinterms<2>(rect2x1Groups);
//     // findAlgebraicMinterms(rect4x2Groups);
// }

// void Map4x4::findGroups()
// {
//     findRect4x2Groups();
//     findRect4x1Groups();
//     findSquare2x2Groups();
//     findRect2x1Groups();
//     find1x1Groups();
//     removeRedundantGroups();
// }

// void Map4x4::findRect4x2Groups()
// {
//     for (int8_t i = 0; i < 4; ++i)
//     {
//         if (isGroup(i, 0, 2, 4))
//         {
//             addRect4x2Group(i, 0, 2, 4);

//             if (not isAnyCellNotInGroupExistent())
//                 return;
//         }
//     }

//     for (int8_t i = 0; i < 4; ++i)
//     {
//         if (isGroup(0, i, 4, 2))
//         {
//             addRect4x2Group(0, i, 4, 2);

//             if (not isAnyCellNotInGroupExistent())
//                  return;
//         }
//     }
// }

// void Map4x4::addRect4x2Group(int8_t row, int8_t col, int8_t height, int8_t width)
// {
//     int8_t k = 0;
//     std::array<std::pair<int8_t, int8_t>, 8> rect4x2Group;

//     for (int8_t i = row; i < row+height; ++i)
//     {
//         for (int8_t j = col; j < col+width; ++j)
//         {
//             rect4x2Group[k++] = getRealIndices(i, j);
//             cellsInGroup[translateIndices(i, j)] = 1;
//             ++cellsNumberOfGroups[translateIndices(i, j)];
//         }
//     }

//     rect4x2Groups.push_back(std::move(rect4x2Group));
// }

// void Map4x4::findSquare2x2Groups() 
// {
//     for (int8_t i = 0; i < 4; ++i)
//     {
//         for (int8_t j = 0; j < 4; ++j) 
//         {
//             if (isGroup(i, j, 2, 2) and not isContainedInGroup<8>(rect4x2Groups, i, j, 2, 2))
//             {
//                 addSquare2x2Group(i, j);

//                 // if (not isAnyCellNotInGroupExistent())
//                 //     return;
//             }
//         }
//     }
// }

// void Map4x4::addSquare2x2Group(int8_t row, int8_t col)
// {
//     int8_t k = 0;
//     std::array<std::pair<int8_t, int8_t>, 4> newGroup;

//     for (int8_t i = row; i < row+2; ++i)
//     {
//         for (int8_t j = col; j < col+2; ++j)
//         {
//             newGroup[k++] = getRealIndices(i, j);
//             cellsInGroup[translateIndices(i, j)] = 1;
//             ++cellsNumberOfGroups[translateIndices(i, j)];
//         }
//     }
    
//     square2x2Groups.push_back(std::move(newGroup));
// }

// void Map4x4::findRect4x1Groups()
// {
//     for (int8_t i = 0; i < 4; ++i)
//     {
//         /* Horizontal group */
//         if (isGroup(i, 0, 1, 4) and not is4x1GroupContainedIn4x2Group(i, 0, 1, 4))
//         {
//             addRect4x1Group(i, 0, 1, 4);

//             if (not isAnyCellNotInGroupExistent())
//                 return;
//         }
//     }

//     for (int8_t i = 0; i < 4; ++i)
//     {
//         /* Vertical group */
//         if (isGroup(0, i, 4, 1) and not is4x1GroupContainedIn4x2Group(0, i, 4, 1))
//         {
//             addRect4x1Group(0, i, 4, 1);

//             if (not isAnyCellNotInGroupExistent())
//                 return;
//         }
//     }
// }

// void Map4x4::addRect4x1Group(int8_t row, int8_t col, int8_t height, int8_t width)
// {
//     int8_t k = 0;
//     std::array<std::pair<int8_t, int8_t>, 4> newGroup;

//     for (int8_t i = row; i < row+height; ++i) 
//     {
//         for (int8_t j = col; j < col+width; ++j)
//         {
//             newGroup[k++] = getRealIndices(i, j);
//             cellsInGroup[translateIndices(i, j)] = 1;
//             ++cellsNumberOfGroups[translateIndices(i, j)];
//         }
//     }

//     rect4x1Groups.push_back(std::move(newGroup));
// }

// void Map4x4::findRect2x1Groups()
// {
//     for (int8_t i = 0; i < 4; ++i)
//     {
//         for (int8_t j = 0; j < 4; ++j) 
//         {
//             if (isGroup(i, j, 1, 2)
//                 and not isContainedInGroup<8>(rect4x2Groups, i, j, 1, 2)
//                 and not isContainedInGroup<4>(square2x2Groups, i, j, 1, 2)
//                 and not isContainedInGroup<4>(rect4x1Groups, i, j, 1, 2))
//             {
//                 addRect2x1Group(i, j, 1, 2);

//                 if (not isAnyCellNotInGroupExistent())
//                     return;
//             }

//             if (isGroup(i, j, 2, 1)
//                 and not isContainedInGroup<8>(rect4x2Groups, i, j, 2, 1)
//                 and not isContainedInGroup<4>(square2x2Groups, i, j, 2, 1)
//                 and not isContainedInGroup<4>(rect4x1Groups, i, j, 2, 1))
//             {
//                 addRect2x1Group(i, j, 2, 1);

//                 if (not isAnyCellNotInGroupExistent())
//                     return;
//             }
//         }
//     }
// }

// void Map4x4::addRect2x1Group(int8_t row, int8_t col, int8_t height, int8_t width)
// {
//     int8_t k = 0;
//     std::array<std::pair<int8_t, int8_t>, 2> newGroup;

//     for (int8_t i = row; i < row+height; ++i) 
//     {
//         for (int8_t j = col; j < col+width; ++j)
//         {
//             newGroup[k++] = getRealIndices(i, j);
//             cellsInGroup[translateIndices(i, j)] = 1;
//             ++cellsNumberOfGroups[translateIndices(i, j)];
//         }
//     }

//     rect2x1Groups.push_back(std::move(newGroup));
// }

// void Map4x4::find1x1Groups()
// {
//     for (int8_t i = 0; i < 4; ++i) 
//     {
//         for (int8_t j = 0; j < 4; ++j)
//         {
//             if (is1x1Group(i, j))
//             {
//                 add1x1Group(i, j);

//                 if (not isAnyCellNotInGroupExistent())
//                     return;
//             }
//         }
//     }
// }

// bool Map4x4::is1x1Group(int8_t row, int8_t col)
// {
//     int8_t x, y;

//     if (kmap[row][col] != Value::one)
//         return false;

//     for (int8_t i : {-1, 1})
//     {
//         std::tie(x, y) = getRealIndices(row+i, col);
//         if (kmap[x][y] == Value::one or kmap[x][y] == Value::dont_care)
//             return false;

//         std::tie(x, y) = getRealIndices(row, col+i);
//         if (kmap[x][y] == Value::one or kmap[x][y] == Value::dont_care)
//             return false;
//     }

//     return true;
// }

// void Map4x4::add1x1Group(int8_t row, int8_t col)
// {
//     _1x1Groups.push_back(std::make_pair(row, col));
//     cellsInGroup[translateIndices(row, col)] = 1;
//     ++cellsNumberOfGroups[translateIndices(row, col)];
// }

// bool Map4x4::isGroup(int8_t row, int8_t col, int8_t height, int8_t width)
// {
//     int8_t x, y;
//     int8_t dontCares = 0;

//     for (int8_t i = row; i < row+height; ++i)
//     {
//         for (int8_t j = col; j < col+width; ++j)
//         {
//             std::tie(x, y) = getRealIndices(i, j);

//             if (kmap[x][y] != Value::one) 
//             {
//                 if (kmap[x][y] != Value::dont_care)
//                 {
//                     return false;
//                 }
                    
//                 ++dontCares;
//             }
//         }
//     }

//     if (dontCares == width*height)
//         return false;
    
//     return true;
// }

// bool Map4x4::is4x1GroupContainedIn4x2Group(int8_t row, int8_t col, int8_t height, int8_t width)
// {
//     int8_t matches = 0, k = 0;
//     std::array<std::pair<int8_t, int8_t>, 4> groupCells;

//     for (int8_t i = row; i < row+height; ++i)
//     {
//         for (int8_t j = col; j < col+width; ++j)
//         {
//             groupCells[k++] = std::make_pair(i, j);
//         }
//     }

//     for (const auto& group : rect4x2Groups)
//     {
//         for (const auto& elem : group) 
//         {
//             for (const auto& cell : groupCells)
//             {
//                 if (elem == cell)
//                     ++matches;
//             }
//         }

//         if (matches == 4)
//             return true;

//         matches = 0;
//     }

//     return false;
// }

// template<int N> 
// bool Map4x4::isContainedInGroup(
//     std::vector<std::array<std::pair<int8_t, int8_t>, N>>& groups, 
//     int8_t row, int8_t col, int8_t height, int8_t width)
// {
//     int8_t matches = 0;
//     auto firstCell = std::pair<int8_t, int8_t>(row, col);
//     auto lastCell = getRealIndices(row+height-1, col+width-1);

//     for (const auto& group : groups)
//     {
//         for (const auto& elem : group) 
//         {
//             if (elem == firstCell)
//                 ++matches;

//             if (elem == lastCell)
//                 ++matches;
//         }

//         if (matches == 2)
//             return true;

//         matches = 0;
//     }

//     return false;
// }

// bool Map4x4::isAnyCellNotInGroupExistent()
// {
//     auto sum = std::accumulate(cellsInGroup.begin(), cellsInGroup.end(), 0);
//     return sum == ones ? false : true;
// }

// void Map4x4::removeRedundantGroups()
// {
//     removeRedundantGroupsFromGivenVector<4, VecIter<4>>(square2x2Groups);
//     // removeRedundantGroupsFromGivenVector<4, VecIter<4>>(rect4x1Groups);
//     // removeRedundantGroupsFromGivenVector<2, VecIter<2>>(rect2x1Groups);
// }

// template<int N, typename T> 
// void Map4x4::removeRedundantGroupsFromGivenVector(
//     std::vector<std::array<std::pair<int8_t, int8_t>, N>>& groups)
// {
//     std::vector<T> itersMax;
//     std::vector<T> testedIters;
//     int8_t index;
//     std::array<int8_t, 16> tempCellsNumberOfGroups{};
//     int8_t maxNumberOfGroups = static_cast<int8_t>(ceil(1.0*getNumberOfCellsInGroup<N>(groups) / N));
//     int8_t numberOfCellsInOnlyOneGroup = 0;
//     int8_t numberOfCellsInOnlyOneGroupMax = 0;

//     std::cout << (int)maxNumberOfGroups << std::endl;
//     if (maxNumberOfGroups == groups.size())
//         return;


//     auto groupsIter = groups.begin();

//     for (int i = 0; i <= groups.size()-maxNumberOfGroups; ++i)
//     {
//         for (int k = i+1; k <= groups.size()-maxNumberOfGroups+1; ++k)
//         {
//             testedIters.push_back(groupsIter + i);
//             for (int j = k; j < k+(maxNumberOfGroups-1); ++j)
//             {
//                 testedIters.push_back(groupsIter + j);
//             }

//             for (const auto& it : testedIters)
//             {
//                 for (const auto& cell : *it)
//                 {
//                     index = translateIndices(cell.first, cell.second);
//                     tempCellsNumberOfGroups[index] += 1;
//                 }
//             }

//             for (const auto& number : tempCellsNumberOfGroups)
//             {
//                 if (1 == number) 
//                 {
//                     ++numberOfCellsInOnlyOneGroup;
//                 }
//             }
//             std::cout << "Number of cells " << (int)numberOfCellsInOnlyOneGroup << std::endl;
//             for (const auto& it : testedIters)
//             {
//                 for (const auto& cell : *it)
//                 {
//                     std::cout << "(" << (int)cell.first << ", " << (int)cell.second << ") ";
//                 }
//                 std::cout << std::endl;
//             }
//             if (numberOfCellsInOnlyOneGroup > numberOfCellsInOnlyOneGroupMax)
//             {
//                 numberOfCellsInOnlyOneGroupMax = numberOfCellsInOnlyOneGroup;
//                 itersMax = testedIters; 
//             }
//             // __TEST__;   
//             numberOfCellsInOnlyOneGroup = 0; 
//             std::fill(tempCellsNumberOfGroups.begin(), tempCellsNumberOfGroups.end(), 0);
//             testedIters.clear();
//         }
    
    
//     }

//     // for (auto it = groups.begin(); it != groups.end() - maxNumberOfGroups + 1 ; ++it)
//     // {
       
//     //     for (auto it2 = it; it2 != it + maxNumberOfGroups; ++it2)
//     //     {
//     //         for (const auto& cell : *it2)
//     //         {
//     //             index = translateIndices(cell.first, cell.second);
//     //             tempCellsNumberOfGroups[index] += 1;
//     //         }
//     //         iters.push_back(it2);
//     //     }

//     //     for (const auto& number : tempCellsNumberOfGroups)
//     //     {
//     //         if (1 == number) 
//     //         {
//     //             ++numberOfCellsInOnlyOneGroup;
//     //         }
//     //     }

//     //     if (numberOfCellsInOnlyOneGroup > numberOfCellsInOnlyOneGroupMax)
//     //     {
//     //         numberOfCellsInOnlyOneGroupMax = numberOfCellsInOnlyOneGroup;
//     //         itersMax = iters; 
//     //     }
        
//     //     iters.clear();
//     //     std::fill(tempCellsNumberOfGroups.begin(), tempCellsNumberOfGroups.end(), 0);
//     // }

//     std::vector<std::array<std::pair<int8_t, int8_t>, N>> newGroups;

//     for (const auto& it : itersMax)
//     {
//         newGroups.push_back(*it);
//     }

//     groups = std::move(newGroups);

//     // for (auto it = groups.begin(); it != groups.end(); ++it)
//     // {
//     //     for (const auto& cell : *it)
//     //     {
//     //         index = translateIndices(cell.first, cell.second);
//     //         if (cellsNumberOfGroups[index] == 1)
//     //         {
//     //             hasCellInOnlyOneGroup = true;
//     //             break;
//     //         }
//     //     }

//     //     if (not hasCellInOnlyOneGroup)
//     //     {
//     //         iters.push_back(it);
//     //         // decrementNumberOfGroups<N>(*it);
//     //     }
//     // }

//     // for (const auto& iter : iters)
//     //     groups.erase(iter);
// }

// template<int N> 
// int Map4x4::getNumberOfCellsInGroup(std::vector<std::array<std::pair<int8_t, int8_t>, N>>& groups)
// {
//     std::array<int8_t, 16> _cellsInGroup{};
//     int8_t index;

//     for (auto it = groups.begin(); it != groups.end(); ++it)
//     {
//         for (const auto& cell : *it)
//         {
//             index = translateIndices(cell.first, cell.second);
//             _cellsInGroup[index] = 1;
//         }
//     }

//     return std::accumulate(_cellsInGroup.begin(), _cellsInGroup.end(), 0);
// }

// template<int N>
// void Map4x4::decrementNumberOfGroups(std::array<std::pair<int8_t, int8_t>, N>& group)
// {
//     int8_t index;

//     for (const auto& cell : group)
//     {
//         index = translateIndices(cell.first, cell.second);
//         --cellsNumberOfGroups[index];
//     }
// }

// template<int N>
// void Map4x4::findAlgebraicMinterms(std::vector<std::array<std::pair<int8_t, int8_t>, N>>& groups)
// {
//     std::string products;
//     std::vector<std::string> cellsBinaryNumbers;

//     for (const auto& group : groups)
//     {
//         for (const auto& cell : group)
//         {
//             cellsBinaryNumbers.push_back(grayCode[cell.second] + grayCode[cell.first]);
//         }

//         // for (const auto& cell : cellsBinaryNumbers)
//         // {
//         //     std::cout << cell << std::endl;
//         // }

//         products = getProduct(cellsBinaryNumbers);
//         algebraicMinterms.push_back(products);
//         cellsBinaryNumbers.clear();
//     }
// }

// std::string Map4x4::getProduct(std::vector<std::string>& cellsBinaryNumbers)
// {
//     int8_t ones = 0;
//     std::string product = "";

//     for (int8_t i = 0; i < cellsBinaryNumbers.size(); ++i)
//     {
//         for (const auto& number : cellsBinaryNumbers)
//         {
//             if (number[i] == '1')
//                 ++ones; 
//         }

//         if (ones == cellsBinaryNumbers.size())
//         {
//             product += variables[i];
//             ones = 0;
//             continue;
//         }

//         if (ones == 0)
//         {
//             product += variables[i] + '\'';
//         }

//         ones = 0;
//     }
    
//     return product;
// }


// void Map4x4::printKmap() const
// {
//     for (const auto& row : kmap)
//     {
//         for (const auto& elem : row)
//         {
//             std::cout << static_cast<char>(elem) << " ";
//         }
//         std::cout << std::endl;
//     }
// }

// void Map4x4::printSquare2x2Groups() const
// {
//     for (const auto& group : square2x2Groups)
//     {
//         for (const auto& elem : group)
//         {
//             std::cout << "(" << (int)elem.first << ", " << (int)elem.second << ")" << " ";
//         }
//         std::cout << std::endl;
//     }
// }

// void Map4x4::printRect4x2Group() const
// {
//     for (const auto& group : rect4x2Groups)
//     {
//         for (const auto& elem : group)
//         {
//             std::cout << "(" << (int)elem.first << ", " << (int)elem.second << ")" << " ";
//         }
//         std::cout << std::endl;
//     }
// }

// void Map4x4::printRect4x1Group() const
// {
//     for (const auto& group : rect4x1Groups)
//     {
//         for (const auto& elem : group)
//         {
//             std::cout << "(" << (int)elem.first << ", " << (int)elem.second << ")" << " ";
//         }
//         std::cout << std::endl;
//     }
// }

// void Map4x4::printRect2x1Group() const
// {
//     for (const auto& group : rect2x1Groups)
//     {
//         for (const auto& elem : group)
//         {
//             std::cout << "(" << (int)elem.first << ", " << (int)elem.second << ")" << " ";
//         }
//         std::cout << std::endl;
//     }
// }

// void Map4x4::print1x1Group() const
// {
//     for (const auto& elem : _1x1Groups)
//     {
//         std::cout << "(" << (int)elem.first << ", " << (int)elem.second << ")" << " ";
//     }
//     std::cout << std::endl;
// }

// void Map4x4::printCellsNumberOfGroups()
// {
//     for (const auto& num : cellsNumberOfGroups)
//     {
//         std::cout << (int)num << " ";
//     }
//     std::cout << std::endl;
// }

// void Map4x4::initializeKmap()
// {
//     for (auto& row : kmap)
//     {
//         for (auto& elem : row)
//         {
//             elem = Value::zero;
//         }
//     }
// }

// void Map4x4::initializeKmapWith(std::array<std::array<Value, 4>, 4>& kmap)
// {
//     this->kmap = kmap;
// }

// void Map4x4::countZeroesAndOnes()
// {
//     for (auto& row : kmap)
//     {
//         for (auto& elem : row)
//         {
//             if (elem == Value::zero)
//                 ++zeroes;
//             if (elem == Value::one)
//                 ++ones;
//         }
//     }
// }

// void Map4x4::printMinterms() const
// {
//     for (const auto& product : algebraicMinterms)
//     {
//         std::cout << product << "+";
//     }
//     std::cout << std::endl;
// }

// std::array<std::array<Value, 4>, 4>& Map4x4::getKmap()
// {
//     return kmap;
// }

// Value Map4x4::getCellValue(int row, int col)
// {
//     if (row < 0 or row > 3)
//         return Value::zero;
    
//     if (col < 0 or col > 3)
//         return Value::zero;

//     return kmap[row][col];
// }

// bool Map4x4::changeCellValue(int row, int col, Value value)
// {
//     if (row < 0 or row > 3)
//         return false;
    
//     if (col < 0 or col > 3)
//         return false;

//     kmap[row][col] = value;

//     return true;
// }

// int8_t Map4x4::getZeroes()
// {
//     return zeroes;
// }

// std::vector<std::pair<int8_t, int8_t>>& Map4x4::get1x1Groups()
// {
//     return _1x1Groups; 
// }

// std::vector<std::array<std::pair<int8_t, int8_t>, 2>>& Map4x4::get2x1Groups()
// {
//     return rect2x1Groups;
// }

// std::vector<std::array<std::pair<int8_t, int8_t>, 4>>& Map4x4::get4x1Groups()
// {
//     return rect4x1Groups;
// }

// std::vector<std::array<std::pair<int8_t, int8_t>, 4>>& Map4x4::get2x2Groups()
// {
//     return square2x2Groups;
// }

// std::vector<std::array<std::pair<int8_t, int8_t>, 8>>& Map4x4::get4x2Groups()
// {
//     return rect4x2Groups;
// }

// std::vector<std::string>& Map4x4::getAlgebraicMinterms()
// {
//     return algebraicMinterms;
// }
