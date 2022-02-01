#pragma once
#include <SFML/Graphics.hpp>
#include "Map4x4.hpp"


class MapInterface
{
    sf::RenderWindow window;
    sf::RectangleShape rectangles[16];
    sf::RectangleShape rectangles4x1Groups[4];
    sf::RectangleShape rectangle4x2Group;
    sf::Text cellValues[16];
    sf::Text grayCodeText[8];
    sf::Text variablesText[2];
    sf::Text algebraicMintermsText[17];
    sf::RectangleShape truthTableCells[102];
    sf::Text truthTableHeaderText[6];
    sf::Text truthTableVariablesValues[64];
    Map4x4 kmapObject;
    int currentColorIndex{0};

    void fillCellsWithWhiteColor();
    void cellHover(sf::Event::MouseMoveEvent& mouseMove);
    void handleMouseButtonPressed(sf::Event::MouseButtonEvent& mouseButtonEvent);
    void drawMap();
    void drawCellValues();
    void drawGrayCode();
    void drawVariables();
    void draw4x4Group();
    void draw2x2Groups();
    void draw1x1Groups();
    void draw2x1Groups();
    void draw4x1Groups();
    void draw4x2Groups();
    void drawGroups();
    void drawAlgebraicMinterms();
    void drawTruthTable();

public:
    MapInterface(sf::Font&, Map4x4& kmapObject);
    void loop();

};
