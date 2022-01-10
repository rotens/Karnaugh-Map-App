#include <array>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>

#define __TEST__ std::cout<<"TEST"<<std::endl;

enum class Value { zero = '0', one = '1', dont_care = 'X' };

std::pair<int8_t, int8_t> getRealIndices(int8_t, int8_t);
int8_t translateIndices(int8_t, int8_t);

class Map4x4
{
    std::array<std::array<Value, 4>, 4> kmap;
    int8_t zeroes = 0;
    int8_t ones = 0;
    std::vector<std::array<std::pair<int8_t, int8_t>, 4>> square2x2Groups;
    std::vector<std::array<std::pair<int8_t, int8_t>, 4>> rect4x1Groups;
    std::vector<std::array<std::pair<int8_t, int8_t>, 8>> rect4x2Groups;
    std::vector<std::array<std::pair<int8_t, int8_t>, 2>> rect2x1Groups;
    std::array<int8_t, 16> cellsInGroup{};

    void findSquare2x2Groups();
    void findRect4x2Groups();
    void findRect4x1Groups();
    void findRect2x1Groups();
    bool isGroup(int8_t, int8_t, int8_t, int8_t);
    void addSquare2x2Group(int8_t, int8_t);
    void addRect4x2Group(int8_t, int8_t, int8_t, int8_t);
    void addRect4x1Group(int8_t, int8_t, int8_t, int8_t);
    void addRect2x1Group(int8_t, int8_t, int8_t, int8_t);
    bool isContainedIn4x2Group(int8_t, int8_t, int8_t, int8_t);
    bool is4x1GroupContainedIn4x2Group(int8_t, int8_t, int8_t, int8_t);
    bool isContainedIn2x2Group(int8_t, int8_t, int8_t, int8_t);
    bool isContainedIn4x1Group(int8_t, int8_t, int8_t, int8_t);
    template<int N> bool isContainedInGroup(
        std::vector<std::array<std::pair<int8_t, int8_t>, N>>&, 
        int8_t, int8_t, int8_t, int8_t);
    bool isAnyCellNotInGroupExistent();

public:
    void printKmap() const;
    void initializeKmap();
    void initializeKmapWith(std::array<std::array<Value, 4>, 4>&);
    void printGroup() const;
    void printSquare2x2Groups() const;
    void printRect4x2Group() const;
    void printRect4x1Group() const;
    void printRect2x1Group() const;
    void countZeroesAndOnes();
    void findGroups();
};

void Map4x4::findGroups()
{
    findRect4x2Groups();
    findRect4x1Groups();
    findSquare2x2Groups();
    findRect2x1Groups();
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
            if (isGroup(i, j, 2, 2) and not isContainedIn4x2Group(i, j, 2, 2))
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
            newGroup[k++] = std::make_pair(i, j);
            cellsInGroup[translateIndices(i, j)] = 1;
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
                and not isContainedIn4x2Group(i, j, 1, 2)
                and not isContainedIn2x2Group(i, j, 1, 2)
                and not isContainedIn4x1Group(i, j, 1, 2))
            {
                addRect2x1Group(i, j, 1, 2);

                if (not isAnyCellNotInGroupExistent())
                    return;
            }

            if (isGroup(i, j, 2, 1)
                and not isContainedIn4x2Group(i, j, 2, 1)
                and not isContainedIn2x2Group(i, j, 2, 1)
                and not isContainedIn4x1Group(i, j, 2, 1))
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
            newGroup[k++] = std::make_pair(i, j);
            cellsInGroup[translateIndices(i, j)] = 1;
        }
    }

    rect2x1Groups.push_back(std::move(newGroup));
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

bool Map4x4::isContainedIn4x2Group(int8_t row, int8_t col, int8_t height, int8_t width)
{
    int8_t matches = 0;
    auto firstCell = std::pair<int8_t, int8_t>(row, col);
    auto lastCell = getRealIndices(row+height-1, col+width-1);

    for (const auto& group : rect4x2Groups)
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

bool Map4x4::isContainedIn2x2Group(int8_t row, int8_t col, int8_t height, int8_t width)
{
    int8_t matches = 0;
    auto firstCell = std::pair<int8_t, int8_t>(row, col);
    auto lastCell = getRealIndices(row+height-1, col+width-1);

    for (const auto& group : square2x2Groups)
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

bool Map4x4::isContainedIn4x1Group(int8_t row, int8_t col, int8_t height, int8_t width)
{
    int8_t matches = 0;
    auto firstCell = std::pair<int8_t, int8_t>(row, col);
    auto lastCell = getRealIndices(row+height-1, col+width-1);

    for (const auto& group : rect4x1Groups)
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

int main()
{
    // sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    // sf::CircleShape shape(100.f);
    // shape.setFillColor(sf::Color::Green);
    
    std::array<std::array<Value, 4>, 4> testKmap = {{
        {Value::one, Value::one, Value::one, Value::one},
        {Value::one, Value::one, Value::zero, Value::zero},
        {Value::one, Value::one, Value::zero, Value::zero},
        {Value::zero, Value::zero, Value::zero, Value::zero}
    }};

    Map4x4 kmapObject;
    kmapObject.initializeKmapWith(testKmap);
    kmapObject.countZeroesAndOnes();
    kmapObject.printKmap();
    kmapObject.findGroups();
    kmapObject.printSquare2x2Groups();
    kmapObject.printRect4x2Group();
    kmapObject.printRect4x1Group(); 
    kmapObject.printRect2x1Group(); 

    // kmap.printKmap();
    // kmap.printGroup();
    // kmap.moveGroup(-2, -2);
    // kmap.printGroup();
    // std::cout << -1 % 4 << std::endl;

    // while (window.isOpen())
    // {
    //     sf::Event event;
    //     while (window.pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed)
    //             window.close();
    //         if (event.type == sf::Event::MouseButtonPressed)
    //             std::cout << "Test" << std::endl;
    //     }

    //     window.clear();
    //     window.draw(shape);
    //     window.display();
    // }

    return 0;
}