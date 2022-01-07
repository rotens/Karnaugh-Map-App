#include <array>
#include <cstdint>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>

enum class Value { zero = '0', one = '1', dont_care = 'X' };

class Map4x4
{
    std::array<std::array<Value, 4>, 4> kmap;
    uint8_t zeroes = 16;
    std::vector<std::array<std::pair<uint8_t, uint8_t>, 4>> square2x2Groups;
    std::vector<std::array<std::pair<uint8_t, uint8_t>, 8>> rect4x2Groups;

    void setNewIndexes(std::pair<uint8_t, uint8_t>&, int8_t, int8_t);
    void findSquare2x2Groups();
    void findRect4x2Groups();
    void moveGroup(std::array<std::pair<uint8_t, uint8_t>, 4>&, int8_t, int8_t);
    bool isGroup(std::array<std::pair<uint8_t, uint8_t>, 4>&);

public:
    void printKmap() const;
    void initializeKmap();
    void initializeKmapWith(std::array<std::array<Value, 4>, 4>&);
    void printGroup() const;
    void printSquare2x2Groups() const;
    void findGroups();
};

void Map4x4::findRect4x2Groups()
{
    std::array<std::pair<uint8_t, uint8_t>, 12> rect4x2GroupsIndexes {{
        {0, 0}, {1, 3}, {0, 0}, {3, 1}, {0, 2}, {3, 3},
        {1, 0}, {2, 2}, {2, 0}, {3, 3}, {0, 1}, {3, 2}
    }};

    // TODO
}

void Map4x4::findSquare2x2Groups() 
{
    std::array<std::pair<uint8_t, uint8_t>, 4> group{{ 
        {3, 3}, {0, 3}, {3, 0}, {0, 0} 
    }};

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (isGroup(group))
            {
                square2x2Groups.push_back(group);
            }
            moveGroup(group, 0, 1);
        }
        moveGroup(group, 1, 0);
    }
}

bool Map4x4::isGroup(std::array<std::pair<uint8_t, uint8_t>, 4>& group)
{
    for (const auto& elem : group)
    {
        if (kmap[elem.first][elem.second] != Value::one 
            and kmap[elem.first][elem.second] != Value::dont_care)
        {
            return false;
        }
    }

    return true;
}

void Map4x4::moveGroup(std::array<std::pair<uint8_t, uint8_t>, 4>& group, int8_t row, int8_t col)
{
    row %= 4;
    col %= 4;

    for (auto& indexes : group)
    {
        setNewIndexes(indexes, row, col);
    }
}

void Map4x4::setNewIndexes(std::pair<uint8_t, uint8_t>& indexes, int8_t row, int8_t col)
{
    if (indexes.first + row < 0)
        indexes.first += 4 + row;
    else if (indexes.first + row > 3)
        indexes.first = (indexes.first + row) % 4;
    else 
        indexes.first += row; 

    if (indexes.second + col < 0)
        indexes.second += 4 + col;
    else if (indexes.second + col > 3)
        indexes.second = (indexes.second + col) % 4;
    else 
        indexes.second += col; 
}

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

int main()
{
    // sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    // sf::CircleShape shape(100.f);
    // shape.setFillColor(sf::Color::Green);
    

    std::array<std::array<Value, 4>, 4> testKmap = {{
        {Value::zero, Value::one, Value::one, Value::zero},
        {Value::one, Value::zero, Value::zero, Value::one},
        {Value::one, Value::zero, Value::zero, Value::one},
        {Value::zero, Value::one, Value::one, Value::zero}
    }};

    Map4x4 kmapObject;
    kmapObject.initializeKmapWith(testKmap);
    kmapObject.printKmap();
    // kmapObject.findSquare2x2Groups();
    // kmapObject.printSquare2x2Groups();
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