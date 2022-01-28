#pragma once
#include <SFML/Graphics.hpp>
#include "Map4x4.hpp"


class MapInterface
{
    sf::RenderWindow window;
    sf::RectangleShape rectangles[16];
    sf::RectangleShape rectangle4x2Group;
    sf::Text cellValues[16];
    sf::Text grayCodeText[8];
    sf::Text variablesText[2];
    Map4x4 kmapObject;

    void fillCellsWithWhiteColor();
    void cellHover(sf::Event::MouseMoveEvent& mouseMove);
    void handleMouseButtonPressed(sf::Event::MouseButtonEvent& mouseButtonEvent);
    void drawMap();
    void drawGrayCode();
    void drawVariables();
    void draw4x2Group();
    void draw1x1Group();

public:
    MapInterface(sf::Font&, Map4x4& kmapObject);
    void loop();

};
