#include "MapInterface.hpp"
#include "other.hpp"
#include <iostream>
#include <cmath>
#include <bitset>

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
constexpr int mintermsWidthOffset = mapWidthOffset - 74;
constexpr int mintermsHeightOffset = mapHeightOffset + 240 + 50;
constexpr int truthTableWidthOffset = 70;
constexpr int truthTableHeightOffset = 80; 
constexpr char truthTableHeader[] = {'A', 'B', 'C', 'D', '0', '1'};
sf::Color colors[8] = {
    sf::Color::Red, sf::Color::Blue, 
    sf::Color::Cyan, sf::Color::Green,
    sf::Color::Magenta};
constexpr std::array<std::pair<int, int>, 4> diffs{{{0, 0}, {0, 1}, {1, 0}, {1, 1}}};
constexpr int truthTableToKmap[] = {0, 4, 12, 8, 1, 5, 13, 9, 3, 7, 15, 11, 2, 6, 14, 10};
constexpr Value intToValue[] = {Value::zero, Value::one, Value::dont_care};

MapInterface::MapInterface(sf::Font& font, Map4x4& kmapObject)
    : kmapObject(kmapObject)
{
    for (auto& text : cellValues)
        text.setFont(font);

    for (auto& text : grayCodeText)
        text.setFont(font);

    for (auto& text : algebraicMintermsText)
        text.setFont(font);
    
    for (auto& text : truthTableHeaderText)
        text.setFont(font);

    for (auto& text : truthTableVariablesValues)
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

void MapInterface::draw4x4Group()
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

void MapInterface::draw4x2Groups()
{
    int width;
    int height;
    int longerLineWidth;
    int longerLineHeight;
    int shorterLinesWidth;
    int shorterLinesHeight;
    std::pair<int8_t, int8_t> firstCell;
    std::pair<int8_t, int8_t> fourthCell;
    std::pair<int8_t, int8_t> lastCell;

    for (const auto& group : kmapObject.get4x2Groups())
    {
        firstCell = group[0];
        fourthCell = group[3];
        lastCell = group[7];

        if (firstCell.first == fourthCell.first)
        {
            width = 1;
            height = 0;
            longerLineWidth = 242;
            longerLineHeight = 2;
            shorterLinesWidth = 2;
            shorterLinesHeight = 62;
        }
        else
        {
            width = 0;
            height = 1;
            longerLineWidth = 2;
            longerLineHeight = 242;
            shorterLinesWidth = 62;
            shorterLinesHeight = 2;
        }

        sf::RectangleShape lines[6];

        // 1

        lines[0].setSize(sf::Vector2f(longerLineWidth, longerLineHeight));
        lines[0].setFillColor(colors[currentColorIndex]);
        lines[0].setPosition(
            (mapWidthOffset-2) + firstCell.second * 60,
            (mapHeightOffset-2) + firstCell.first * 60
        );
        window.draw(lines[0]);
        
        lines[1].setSize(sf::Vector2f(shorterLinesWidth, shorterLinesHeight));
        lines[1].setFillColor(colors[currentColorIndex]);
        lines[1].setPosition(
            (mapWidthOffset-2) + firstCell.second * 60,
            (mapHeightOffset-2) + firstCell.first * 60
        );
        window.draw(lines[1]);

        lines[2].setSize(sf::Vector2f(shorterLinesWidth, shorterLinesHeight));
        lines[2].setFillColor(colors[currentColorIndex]);
        lines[2].setPosition(
            (mapWidthOffset-2) + firstCell.second*60 + 4*60*width,
            (mapHeightOffset-2) + firstCell.first*60 + 4*60*height
        );
        window.draw(lines[2]);

        // 2

        lines[3].setSize(sf::Vector2f(longerLineWidth, longerLineHeight));
        lines[3].setFillColor(colors[currentColorIndex]);
        lines[3].setPosition(
            (mapWidthOffset-2) + lastCell.second * 60 + 62,
            (mapHeightOffset-2) + lastCell.first * 60 + 62
        );
        lines[3].setRotation(180.f);
        window.draw(lines[3]);
        
        lines[4].setSize(sf::Vector2f(shorterLinesWidth, shorterLinesHeight));
        lines[4].setFillColor(colors[currentColorIndex]);
        lines[4].setPosition(
            (mapWidthOffset-2) + lastCell.second * 60 + 62,
            (mapHeightOffset-2) + lastCell.first * 60 + 62
        );
        lines[4].setRotation(180.f);
        window.draw(lines[4]);

        lines[5].setSize(sf::Vector2f(shorterLinesWidth, shorterLinesHeight));
        lines[5].setFillColor(colors[currentColorIndex]);
        lines[5].setPosition(
            (mapWidthOffset-2) + lastCell.second * 60 + 62 - 4*60*width,
            (mapHeightOffset-2) + lastCell.first * 60 + 62 - 4*60*height
        );
        lines[5].setRotation(180.f);
        window.draw(lines[5]);
      
       ++currentColorIndex;
    }
}

void MapInterface::draw2x2Groups()
{
    for (const auto& group : kmapObject.get2x2Groups())
    {
        int l = 0;

        for (const auto& cell : group) 
        {
            sf::RectangleShape lines[2];

            lines[0].setSize(sf::Vector2f(62, 2));
            lines[0].setFillColor(colors[currentColorIndex]);
            lines[0].setPosition(
                (mapWidthOffset-2) + (cell.second) * 60,
                (mapHeightOffset-2) + (cell.first + diffs[l].first) * 60
            );
            window.draw(lines[0]);
            
            lines[1].setSize(sf::Vector2f(2, 62));
            lines[1].setFillColor(colors[currentColorIndex]);
            lines[1].setPosition(
                (mapWidthOffset-2) + (cell.second + diffs[l].second) * 60,
                (mapHeightOffset-2) + cell.first * 60
            );
            window.draw(lines[1]);

            ++l;
        }

        ++currentColorIndex;
    }
}

void MapInterface::draw4x1Groups()
{
    int width;
    int height;
    int k = 0;
    std::pair<int8_t, int8_t> firstCell;
    std::pair<int8_t, int8_t> lastCell;

    for (const auto& group : kmapObject.get4x1Groups())
    {
        firstCell = group[0];
        lastCell  = group[3];

        if (firstCell.first == lastCell.first)
        {
            width = 4;
            height = 1;
        }
        else
        {
            width = 1;
            height = 4;
        }
        
        rectangles4x1Groups[k].setOutlineColor(colors[currentColorIndex]);
        rectangles4x1Groups[k].setOutlineThickness(2.f);
        rectangles4x1Groups[k].setFillColor(sf::Color::Transparent);
        rectangles4x1Groups[k].setSize(
            sf::Vector2f(
                58*width + (width-1)*2, 
                58*height + (height-1)*2));
        rectangles4x1Groups[k].setPosition(
            sf::Vector2f(
                mapWidthOffset + 60*firstCell.second, 
                mapHeightOffset + 60*firstCell.first));

        window.draw(rectangles4x1Groups[k]);
        ++k;
        ++currentColorIndex;
    }
}

void MapInterface::draw2x1Groups()
{
    int width;
    int height;

    for (const auto& group : kmapObject.get2x1Groups())
    {
        auto [firstCell, lastCell] = group;

        if (firstCell.first == lastCell.first)
        {
            width = 1;
            height = 0;
        }
        else
        {
            width = 0;
            height = 1;
        }

        sf::RectangleShape lines[6];

        // 1

        lines[0].setSize(sf::Vector2f(62, 2));
        lines[0].setFillColor(colors[currentColorIndex]);
        lines[0].setPosition(
            (mapWidthOffset-2) + firstCell.second * 60,
            (mapHeightOffset-2) + firstCell.first * 60
        );
        window.draw(lines[0]);

        lines[1].setSize(sf::Vector2f(2, 62));
        lines[1].setFillColor(colors[currentColorIndex]);
        lines[1].setPosition(
            (mapWidthOffset-2) + firstCell.second * 60,
            (mapHeightOffset-2) + firstCell.first * 60
        );

        window.draw(lines[1]);

        lines[2].setSize(sf::Vector2f(2 + 60*width, 2 + 60*height));
        lines[2].setFillColor(colors[currentColorIndex]);
        lines[2].setPosition(
            (mapWidthOffset-2) + firstCell.second * 60 + 60*height,
            (mapHeightOffset-2) + firstCell.first * 60 + 60*width
        );
        window.draw(lines[2]);

        // 2

        lines[3].setSize(sf::Vector2f(62, 2));
        lines[3].setFillColor(colors[currentColorIndex]);
        lines[3].setPosition(
            (mapWidthOffset-2) + lastCell.second * 60 + 62,
            (mapHeightOffset-2) + lastCell.first * 60 + 62
        );
        lines[3].setRotation(180.f);
        window.draw(lines[3]);

        lines[4].setSize(sf::Vector2f(2, 62));
        lines[4].setFillColor(colors[currentColorIndex]);
        lines[4].setPosition(
            (mapWidthOffset-2) + lastCell.second * 60 + 62,
            (mapHeightOffset-2) + lastCell.first * 60 + 62
        );
        lines[4].setRotation(180.f);
        window.draw(lines[4]);

        lines[5].setSize(sf::Vector2f(2 + 60*width, 2 + 60*height));
        lines[5].setFillColor(colors[currentColorIndex]);
        lines[5].setPosition(
            (mapWidthOffset-2) + lastCell.second * 60 + 62 - 60*height,
            (mapHeightOffset-2) + lastCell.first * 60 + 62 - 60*width
        );
        lines[5].setRotation(180.f);
        window.draw(lines[5]);
       
        ++currentColorIndex;
    }
}

void MapInterface::draw1x1Groups()
{
    int index;

    for (const auto& group : kmapObject.get1x1Groups())
    {
        index = group.first*4 + group.second;
        rectangles[index].setOutlineColor(colors[currentColorIndex++]);
        window.draw(rectangles[index]);
    }
}

void MapInterface::drawGroups() 
{
    currentColorIndex = 0;
    draw4x4Group();
    draw4x2Groups();
    draw4x1Groups();
    draw2x2Groups();
    draw2x1Groups();
    draw1x1Groups();
}

void MapInterface::drawAlgebraicMinterms()
{
    int i = 1;
    int colorIndex = 0;
    int currentWidthOffset = mintermsWidthOffset;

    algebraicMintermsText[0].setString("y = ");
    algebraicMintermsText[0].setCharacterSize(24);
    algebraicMintermsText[0].setFillColor(sf::Color::Black);
    algebraicMintermsText[0].setPosition(mintermsWidthOffset, mintermsHeightOffset);
    window.draw(algebraicMintermsText[0]);

    sf::FloatRect bounds = algebraicMintermsText[0].getLocalBounds();
    currentWidthOffset += bounds.width;

    for (const auto& minterm : kmapObject.getAlgebraicMinterms())
    {
        algebraicMintermsText[i].setString(minterm);
        algebraicMintermsText[i].setCharacterSize(24);
        algebraicMintermsText[i].setFillColor(colors[colorIndex]);
        algebraicMintermsText[i].setPosition(currentWidthOffset, mintermsHeightOffset);
        window.draw(algebraicMintermsText[i]);

        bounds = algebraicMintermsText[i].getLocalBounds();
        currentWidthOffset += bounds.width + 8;
        ++i;

        algebraicMintermsText[i].setString("+");
        algebraicMintermsText[i].setCharacterSize(24);
        algebraicMintermsText[i].setFillColor(sf::Color::Black);
        algebraicMintermsText[i].setPosition(currentWidthOffset, mintermsHeightOffset);
        window.draw(algebraicMintermsText[i]);

        bounds = algebraicMintermsText[i].getLocalBounds();
        currentWidthOffset += bounds.width + 12;
        ++i;

        ++colorIndex;
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

void MapInterface::drawTruthTable()
{
    int index;
    int value = 0;
    char bit;
    sf::FloatRect bounds;

    for (int i = 0; i < 17; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            index = i*6 + j;

            truthTableCells[index].setSize(sf::Vector2f(26, 26.f));
            truthTableCells[index].setOutlineColor(sf::Color(127, 127, 127));
            truthTableCells[index].setOutlineThickness(2.f);
            truthTableCells[index].setPosition(
                truthTableWidthOffset + j*28, 
                truthTableHeightOffset + i*28);

            window.draw(truthTableCells[index]);
        }
        
    }

    for (int i = 0; i < 6; ++i)
    {
        truthTableHeaderText[i].setString(truthTableHeader[i]);
        truthTableHeaderText[i].setCharacterSize(20);
        truthTableHeaderText[i].setFillColor(sf::Color::Black);
        bounds = truthTableHeaderText[i].getLocalBounds();
        truthTableHeaderText[i].setPosition(
            truthTableWidthOffset + (26 - bounds.width) / 2 + i*28 - bounds.left,
            truthTableHeightOffset);

        window.draw(truthTableHeaderText[i]);
    }

    for (int i = 1; i < 17; ++i)
    {
        std::bitset<4> x(value);

        for (int j = 0; j < 4; ++j)
        {
            index = (i-1)*4 + j;
            bit = '0' + x[3-j];

            truthTableVariablesValues[index].setString(bit);
            truthTableVariablesValues[index].setCharacterSize(20);
            truthTableVariablesValues[index].setFillColor(sf::Color::Black);
            bounds = truthTableVariablesValues[index].getLocalBounds();
            truthTableVariablesValues[index].setPosition(
                truthTableWidthOffset + (26 - bounds.width) / 2 + j*28 - bounds.left,
                truthTableHeightOffset + i*28);

            window.draw(truthTableVariablesValues[index]);
        }

        ++value;
    }

}

void MapInterface::handleMouseButtonPressedOnTruthTable(
    sf::Event::MouseButtonEvent& mouseButtonEvent)
{
    if (mouseButtonEvent.x <= truthTableWidthOffset + 28*4
        or mouseButtonEvent.x >= truthTableWidthOffset + 28*6)
    {
        return;
    } 

    if (mouseButtonEvent.y <= truthTableHeightOffset + 28
        or mouseButtonEvent.y >= truthTableHeightOffset + 28*17)
    {
        return;
    }

    int x = (mouseButtonEvent.x - (truthTableWidthOffset + 28*4)) / 28;
    int y = (mouseButtonEvent.y - (truthTableHeightOffset + 28)) / 28;
    int row = truthTableToKmap[y] / 4;
    int col = truthTableToKmap[y] % 4;

    kmapObject.changeCellValue(row, col, intToValue[x]);
}

void MapInterface::loop()
{
    window.create(sf::VideoMode(windowWidth, windowHeight), "Karnaugh map simulator!");
    window.setVerticalSyncEnabled(true);

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
                    handleMouseButtonPressedOnTruthTable(event.mouseButton);
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
        drawGroups();
        drawCellValues();
        drawAlgebraicMinterms();
        drawTruthTable();
        window.display();
    }
}
