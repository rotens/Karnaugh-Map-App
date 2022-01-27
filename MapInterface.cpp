#include "MapInterface.hpp"
#include <iostream>

constexpr int windowWidth = 800;
constexpr int windowHeight = 640;
constexpr int mapWidthOffset = 500;
constexpr int mapHeightOffset = 197;
constexpr int textRelativeHeightOffset = 19 - 10;
constexpr int textRelativeWidthOffset = 23 - 3;

MapInterface::MapInterface(sf::Font& font, Map4x4 kmapObject)
    : kmapObject(kmapObject)
{
    for (auto& text : cellValues)
        text.setFont(font);
}

void MapInterface::fillCellsWithWhiteColor()
{
    for (auto& cell : rectangles)
    {
        cell.setFillColor(sf::Color::White);
    }
}

void MapInterface::drawMap()
{
    int k = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            rectangles[k].setSize(sf::Vector2f(58, 58.f));
            rectangles[k].setOutlineColor(sf::Color(127, 127, 127));
            rectangles[k].setOutlineThickness(2.f);
            rectangles[k].setPosition(
                mapWidthOffset + j * 60, 
                mapHeightOffset + i * 60);

            window.draw(rectangles[k]);

            cellValues[k].setString(static_cast<char>(kmapObject.getCellValue(i, j)));
            cellValues[k].setCharacterSize(30); 
            cellValues[k].setFillColor(sf::Color::Black);
            cellValues[k].setPosition(
                mapWidthOffset + textRelativeWidthOffset + j * 60, 
                mapHeightOffset + textRelativeHeightOffset + i * 60);
            window.draw(cellValues[k]);
            ++k;
        }
    }
}

void MapInterface::cellHover(sf::Event::MouseMoveEvent& mouseMove)
{
    if (mouseMove.x <= mapWidthOffset or mouseMove.x >= mapWidthOffset + 4*58 + 6)
    {
        fillCellsWithWhiteColor();
        return;
    } 

    if (mouseMove.y <= mapHeightOffset or mouseMove.y >= mapHeightOffset + 4*58 + 6)
    {
        fillCellsWithWhiteColor();
        return;
    }

    int col = (mouseMove.x - mapWidthOffset) / 60;
    int row = (mouseMove.y - mapHeightOffset) / 60;
    int index = row*4 + col;

    fillCellsWithWhiteColor();
    rectangles[index].setFillColor(sf::Color(170, 215, 255));
    // window.draw(rectangles[index]);
}

void MapInterface::handleMouseButtonPressed(sf::Event::MouseButtonEvent& mouseButtonEvent)
{
    if (mouseButtonEvent.x <= mapWidthOffset or mouseButtonEvent.x >= mapWidthOffset + 4*58 + 6)
    {
        return;
    } 

    if (mouseButtonEvent.y <= mapHeightOffset or mouseButtonEvent.y >= mapHeightOffset + 4*58 + 6)
    {
        return;
    }

    int col = (mouseButtonEvent.x - mapWidthOffset) / 60;
    int row = (mouseButtonEvent.y - mapHeightOffset) / 60;
    int index = row*4 + col;

    if (kmapObject.getCellValue(row, col) == Value::one)
    {
        kmapObject.changeCellValue(row, col, Value::zero);
        return;
    }

    kmapObject.changeCellValue(row, col, Value::one);
}

void MapInterface::loop()
{
    window.create(sf::VideoMode(windowWidth, windowHeight), "Karnaugh map simulator!");
    

    // sf::RectangleShape rectangle(sf::Vector2f(242, 242.f));
    // window.clear(sf::Color::White);
    // rectangle.setFillColor(sf::Color::White);
    // rectangle.setOutlineThickness(2.f);
    // rectangle.setOutlineColor(sf::Color(127, 127, 127));
    // rectangle.setPosition(500, mapHeightOffset);

    
    // sf::FloatRect textRect = cellValues[0].getLocalBounds();
    // std::cout << textRect.left << " " << textRect.top << std::endl;
    // std::cout << textRect.width << " " << textRect.height << std::endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) 
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    handleMouseButtonPressed(event.mouseButton);
                    std::cout << "Test" << std::endl;
                    break;
                case sf::Event::MouseMoved:
                    cellHover(event.mouseMove);
                    break;

            }

        }

        window.clear(sf::Color::White);
        drawMap();
        window.display();
    }
}
