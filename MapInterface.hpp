#pragma once
#include <SFML/Graphics.hpp>
#include "Map4x4.hpp"


class MapInterface
{
    sf::RenderWindow window;
    sf::RectangleShape rectangles[16];
    sf::RectangleShape rectangles2x1Group[5];
    sf::RectangleShape rectangles4x1Groups[4];
    sf::RectangleShape rectangle4x2Group;
    sf::Text cellValues[16];
    sf::Text grayCodeText[8];
    sf::Text variablesText[2];
    Map4x4 kmapObject;

    void fillCellsWithWhiteColor();
    void cellHover(sf::Event::MouseMoveEvent& mouseMove);
    void handleMouseButtonPressed(sf::Event::MouseButtonEvent& mouseButtonEvent);
    void drawMap();
    void drawCellValues();
    void drawGrayCode();
    void drawVariables();
    void draw4x4Groups();
    void draw2x2Groups();
    void draw1x1Groups();
    void draw2x1Groups();
    void draw4x1Groups();
    void draw4x2Groups();

public:
    MapInterface(sf::Font&, Map4x4& kmapObject);
    void loop();

};
