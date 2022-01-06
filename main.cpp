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
    std::array<std::pair<uint8_t, uint8_t>, 4> group{{ {1, 1}, {2, 1}, {1, 2}, {2, 2} }};
    void setNewIndexes(std::pair<uint8_t, uint8_t>&, int8_t, int8_t);

public:
    void printKmap() const;
    void initializeKmap();
    void moveGroup(int8_t, int8_t);
    void printGroup() const;
};

void Map4x4::moveGroup(int8_t row, int8_t col)
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

void Map4x4::printGroup() const
{
    for (auto& indexes : group)
    {
        std::cout << "(" << (int)indexes.first << ", " << (int)indexes.second << ")" << "\n";
    }
}

void Map4x4::printKmap() const
{
    for (const auto& row : kmap)
    {
        for (const auto& elem : row)
        {
            std::cout << static_cast<char>(elem);
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

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    

    Map4x4 kmap;
    kmap.initializeKmap();
    kmap.printKmap();
    kmap.printGroup();
    kmap.moveGroup(-2, -2);
    kmap.printGroup();
    std::cout << -1 % 4 << std::endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
                std::cout << "Test" << std::endl;
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}