#include "MapInterface.hpp"
#include "other.hpp"
#include <iostream>

constexpr int windowWidth = 800;
constexpr int windowHeight = 640;
constexpr int mapWidthOffset = 500;
constexpr int mapHeightOffset = 197;
constexpr int textRelativeHeightOffset = 19 - 10;
constexpr int textRelativeWidthOffset = 24 - 3;
constexpr int horizontalGrayCodeHeightOffset = mapHeightOffset - 34;
constexpr int horizontalGrayCodeWidthOffset = mapWidthOffset + 17 - 1; // 1 = text's left bound
constexpr int verticalGrayCodeHeightOffset = mapHeightOffset + 21 - 8;
constexpr int verticalGrayCodeWidthOffset = mapWidthOffset - 34; // 1 = text's left bound
constexpr int ABVariablesWidthOffset = mapWidthOffset + 101;
constexpr int ABVariablesHeightOffset = mapHeightOffset - 68;
constexpr int CDVariablesWidthOffset = mapWidthOffset - 74;
constexpr int CDVariablesHeightOffset = mapHeightOffset + 138;
sf::Color colors[8] = {
    sf::Color::Red, sf::Color::Blue, 
    sf::Color::Cyan, sf::Color::Green,
    sf::Color::Magenta};

MapInterface::MapInterface(sf::Font& font, Map4x4& kmapObject)
    : kmapObject(kmapObject)
{
    for (auto& text : cellValues)
        text.setFont(font);

    for (auto& text : grayCodeText)
        text.setFont(font);

    variablesText[0].setFont(font);
    variablesText[1].setFont(font);
}

void MapInterface::fillCellsWithWhiteColor()
{
    for (auto& cell : rectangles)
    {
        cell.setFillColor(sf::Color::White);
    }
}

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
            rectangles[k].setPosition(
                mapWidthOffset + j * 60, 
                mapHeightOffset + i * 60);

            window.draw(rectangles[k]);
            ++k;
        }
    }
}

void MapInterface::drawCellValues()
{
    int k = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            cellValues[k].setString(static_cast<char>(kmapObject.getCellValue(i, j)));
            cellValues[k].setCharacterSize(30); 
            cellValues[k].setFillColor(sf::Color::Black);
            cellValues[k].setPosition(
                mapWidthOffset + textRelativeWidthOffset + j * 60, 
                mapHeightOffset + textRelativeHeightOffset + i * 60);

            window.draw(cellValues[k]);
            ++k;
        }
    }
}

void MapInterface::drawGrayCode()
{
    for (int i = 0; i < 4; ++i)
    {
        grayCodeText[i].setString(grayCode[i]);
        grayCodeText[i].setCharacterSize(24); 
        grayCodeText[i].setFillColor(sf::Color::Black);
        grayCodeText[i].setPosition(
            horizontalGrayCodeWidthOffset + i * 60,
            horizontalGrayCodeHeightOffset);

        window.draw(grayCodeText[i]);

        grayCodeText[i+4].setString(grayCode[i]);
        grayCodeText[i+4].setCharacterSize(24); 
        grayCodeText[i+4].setFillColor(sf::Color::Black);
        grayCodeText[i+4].setPosition(
            verticalGrayCodeWidthOffset,
            verticalGrayCodeHeightOffset + i * 60);

        window.draw(grayCodeText[i+4]);
    }
}

void MapInterface::drawVariables()
{
    variablesText[0].setString("AB");
    variablesText[0].setCharacterSize(30); 
    variablesText[0].setFillColor(sf::Color::Black);
    variablesText[0].setPosition(
        ABVariablesWidthOffset,
        ABVariablesHeightOffset);

    window.draw(variablesText[0]);

    variablesText[1].setString("CD");
    variablesText[1].setCharacterSize(30); 
    variablesText[1].setFillColor(sf::Color::Black);
    variablesText[1].setRotation(-90.f);
    variablesText[1].setPosition(
        CDVariablesWidthOffset,
        CDVariablesHeightOffset);

    window.draw(variablesText[1]);
}

