#include "Map2x4.hpp"
#include "Kmap2x4Cell.hpp"
#include <numeric>

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
            if (kmap[getRealIndex(cellIndex + offset)]->getCellValue() == Value::one)
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

        for (const auto offset : {1, 4, 5})
        {   
            quad.push_back(getRealIndex(cellIndex + offset));
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
        squareQuads.push_back(std::move(quad));
        oneValues = 0;
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
