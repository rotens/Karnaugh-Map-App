#include "Map4x4.hpp"
#include <tuple>

void Map4x4::solve()
{
    countZeroesAndOnes();
    findGroups();
    findAlgebraicMinterms<8>(rect4x2Groups);
    findAlgebraicMinterms<4>(rect4x1Groups);
    findAlgebraicMinterms<4>(square2x2Groups);
    findAlgebraicMinterms<2>(rect2x1Groups);
    // findAlgebraicMinterms(rect4x2Groups);
}

void Map4x4::findGroups()
{
    findRect4x2Groups();
    findRect4x1Groups();
    findSquare2x2Groups();
    findRect2x1Groups();
    find1x1Groups();
    removeRedundantGroups();
}

void Map4x4::findRect4x2Groups()
{
    for (int8_t i = 0; i < 4; ++i)
    {
        if (isGroup(i, 0, 2, 4))
        {
            addRect4x2Group(i, 0, 2, 4);

            if (not isAnyCellNotInGroupExistent())
                return;
        }
    }

    for (int8_t i = 0; i < 4; ++i)
    {
        if (isGroup(0, i, 4, 2))
        {
            addRect4x2Group(0, i, 4, 2);

            if (not isAnyCellNotInGroupExistent())
                 return;
        }
    }
}

void Map4x4::addRect4x2Group(int8_t row, int8_t col, int8_t height, int8_t width)
{
    int8_t k = 0;
    std::array<std::pair<int8_t, int8_t>, 8> rect4x2Group;

    for (int8_t i = row; i < row+height; ++i)
    {
        for (int8_t j = col; j < col+width; ++j)
        {
            rect4x2Group[k++] = getRealIndices(i, j);
            cellsInGroup[translateIndices(i, j)] = 1;
            ++cellsNumberOfGroups[translateIndices(i, j)];
        }
    }

    rect4x2Groups.push_back(std::move(rect4x2Group));
}

void Map4x4::findSquare2x2Groups() 
{
    for (int8_t i = 0; i < 4; ++i)
    {
        for (int8_t j = 0; j < 4; ++j) 
        {
            if (isGroup(i, j, 2, 2) and not isContainedInGroup<8>(rect4x2Groups, i, j, 2, 2))
            {
                addSquare2x2Group(i, j);

                if (not isAnyCellNotInGroupExistent())
                    return;
            }
        }
    }
}

void Map4x4::addSquare2x2Group(int8_t row, int8_t col)
{
    int8_t k = 0;
    std::array<std::pair<int8_t, int8_t>, 4> newGroup;

    for (int8_t i = row; i < row+2; ++i)
    {
        for (int8_t j = col; j < col+2; ++j)
        {
            newGroup[k++] = getRealIndices(i, j);
            cellsInGroup[translateIndices(i, j)] = 1;
            ++cellsNumberOfGroups[translateIndices(i, j)];
        }
    }
    
    square2x2Groups.push_back(std::move(newGroup));
}

void Map4x4::findRect4x1Groups()
{
    for (int8_t i = 0; i < 4; ++i)
    {
        /* Horizontal group */
        if (isGroup(i, 0, 1, 4) and not is4x1GroupContainedIn4x2Group(i, 0, 1, 4))
        {
            addRect4x1Group(i, 0, 1, 4);

            if (not isAnyCellNotInGroupExistent())
                return;
        }
    }

    for (int8_t i = 0; i < 4; ++i)
    {
        /* Vertical group */
        if (isGroup(0, i, 4, 1) and not is4x1GroupContainedIn4x2Group(0, i, 4, 1))
        {
            addRect4x1Group(0, i, 4, 1);

            if (not isAnyCellNotInGroupExistent())
                return;
        }
    }
}

void Map4x4::addRect4x1Group(int8_t row, int8_t col, int8_t height, int8_t width)
{
    int8_t k = 0;
    std::array<std::pair<int8_t, int8_t>, 4> newGroup;

    for (int8_t i = row; i < row+height; ++i) 
    {
        for (int8_t j = col; j < col+width; ++j)
        {
            newGroup[k++] = getRealIndices(i, j);
            cellsInGroup[translateIndices(i, j)] = 1;
            ++cellsNumberOfGroups[translateIndices(i, j)];
        }
    }

    rect4x1Groups.push_back(std::move(newGroup));
}

void Map4x4::findRect2x1Groups()
{
    for (int8_t i = 0; i < 4; ++i)
    {
        for (int8_t j = 0; j < 4; ++j) 
        {
            if (isGroup(i, j, 1, 2)
                and not isContainedInGroup<8>(rect4x2Groups, i, j, 1, 2)
                and not isContainedInGroup<4>(square2x2Groups, i, j, 1, 2)
                and not isContainedInGroup<4>(rect4x1Groups, i, j, 1, 2))
            {
                addRect2x1Group(i, j, 1, 2);

                if (not isAnyCellNotInGroupExistent())
                    return;
            }

            if (isGroup(i, j, 2, 1)
                and not isContainedInGroup<8>(rect4x2Groups, i, j, 2, 1)
                and not isContainedInGroup<4>(square2x2Groups, i, j, 2, 1)
                and not isContainedInGroup<4>(rect4x1Groups, i, j, 2, 1))
            {
                addRect2x1Group(i, j, 2, 1);

                if (not isAnyCellNotInGroupExistent())
                    return;
            }
        }
    }
}

