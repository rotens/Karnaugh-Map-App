#pragma once
#include <SFML/Graphics.hpp>


class MapInterface
{
    void drawMap();
    sf::RenderWindow window;
    sf::RectangleShape rectangles[16];
    sf::Text cellValues[16];
    void fillCellsWithWhiteColor();
    void cellHover(sf::Event::MouseMoveEvent& mouseMove);
public:
    MapInterface(sf::Font&);
    void loop();

};
