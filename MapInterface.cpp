#include "MapInterface.h"
#include <iostream>

constexpr int windowWidth = 800;
constexpr int windowHeight = 640;
constexpr int mapWidthOffset = 0;
constexpr int mapHeightOffset = 197;


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
            rectangles[k++].setPosition(
                500, mapHeightOffset);
        }
    }
}

void MapInterface::loop()
{
    window.create(sf::VideoMode(windowWidth, windowHeight), "Karnaugh map simulator!");
    sf::RectangleShape rectangle(sf::Vector2f(242, 242.f));
    window.clear(sf::Color::White);
    rectangle.setFillColor(sf::Color::White);
    rectangle.setOutlineThickness(2.f);
    rectangle.setOutlineColor(sf::Color(127, 127, 127));
    rectangle.setPosition(500, mapHeightOffset);




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
        window.draw(rectangle);
        window.draw(rectangles[0]);
        window.display();
    }
}
