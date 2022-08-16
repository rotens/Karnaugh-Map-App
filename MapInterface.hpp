#pragma once
#include <SFML/Graphics.hpp>
#include "Map4x4.hpp"


class MapInterface
{
    sf::RenderWindow window;
    sf::RectangleShape rectangles[16];
    sf::RectangleShape rectangles4x1Groups[4];
    sf::RectangleShape mapBorder;
    sf::Text cellValues[16];
    sf::Text grayCodeText[8];
    sf::Text variablesText[2];
    sf::Text algebraicMintermsText[17];
    sf::RectangleShape truthTableCells[102];
    sf::Text truthTableHeaderText[6];
    sf::Text truthTableVariablesValues[64];
    Map4x4 kmapObject;
    int currentColorIndex{0};
    sf::CircleShape truthTableStateCircle[16];
    sf::Font& font;

    void fillCellsWithWhiteColor();
    void cellHover(sf::Event::MouseMoveEvent& mouseMove);
    void handleMouseButtonPressed(sf::Event::MouseButtonEvent& mouseButtonEvent);
    void drawMap();
    void drawCellValues();
    void drawGrayCode();
    void drawVariables();
    void draw4x4Group();
    void drawSquareQuads();
    void drawSingleGroups();
    void drawPairs();
    void drawRectQuads();
    void drawOctets();
    void drawGroups();
    void drawAlgebraicMinterms();
    void drawTruthTable();
    void handleMouseButtonPressedOnTruthTable(
        sf::Event::MouseButtonEvent& mouseButtonEvent);
    void performMap4x4Minimizing();
    void incrementCurrentColorIndex();

public:
    MapInterface(sf::Font&);
    void loop();

};
