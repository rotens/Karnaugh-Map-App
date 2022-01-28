#pragma once
#include <SFML/Graphics.hpp>
#include "Map4x4.hpp"


class MapInterface
{
    sf::RenderWindow window;
    sf::RectangleShape rectangles[16];
    sf::Text cellValues[16];
    sf::Text grayCodeText[8];
    Map4x4 kmapObject;

    void fillCellsWithWhiteColor();
    void cellHover(sf::Event::MouseMoveEvent& mouseMove);
    void handleMouseButtonPressed(sf::Event::MouseButtonEvent& mouseButtonEvent);
    void drawMap();
    void drawGrayCode();

public:
    MapInterface(sf::Font&, Map4x4& kmapObject);
    void loop();

};