void Map4x4::addRect2x1Group(int8_t row, int8_t col, int8_t height, int8_t width)
{
    int8_t k = 0;
    std::array<std::pair<int8_t, int8_t>, 2> newGroup;

    for (int8_t i = row; i < row+height; ++i) 
    {
        for (int8_t j = col; j < col+width; ++j)
        {
            newGroup[k++] = getRealIndices(i, j);
            cellsInGroup[translateIndices(i, j)] = 1;
            ++cellsNumberOfGroups[translateIndices(i, j)];
        }
    }

    rect2x1Groups.push_back(std::move(newGroup));
}

void Map4x4::find1x1Groups()
{
    for (int8_t i = 0; i < 4; ++i) 
    {
        for (int8_t j = 0; j < 4; ++j)
        {
            if (is1x1Group(i, j))
            {
                add1x1Group(i, j);

                if (not isAnyCellNotInGroupExistent())
                    return;
            }
        }
    }
}

bool Map4x4::is1x1Group(int8_t row, int8_t col)
{
    int8_t x, y;

    if (kmap[row][col] != Value::one)
        return false;

    for (int8_t i : {-1, 1})
    {
        std::tie(x, y) = getRealIndices(row+i, col);
        if (kmap[x][y] == Value::one or kmap[x][y] == Value::dont_care)
            return false;

        std::tie(x, y) = getRealIndices(row, col+i);
        if (kmap[x][y] == Value::one or kmap[x][y] == Value::dont_care)
            return false;
    }

    return true;
}

void Map4x4::add1x1Group(int8_t row, int8_t col)
{
    _1x1Groups.push_back(std::make_pair(row, col));
    cellsInGroup[translateIndices(row, col)] = 1;
    ++cellsNumberOfGroups[translateIndices(row, col)];
}

bool Map4x4::isGroup(int8_t row, int8_t col, int8_t height, int8_t width)
{
    int8_t x, y;
    int8_t dontCares = 0;

    for (int8_t i = row; i < row+height; ++i)
    {
        for (int8_t j = col; j < col+width; ++j)
        {
            std::tie(x, y) = getRealIndices(i, j);

            if (kmap[x][y] != Value::one) 
            {
                if (kmap[x][y] != Value::dont_care)
                {
                    return false;
                }
                    
                ++dontCares;
            }
        }
    }

    if (dontCares == width*height)
        return false;
    
    return true;
}

bool Map4x4::is4x1GroupContainedIn4x2Group(int8_t row, int8_t col, int8_t height, int8_t width)
{
    int8_t matches = 0, k = 0;
    std::array<std::pair<int8_t, int8_t>, 4> groupCells;

    for (int8_t i = row; i < row+height; ++i)
    {
        for (int8_t j = col; j < col+width; ++j)
        {
            groupCells[k++] = std::make_pair(i, j);
        }
    }

    for (const auto& group : rect4x2Groups)
    {
        for (const auto& elem : group) 
        {
            for (const auto& cell : groupCells)
            {
                if (elem == cell)
                    ++matches;
            }
        }

        if (matches == 4)
            return true;

        matches = 0;
    }

    return false;
}

template<int N> 
bool Map4x4::isContainedInGroup(
    std::vector<std::array<std::pair<int8_t, int8_t>, N>>& groups, 
    int8_t row, int8_t col, int8_t height, int8_t width)
{
    int8_t matches = 0;
    auto firstCell = std::pair<int8_t, int8_t>(row, col);
    auto lastCell = getRealIndices(row+height-1, col+width-1);

    for (const auto& group : groups)
    {
        for (const auto& elem : group) 
        {
            if (elem == firstCell)
                ++matches;

            if (elem == lastCell)
                ++matches;
        }

        if (matches == 2)
            return true;

        matches = 0;
    }

    return false;
}

bool Map4x4::isAnyCellNotInGroupExistent()
{
    auto sum = std::accumulate(cellsInGroup.begin(), cellsInGroup.end(), 0);
    return sum == ones ? false : true;
}

void Map4x4::removeRedundantGroups()
{
    removeRedundantGroupsFromGivenVector<4, VecIter<4>>(square2x2Groups);
    removeRedundantGroupsFromGivenVector<4, VecIter<4>>(rect4x1Groups);
    removeRedundantGroupsFromGivenVector<2, VecIter<2>>(rect2x1Groups);
}

