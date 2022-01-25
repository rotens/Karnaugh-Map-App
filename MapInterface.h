#pragma once
#include <SFML/Graphics.hpp>


class MapInterface
{
    void drawMap();
    sf::RenderWindow window;
    sf::RectangleShape rectangles[16];
public:
    void loop();

};
