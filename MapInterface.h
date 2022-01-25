#pragma once
#include <SFML/Graphics.hpp>


class MapInterface
{
    void drawMap();
    sf::RenderWindow window;
    sf::RectangleShape rectangles[16];
    sf::Text cellValues[16];
public:
    MapInterface(sf::Font&);
    void loop();

};