template<int N, typename T> 
void Map4x4::removeRedundantGroupsFromGivenVector(
    std::vector<std::array<std::pair<int8_t, int8_t>, N>>& groups)
{
    std::vector<T> iters;
    bool hasCellInOnlyOneGroup = false;
    int8_t index;

    for (auto it = groups.begin(); it != groups.end(); ++it)
    {
        for (const auto& cell : *it)
        {
            index = translateIndices(cell.first, cell.second);
            if (cellsNumberOfGroups[index] == 1)
            {
                hasCellInOnlyOneGroup = true;
                break;
            }
        }

        if (not hasCellInOnlyOneGroup)
        {
            iters.push_back(it);
            decrementNumberOfGroups<N>(*it);
        }
    }

    for (const auto& iter : iters)
        groups.erase(iter);
}

template<int N>
void Map4x4::decrementNumberOfGroups(std::array<std::pair<int8_t, int8_t>, N>& group)
{
    int8_t index;

    for (const auto& cell : group)
    {
        index = translateIndices(cell.first, cell.second);
        --cellsNumberOfGroups[index];
    }
}

template<int N>
void Map4x4::findAlgebraicMinterms(std::vector<std::array<std::pair<int8_t, int8_t>, N>>& groups)
{
    std::string products;
    std::vector<std::string> cellsBinaryNumbers;

    for (const auto& group : groups)
    {
        for (const auto& cell : group)
        {
            cellsBinaryNumbers.push_back(grayCode[cell.second] + grayCode[cell.first]);
        }

        for (const auto& cell : cellsBinaryNumbers)
        {
            std::cout << cell << std::endl;
        }

        products = getProduct(cellsBinaryNumbers);
        algebraicMinterms.push_back(products);
    }
}

std::string Map4x4::getProduct(std::vector<std::string>& cellsBinaryNumbers)
{
    int8_t ones = 0;
    std::string product = "";

    for (int8_t i = 0; i < cellsBinaryNumbers.size(); ++i)
    {
        for (const auto& number : cellsBinaryNumbers)
        {
            if (number[i] == '1')
                ++ones; 
        }
        std::cout << (int)ones << std::endl;
        if (ones == cellsBinaryNumbers.size())
        {
            product += variables[i];
            ones = 0;
            continue;
        }

        if (ones == 0)
        {
            product += variables[i] + '\'';
        }

        ones = 0;
    }
    
    return product;
}


void Map4x4::printKmap() const
{
    for (const auto& row : kmap)
    {
        for (const auto& elem : row)
        {
            std::cout << static_cast<char>(elem) << " ";
        }
        std::cout << std::endl;
    }
}

void Map4x4::printSquare2x2Groups() const
{
    for (const auto& group : square2x2Groups)
    {
        for (const auto& elem : group)
        {
            std::cout << "(" << (int)elem.first << ", " << (int)elem.second << ")" << " ";
        }
        std::cout << std::endl;
    }
}

void Map4x4::printRect4x2Group() const
{
    for (const auto& group : rect4x2Groups)
    {
        for (const auto& elem : group)
        {
            std::cout << "(" << (int)elem.first << ", " << (int)elem.second << ")" << " ";
        }
        std::cout << std::endl;
    }
}

void Map4x4::printRect4x1Group() const
{
    for (const auto& group : rect4x1Groups)
    {
        for (const auto& elem : group)
        {
            std::cout << "(" << (int)elem.first << ", " << (int)elem.second << ")" << " ";
        }
        std::cout << std::endl;
    }
}

void Map4x4::printRect2x1Group() const
{
    for (const auto& group : rect2x1Groups)
    {
        for (const auto& elem : group)
        {
            std::cout << "(" << (int)elem.first << ", " << (int)elem.second << ")" << " ";
        }
        std::cout << std::endl;
    }
}

void Map4x4::print1x1Group() const
{
    for (const auto& elem : _1x1Groups)
    {
        std::cout << "(" << (int)elem.first << ", " << (int)elem.second << ")" << " ";
    }
    std::cout << std::endl;
}

void Map4x4::printCellsNumberOfGroups()
{
    for (const auto& num : cellsNumberOfGroups)
    {
        std::cout << (int)num << " ";
    }
    std::cout << std::endl;
}

void Map4x4::initializeKmap()
{
    for (auto& row : kmap)
    {
        for (auto& elem : row)
        {
            elem = Value::zero;
        }
    }
}

void Map4x4::initializeKmapWith(std::array<std::array<Value, 4>, 4>& kmap)
{
    this->kmap = kmap;
}

void Map4x4::countZeroesAndOnes()
{
    for (auto& row : kmap)
    {
        for (auto& elem : row)
        {
            if (elem == Value::zero)
                ++zeroes;
            if (elem == Value::one)
                ++ones;
        }
    }
}

void Map4x4::printMinterms() const
{
    for (const auto& product : algebraicMinterms)
    {
        std::cout << product << "+";
    }
    std::cout << std::endl;
}