void MapInterface::draw4x2Group()
{
    if (kmapObject.getZeroes() != 0)
        return;
    
    rectangle4x2Group.setOutlineColor(sf::Color::Red);
    rectangle4x2Group.setOutlineThickness(2.f);
    rectangle4x2Group.setFillColor(sf::Color::Transparent);
    rectangle4x2Group.setSize(sf::Vector2f(238, 238));
    rectangle4x2Group.setPosition(sf::Vector2f(mapWidthOffset, mapHeightOffset));

    window.draw(rectangle4x2Group);
}

void MapInterface::draw2x1Group()
{
    int width;
    int height;
    int k = 0;

    for (const auto& group : kmapObject.get2x1Groups())
    {
        auto [firstCell, lastCell] = group;

        if (firstCell.first == lastCell.first)
        {
            width = 2;
            height = 1;
        }
        else
        {
            width = 1;
            height = 2;
        }
        
        rectangles2x1Group[k].setOutlineColor(colors[k]);
        rectangles2x1Group[k].setOutlineThickness(2.f);
        rectangles2x1Group[k].setFillColor(sf::Color::Transparent);
        rectangles2x1Group[k].setSize(
            sf::Vector2f(
                58*width + (width-1)*2, 
                58*height + (height-1)*2));
        rectangles2x1Group[k].setPosition(
            sf::Vector2f(
                mapWidthOffset + 60*firstCell.second, 
                mapHeightOffset + 60*firstCell.first));

        window.draw(rectangles2x1Group[k]);
        k++;
    }
}

void MapInterface::draw1x1Group()
{
    int index;
    int k = 0;

    for (const auto& group : kmapObject.get1x1Groups())
    {
        index = group.first*4 + group.second;
        rectangles[index].setOutlineColor(colors[k++]);
        window.draw(rectangles[index]);
    }
}

void MapInterface::cellHover(sf::Event::MouseMoveEvent& mouseMove)
{
    if (mouseMove.x <= mapWidthOffset or mouseMove.x >= mapWidthOffset + 4*58 + 6)
    {
        fillCellsWithWhiteColor();
        return;
    } 

    if (mouseMove.y <= mapHeightOffset or mouseMove.y >= mapHeightOffset + 4*58 + 6)
    {
        fillCellsWithWhiteColor();
        return;
    }

    int col = (mouseMove.x - mapWidthOffset) / 60;
    int row = (mouseMove.y - mapHeightOffset) / 60;
    int index = row*4 + col;

    fillCellsWithWhiteColor();
    rectangles[index].setFillColor(sf::Color(170, 215, 255));
}

void MapInterface::handleMouseButtonPressed(sf::Event::MouseButtonEvent& mouseButtonEvent)
{
    if (mouseButtonEvent.x <= mapWidthOffset or mouseButtonEvent.x >= mapWidthOffset + 4*58 + 6)
    {
        return;
    } 

    if (mouseButtonEvent.y <= mapHeightOffset or mouseButtonEvent.y >= mapHeightOffset + 4*58 + 6)
    {
        return;
    }

    int col = (mouseButtonEvent.x - mapWidthOffset) / 60;
    int row = (mouseButtonEvent.y - mapHeightOffset) / 60;
    int index = row*4 + col;

    if (kmapObject.getCellValue(row, col) == Value::one)
    {
        kmapObject.changeCellValue(row, col, Value::zero);
        return;
    }

    kmapObject.changeCellValue(row, col, Value::one);
}

void MapInterface::loop()
{
    window.create(sf::VideoMode(windowWidth, windowHeight), "Karnaugh map simulator!");
    

    // sf::RectangleShape rectangle(sf::Vector2f(242, 242.f));
    // window.clear(sf::Color::White);
    // rectangle.setFillColor(sf::Color::White);
    // rectangle.setOutlineThickness(2.f);
    // rectangle.setOutlineColor(sf::Color(127, 127, 127));
    // rectangle.setPosition(500, mapHeightOffset);

    
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) 
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    handleMouseButtonPressed(event.mouseButton);
                    break;
                case sf::Event::MouseMoved:
                    cellHover(event.mouseMove);
                    break;
            }

        }

        window.clear(sf::Color::White);
        drawMap();
        drawGrayCode();
        drawVariables();
        draw4x2Group();
        draw1x1Group();
        draw2x1Group();
        drawCellValues();
        window.display();
    }
}
