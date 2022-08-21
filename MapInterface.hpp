#pragma once
#include <SFML/Graphics.hpp>
#include "Map4x4.hpp"
#include "Map2x4.hpp"


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
    Map2x4 kmap2x4Object;
    int currentColorIndex{0};
    sf::CircleShape truthTableStateCircle[16];

    int currentMapWidth;
    int currentMapHeight;
    int currentVariables1WidthOffset;
    int currentVariables1HeightOffset;
    int currentVariables2WidthOffset;
    int currentVariables2HeightOffset;
    std::string currentVariables1;
    std::string currentVariables2;
    MapType currentMapType;
    Groups& const octets;
    Groups& const squareQuads;
    Groups& const rectQuads;
    Groups& const pairs;
    std::vector<int>& const singleGroups;

    void fillCellsWithWhiteColor();
    void cellHover(sf::Event::MouseMoveEvent& mouseMove);
    void handleMouseButtonPressed(sf::Event::MouseButtonEvent& mouseButtonEvent);
    void drawMap();
    void drawCellValues();
    void drawGrayCode();
    void drawGrayCodeMap4x4();
    void drawGrayCodeMap2x4();
    void drawVariables();
    void drawMapBorder();
    void drawSquareQuads();
    void drawSingleGroups();
    void drawPairs();
    void drawRectQuads();
    void drawOctets();
    void drawGroups();
    void drawMap4x4Groups();
    void drawMap2x4Groups();
    void drawAlgebraicMinterms();
    void drawTruthTable();
    void handleMouseButtonPressedOnTruthTable(
        sf::Event::MouseButtonEvent& mouseButtonEvent);
    void performMap4x4Minimizing();
    void incrementCurrentColorIndex();
    Value getCellValue(int cellIndex);
    void setMap2x4Variables();
    void setMap4x4Variables();

public:
    MapInterface(sf::Font&);
    void loop();

};
