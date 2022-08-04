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
        
        if (*valuesIt == Value::one)
            ++ones; 

        ++valuesIt;
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

    findingPairsWithSharingDone = true;
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
            decrementGroupingPossibilities();

            std::cout << "PAIRING WITHOUT SHARING" << std::endl;
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

            decrementGroupingPossibilities();
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
    cell->setDone();
    for (auto quadedCellIndex : possibleQuad)
    {
        kmap[quadedCellIndex]->setDone();
    }
    
    std::vector<int> quad{possibleQuad};
    quad.insert(quad.begin(), cell->getIndex());
    justGroupedCells.insert(quad.begin(), quad.end());
    squareQuads.push_back(std::move(quad));
}

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
    cell->setDone();
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
        
        // cell->setDone();

        if (cell->getRectQuadsNumber() == 1)
        {
            rectQuadCells(cell);
        }
        else
        {
            squareQuadCells(cell);
        }
        
        quadFound1 = true;
        decrementGroupingPossibilities();
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

    findingQuadsWithSharingDone = true;
}

void Map4x4::quadCellsWithTwoPossibilities()
{
    for (auto& cell : kmap)
    {
        if (cell->isDone()) continue;
    
        if (cell->getQuadsNumber() != 2) continue;
        // std::cout << "index " << cell->getIndex() << std::endl;
        if (cell->getRectQuadsNumber() == 1 or cell->getRectQuadsNumber() == 2)
        {
            rectQuadCells(cell);
        }
        else
        {
            squareQuadCells(cell);
        }

        decrementGroupingPossibilities();
        quadFound2 = true;

        std::cout << "QUADING CELL= " << cell->getIndex() << std::endl;
    }
}

void Map4x4::repeatQuadingCellsWithTwoPossibilities()
{
    do
    {
        quadFound2 = false;
        quadCellsWithTwoPossibilities();
        repeatQuadingCellsWithOnePossibility();
    } while (quadFound2);
}

void Map4x4::repeatQuadingCellsWithOnePossibility()
{
    do
    {
        quadFound1 = false;
        quadCellsWithOnePossibility();
    } while (quadFound1);
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
            auto index = cell->getIndexOfRectQuadWithMinimalSharing() - 1;
            addRectQuadWithSharing(cell, index);
        } 
        else if (cell->getNumberOfSquareQuadsWithSharing() == 2)
        {
            auto index = cell->getIndexOfSquareQuadWithMinimalSharing() - 1;
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
               std::cout << "QUADING CELL= " << cell->getIndex() << std::endl;

        decrementGroupingPossibilities();
        quadFound2 = true;
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
                
                cell->removeSquareQuadContainingGivenCellIndex(cellIndex);
                cell->removeRectQuadContainingGivenCellIndex(cellIndex);
                // if (cell->getSquareQuadsNumber() == 0)
                // {
                //     cell->removeRectQuadContainingGivenCellIndex(cellIndex);
                //     continue;
                // }

                // if (cell->getRectQuadsNumber() == 0)
                // {
                //     cell->removeSquareQuadContainingGivenCellIndex(cellIndex);
                //     continue;
                // }

                // // TODO
                // if (cell->getRectQuadsNumber() == 1 and cell->getSquareQuadsNumber() == 1)
                // {
                //     cell->removeSquareQuadContainingGivenCellIndex(cellIndex);
                //     continue;
                // }

                // if (cell->getRectQuadsNumber() == 2 and cell->getSquareQuadsNumber() == 1)
                // {

                //     cell->removeSquareQuadContainingGivenCellIndex(cellIndex);
                //     continue;
                // }

                // if (cell->getRectQuadsNumber() >= 1 and cell->getSquareQuadsNumber() >= 1)
            }
        } 
    }

    justGroupedCells.clear();
}

void Map4x4::decrementGroupingPossibilities2(std::vector<int>& groupedCellsIndices)
{
    for (auto cellIndex : groupedCellsIndices)
    {
        if (isCellDone(cellIndex)) continue;

        for (auto& cell : kmap)
        {
            if (cell->isDone() or cell->getCellValue() == Value::zero) 
                continue;

            if (cell->getPairsNumber() > 1)
            {
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
}

void Map4x4::setGroupingPossibilitiesToTwo()
{
    for (const auto cell : kmap)
    {
        if (cell->getQuadsNumber() > 2)
        {
            if (cell->getRectQuadsNumber() == 0)
            {
                cell->setSquareQuadsNumber(2);
                break;
            }

            if (cell->getSquareQuadsNumber() == 0)
            {
                cell->setRectQuadsNumber(2);
                break;
            }
            
            if (cell->getRectQuadsNumber() == 2)
            {
                cell->setSquareQuadsNumber(0);
                break;
            }

            if (cell->getRectQuadsNumber() > 2)
            {
                cell->setRectQuadsNumber(2);
                cell->setSquareQuadsNumber(0);
                break;
            }

            if (cell->getRectQuadsNumber() == 1)
            {
                cell->setSquareQuadsNumber(1);
            }

            break;
        }
    }
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
    // std::cout << "ONES=" << ones << std::endl;
    if (ones == 0)
    {
        // algebraicMinterms.push_back("0");
        return;
    }

    // STEP 1
    findHorizontalOctets();
    if (hasAllCellsGrouped()) return;

    findVerticalOctets();
    if (hasAllCellsGrouped()) return;

    // STEP 2
    findPossiblePairs();
    pairCells(1);
    if (hasAllCellsGrouped()) return;

    // STEP 3
    findPossibleQuads();

    int iter = 1;
    while (true)
    {
        std::cout << "ITERATION " << iter++ << "\n";
        quadCellsWithOnePossibility();
        if (hasAllCellsGrouped()) return;

        pairCells(1);
        if (hasAllCellsGrouped()) return;

        quadCellsWithOnePossibility();
        if (hasAllCellsGrouped()) return;

        // STEP 4
        repeatQuadingCellsWithTwoPossibilities();
        if (hasAllCellsGrouped()) return;

        // STEP 5
        findPossibleQuadsWithSharing();
        quadCellsWithTwoPossibilitiesAndWithSharing();
        if (hasAllCellsGrouped()) return;

        // STEP 6
        repeatPairingCellsWithTwoPossibilities();
        if (hasAllCellsGrouped()) return;

        // STEP 7
        findPossiblePairsWithSharing();
        repeatPairingCellsWithTwoPossibilitiesAndSharing();
        if (hasAllCellsGrouped()) return;

        if (hasCellsWithMoreThanTwoPossibilitiesToQuad())
        {
            setGroupingPossibilitiesToTwo();
        }
        else
        {
            break;
        }
    } 
    
    // STEP 8a
    findSingleGroups();
}

bool Map4x4::hasAllCellsGrouped()
{
    int groupedCells = std::count_if(
        kmap.begin(), kmap.end(), [](KmapCell* cell) { return cell->isDone(); });
    return groupedCells == ones;
}

bool Map4x4::hasCellsWithMoreThanTwoPossibilitiesToQuad()
{
    for (auto cell : kmap)
    {
        if (cell->getQuadsNumber() > 2)
            return true;
    }

    return false;
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
