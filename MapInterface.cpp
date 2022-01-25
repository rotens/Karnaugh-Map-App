#include "MapInterface.h"
#include <iostream>

constexpr int windowWidth = 800;
constexpr int windowHeight = 640;
constexpr int mapWidthOffset = 500;
constexpr int mapHeightOffset = 197;
constexpr int textRelativeHeightOffset = 19 - 10;
constexpr int textRelativeWidthOffset = 23 - 3;

MapInterface::MapInterface(sf::Font& font) 
{
    for (auto& text : cellValues)
        text.setFont(font);
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

            cellValues[k].setString("0");
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

void MapInterface::loop()
{
    window.create(sf::VideoMode(windowWidth, windowHeight), "Karnaugh map simulator!");
    

    // sf::RectangleShape rectangle(sf::Vector2f(242, 242.f));
    // window.clear(sf::Color::White);
    // rectangle.setFillColor(sf::Color::White);
    // rectangle.setOutlineThickness(2.f);
    // rectangle.setOutlineColor(sf::Color(127, 127, 127));
    // rectangle.setPosition(500, mapHeightOffset);

    
    // set the string to display
    cellValues[0].setString("0");

    // set the character size
    cellValues[0].setCharacterSize(30); // in pixels, not points!

    // set the color
    cellValues[0].setFillColor(sf::Color::Black);
    cellValues[0].setPosition(mapWidthOffset + textRelativeWidthOffset, mapHeightOffset + textRelativeHeightOffset);
    // sf::FloatRect textRect = cellValues[0].getLocalBounds();
    // std::cout << textRect.left << " " << textRect.top << std::endl;
    // std::cout << textRect.width << " " << textRect.height << std::endl;

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

        window.clear(sf::Color::White);
        drawMap();
        window.draw(cellValues[0]);
        window.display();
    }
}
