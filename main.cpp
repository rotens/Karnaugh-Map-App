#include <array>
#include <cstdint>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>

#define __TEST__ std::cout<<"TEST"<<std::endl;

enum class Value { zero = '0', one = '1', dont_care = 'X' };

std::pair<int8_t, int8_t> getRealIndices(int8_t, int8_t);

class Map4x4
{
    std::array<std::array<Value, 4>, 4> kmap;
    int8_t zeroes = 16;
    std::vector<std::array<std::pair<int8_t, int8_t>, 4>> square2x2Groups;
    std::array<std::pair<int8_t, int8_t>, 8> rect4x2Group;

    // void setNewIndexes(std::pair<int8_t, int8_t>&, int8_t, int8_t);
    void findSquare2x2Groups();
    void findRect4x2Groups();
    // void moveGroup(std::array<std::pair<int8_t, int8_t>, 4>&, int8_t, int8_t);
    bool isSquare2x2Group(int8_t, int8_t);
    void addSquare2x2GroupToVector(int8_t, int8_t);
    bool isRect4x2VerticalGroup(int8_t, int8_t);
    bool isRect4x2HorizontalGroup(int8_t, int8_t);
    void addRect4x2Group(int8_t, int8_t, int8_t, int8_t);

public:
    void printKmap() const;
    void initializeKmap();
    void initializeKmapWith(std::array<std::array<Value, 4>, 4>&);
    void printGroup() const;
    void printSquare2x2Groups() const;
    void printRect4x2Group() const;
    void findGroups();
};

void Map4x4::findGroups()
{
    findRect4x2Groups();
    //findSquare2x2Groups();
}

void Map4x4::findRect4x2Groups()
{
    for (int8_t i = 0; i < 4; ++i)
    {
        if (isRect4x2HorizontalGroup(i, 0))
        {
            addRect4x2Group(i, 0, 2, 4);
        }
    }

    for (int8_t i = 0; i < 4; ++i)
    {
        if (isRect4x2VerticalGroup(0, i))
        {
            addRect4x2Group(0, i, 4, 2);
        }
    }
}

void Map4x4::addRect4x2Group(int8_t row, int8_t col, int8_t height, int8_t width)
{
    int8_t k = 0;

    for (int8_t i = row; i < row+height; ++i)
    {
        for (int8_t j = col; j < col+width; ++j)
        {
            rect4x2Group[k++] = getRealIndices(i, j);
        }
    }
}

bool Map4x4::isRect4x2HorizontalGroup(int8_t row, int8_t col)
{
    int8_t x, y;
    int8_t dontCares = 0;

    for (int8_t i = row; i < row+2; ++i)
    {
        for (int8_t j = col; j <= col+4; ++j)
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

    if (dontCares == 8)
        return false;
    
    return true;
}

bool Map4x4::isRect4x2VerticalGroup(int8_t row, int8_t col)
{
    int8_t x, y;
    int8_t dontCares = 0;

    for (int8_t i = row; i < row+4; ++i)
    {
        for (int8_t j = col; j < col+2; ++j)
        {
            std::tie(x, y) = getRealIndices(i, j);

            if (kmap[x][y] != Value::one) 
            {
                if (kmap[x][y] != Value::dont_care)
                {
                    __TEST__
                    return false;
                }
                    
                ++dontCares;
            }
        }
    }

    if (dontCares == 8)
        return false;
    
    return true;
}

void Map4x4::findSquare2x2Groups() 
{
    for (int8_t i = 0; i < 4; ++i)
    {
        for (int8_t j = 0; j < 4; ++j) 
        {
            if (isSquare2x2Group(i, j))
            {
                addSquare2x2GroupToVector(i, j);
            }
        }
    }
}

bool Map4x4::isSquare2x2Group(int8_t row, int8_t col)
{
    int8_t x, y;
    int8_t dontCares = 0;

    for (int8_t i = row; i <= row+1; ++i)
    {
        for (int8_t j = col; j <= col+1; ++j)
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

    if (dontCares == 4)
        return false;
    
    return true;
}

void Map4x4::addSquare2x2GroupToVector(int8_t row, int8_t col)
{
    std::array<std::pair<int8_t, int8_t>, 4> newGroup;
    newGroup[0] = getRealIndices(row, col);
    newGroup[1] = getRealIndices(row, col+1);
    newGroup[2] = getRealIndices(row+1, col);
    newGroup[3] = getRealIndices(row+1, col+1);
    square2x2Groups.push_back(std::move(newGroup));
}

// void Map4x4::moveGroup(std::array<std::pair<int8_t, int8_t>, 4>& group, int8_t row, int8_t col)
// {
//     row %= 4;
//     col %= 4;

//     for (auto& indexes : group)
//     {
//         setNewIndexes(indexes, row, col);
//     }
// }

// void Map4x4::setNewIndexes(std::pair<int8_t, int8_t>& indexes, int8_t row, int8_t col)
// {
//     if (indexes.first + row < 0)
//         indexes.first += 4 + row;
//     else if (indexes.first + row > 3)
//         indexes.first = (indexes.first + row) % 4;
//     else 
//         indexes.first += row; 

//     if (indexes.second + col < 0)
//         indexes.second += 4 + col;
//     else if (indexes.second + col > 3)
//         indexes.second = (indexes.second + col) % 4;
//     else 
//         indexes.second += col; 
// }

// void Map4x4::printGroup() const
// {
//     for (auto& indexes : group)
//     {
//         std::cout << "(" << (int)indexes.first << ", " << (int)indexes.second << ")" << "\n";
//     }
// }

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
    for (const auto& elem : rect4x2Group)
    {
        std::cout << "(" << (int)elem.first << ", " << (int)elem.second << ")" << " ";
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

int main()
{
    // sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    // sf::CircleShape shape(100.f);
    // shape.setFillColor(sf::Color::Green);
    

    std::array<std::array<Value, 4>, 4> testKmap = {{
        {Value::one, Value::one, Value::zero, Value::zero},
        {Value::one, Value::one, Value::zero, Value::zero},
        {Value::one, Value::one, Value::zero, Value::zero},
        {Value::one, Value::one, Value::zero, Value::zero}
    }};

    Map4x4 kmapObject;
    kmapObject.initializeKmapWith(testKmap);
    kmapObject.printKmap();
    kmapObject.findGroups();
    // kmapObject.printSquare2x2Groups();
    kmapObject.printRect4x2Group();
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