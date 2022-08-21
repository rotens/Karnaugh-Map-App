#include "MapInterface.hpp"
#include "other.hpp"
#include <iostream>
#include <cmath>
#include <bitset>

constexpr int map4x4Height = 4;
constexpr int map4x4Width = 4;
constexpr int map2x4Height = 2;
constexpr int map2x4Width = 4;

constexpr int windowWidth = 800;
constexpr int windowHeight = 640;
constexpr int mapWidthOffset = 500;
constexpr int mapHeightOffset = 197;
constexpr int textRelativeHeightOffset = 19 - 10;
constexpr int textRelativeWidthOffset = 24 - 3;

constexpr int map2x4CellsNumber = 8;
constexpr int map4x4CellsNumber = 16;

constexpr int horizontalGrayCodeHeightOffset = mapHeightOffset - 34;
constexpr int horizontalGrayCodeWidthOffset = mapWidthOffset + 17 - 1; // 1 = text's left bound
constexpr int verticalGrayCodeHeightOffset = mapHeightOffset + 21 - 8;
constexpr int verticalGrayCodeWidthOffset = mapWidthOffset - 34; // 1 = text's left bound

constexpr int ABVariablesWidthOffset = mapWidthOffset + 101;
constexpr int ABVariablesHeightOffset = mapHeightOffset - 68;
constexpr int CDVariablesWidthOffset = mapWidthOffset - 74;
constexpr int CDVariablesHeightOffset = mapHeightOffset + 138;
constexpr int CVariableHeightOffset = mapHeightOffset + 69;

constexpr int mintermsWidthOffset = mapWidthOffset - 230;
constexpr int mintermsHeightOffset = mapHeightOffset + 240 + 50;

constexpr int truthTableWidthOffset = 70;
constexpr int truthTableHeightOffset = 80; 
constexpr char truthTableHeader[] = {'A', 'B', 'C', 'D', '0', '1'};

sf::Color colors[8] = {
    sf::Color::Red, sf::Color::Blue, 
    sf::Color::Cyan, sf::Color::Green,
    sf::Color::Magenta, sf::Color(242, 213, 2),
    sf::Color(185, 122, 87)};
constexpr std::array<std::pair<int, int>, 4> diffs{{{0, 0}, {0, 1}, {1, 0}, {1, 1}}};
constexpr int truthTableToKmap[] = {0, 4, 12, 8, 1, 5, 13, 9, 3, 7, 15, 11, 2, 6, 14, 10};
constexpr int kmapToTruthTable[] = {1, 5, 13, 9, 2, 6, 14, 10, 4, 8, 16, 12, 3, 7, 15, 11};
constexpr Value intToValue[] = {Value::zero, Value::one, Value::dont_care};

MapInterface::MapInterface(sf::Font& font)
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

    setUpMap2x4();
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
    for (int i = 0; i < currentMapHeight; ++i)
    {
        for (int j = 0; j < currentMapWidth; ++j)
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
    int cellIndex;

    for (int i = 0; i < currentMapHeight; ++i)
    {
        for (int j = 0; j < currentMapWidth; ++j)
        {
            cellIndex = 4*i + j;
            cellValues[k].setString(static_cast<char>(getCellValue(cellIndex)));
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
    switch (currentMapType)
    {
        case MapType::map4x4:
            drawGrayCodeMap4x4();
            break;
        case MapType::map2x4:
            drawGrayCodeMap2x4();
            break;
    }
}

void MapInterface::drawGrayCodeMap4x4()
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

void MapInterface::drawGrayCodeMap2x4()
{
    for (int i = 0; i < map2x4Width; ++i)
    {
        grayCodeText[i].setString(grayCode[i]);
        grayCodeText[i].setCharacterSize(24); 
        grayCodeText[i].setFillColor(sf::Color::Black);
        grayCodeText[i].setPosition(
            horizontalGrayCodeWidthOffset + i * 60,
            horizontalGrayCodeHeightOffset);

        window.draw(grayCodeText[i]);
    }

    for (int i = 0; i < map2x4Height; ++i)
    {
        grayCodeText[i+4].setString(grayCode[1][i]);
        grayCodeText[i+4].setCharacterSize(24); 
        grayCodeText[i+4].setFillColor(sf::Color::Black);
        grayCodeText[i+4].setPosition(
            verticalGrayCodeWidthOffset + 12,
            verticalGrayCodeHeightOffset + i * 60);

        window.draw(grayCodeText[i+4]);
    }
}

void MapInterface::drawVariables()
{
    variablesText[0].setString(currentVariables1);
    variablesText[0].setCharacterSize(30); 
    variablesText[0].setFillColor(sf::Color::Black);
    variablesText[0].setPosition(
        currentVariables1WidthOffset,
        currentVariables1HeightOffset);

    window.draw(variablesText[0]);

    variablesText[1].setString(currentVariables2);
    variablesText[1].setCharacterSize(30); 
    variablesText[1].setFillColor(sf::Color::Black);
    variablesText[1].setRotation(-90.f);
    variablesText[1].setPosition(
        currentVariables2WidthOffset,
        currentVariables2HeightOffset);

    window.draw(variablesText[1]);
}

void MapInterface::drawMapBorder()
{
    if (getOnes() != currentCellsNumber)
        return;
    
    mapBorder.setOutlineColor(sf::Color::Red);
    mapBorder.setOutlineThickness(2.f);
    mapBorder.setFillColor(sf::Color::Transparent);
    mapBorder.setSize(
        sf::Vector2f(
            currentMapWidth*58 + 2*(currentMapWidth-1),
            currentMapHeight*58 + 2*(currentMapHeight-1)));
    mapBorder.setPosition(sf::Vector2f(mapWidthOffset, mapHeightOffset));

    window.draw(mapBorder);
}

void MapInterface::drawOctets()
{
    int width;
    int height;
    int longerLineWidth;
    int longerLineHeight;
    int shorterLinesWidth;
    int shorterLinesHeight;
    std::pair<int, int> firstCell, fourthCell, lastCell;

    for (const auto& octet : *octets)
    {
        firstCell = std::make_pair(octet[0] / 4, octet[0] % 4);
        fourthCell = std::make_pair(octet[3] / 4, octet[3] % 4);
        lastCell = std::make_pair(octet[7] / 4, octet[7] % 4);

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
      
       incrementCurrentColorIndex();
    }
}

void MapInterface::drawSquareQuads()
{
    std::pair<int, int> cell;
    int l;

    for (auto& quad : *squareQuads)
    {
        l = 0;
        quad = getSortedQuad(quad);

        for (const auto& cellIndex : quad) 
        {
            cell = std::make_pair(cellIndex / 4, cellIndex % 4); 
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

        incrementCurrentColorIndex();
    }
}

void MapInterface::drawRectQuads()
{
    int width;
    int height;
    int k = 0;
    std::pair<int, int> firstCell;
    std::pair<int, int> lastCell;

    for (auto& quad : *rectQuads)
    {
        std::sort(quad.begin(), quad.end());

        firstCell = std::make_pair(quad[0] / 4, quad[0] % 4);
        lastCell = std::make_pair(quad[3] / 4, quad[3] % 4);

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
        incrementCurrentColorIndex();
    }
}

void MapInterface::drawPairs()
{
    int width;
    int height;
    std::pair<int, int> firstCell, lastCell;

    for (const auto& pair : *pairs)
    {
        auto sortedPair = getSortedPair(pair);
        firstCell = std::make_pair(sortedPair.first / 4, sortedPair.first % 4);
        lastCell = std::make_pair(sortedPair.second / 4, sortedPair.second % 4);

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
       
        incrementCurrentColorIndex();
    }
}

void MapInterface::drawSingleGroups()
{
    for (const auto cellIndex : *singleGroups)
    {
        rectangles[cellIndex].setOutlineColor(colors[currentColorIndex]);
        window.draw(rectangles[cellIndex]);
        incrementCurrentColorIndex();
    }
}

void MapInterface::drawGroups() 
{
    currentColorIndex = 0;

    switch (currentMapType)
    {
        case MapType::map4x4:
            drawMap4x4Groups();
            break;
        case MapType::map2x4:
            drawMap2x4Groups();
            break;
    }
}
void MapInterface::drawMap4x4Groups() 
{
    drawMapBorder();
    drawOctets();
    drawRectQuads();
    drawSquareQuads();
    drawPairs();
    drawSingleGroups();
}
void MapInterface::drawMap2x4Groups() 
{
    drawMapBorder();
    drawRectQuads();
    drawSquareQuads();
    drawPairs();
    drawSingleGroups();
}

void MapInterface::drawAlgebraicMinterms()
{
    int i = 1;
    int mintermCounter = 1;
    int colorIndex = 0;
    int currentWidthOffset = mintermsWidthOffset;
    int currentHeightOffset = mintermsHeightOffset;
    
    algebraicMintermsText[0].setString("y = ");
    algebraicMintermsText[0].setCharacterSize(24);
    algebraicMintermsText[0].setFillColor(sf::Color::Black);
    algebraicMintermsText[0].setPosition(mintermsWidthOffset, currentHeightOffset);
    window.draw(algebraicMintermsText[0]);

    sf::FloatRect bounds = algebraicMintermsText[0].getLocalBounds();
    currentWidthOffset += bounds.width;

    for (const auto& minterm : *minterms)
    {
        algebraicMintermsText[i].setString(minterm);
        algebraicMintermsText[i].setCharacterSize(24);
        algebraicMintermsText[i].setFillColor(colors[colorIndex]);
        algebraicMintermsText[i].setPosition(currentWidthOffset, currentHeightOffset);
        window.draw(algebraicMintermsText[i]);

        if (mintermCounter == minterms->size())
            break;

        bounds = algebraicMintermsText[i].getLocalBounds();
        currentWidthOffset += bounds.width + 8;
        ++i;
        ++mintermCounter;

        if (currentWidthOffset > windowWidth - 110)
        {
            currentWidthOffset = mintermsWidthOffset;
            currentHeightOffset += 40;
        }

        algebraicMintermsText[i].setString("+");
        algebraicMintermsText[i].setCharacterSize(24);
        algebraicMintermsText[i].setFillColor(sf::Color::Black);
        algebraicMintermsText[i].setPosition(currentWidthOffset, currentHeightOffset);
        window.draw(algebraicMintermsText[i]);

        bounds = algebraicMintermsText[i].getLocalBounds();
        currentWidthOffset += bounds.width + 12;
        ++i;

        colorIndex = colorIndex + 1 > 7 ? 0 : ++colorIndex;
    }
}

void MapInterface::cellHover(sf::Event::MouseMoveEvent& mouseMove)
{
    if (mouseMove.x <= mapWidthOffset or mouseMove.x >= mapWidthOffset + currentMapWidth*58 + 6)
    {
        fillCellsWithWhiteColor();
        return;
    } 

    if (mouseMove.y <= mapHeightOffset or mouseMove.y >= mapHeightOffset + currentMapHeight*58 + 6)
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
    if (mouseButtonEvent.x <= mapWidthOffset or mouseButtonEvent.x >= mapWidthOffset + currentMapWidth*58 + 6)
    {
        return;
    } 

    if (mouseButtonEvent.y <= mapHeightOffset or mouseButtonEvent.y >= mapHeightOffset + currentMapHeight*58 + 6)
    {
        return;
    }

    int col = (mouseButtonEvent.x - mapWidthOffset) / 60;
    int row = (mouseButtonEvent.y - mapHeightOffset) / 60;
    int index = row*4 + col;

    changeCellValue(index);
    performMapMinimizing();
}

void MapInterface::drawTruthTable()
{
    int index;
    int value = 0;
    char bit;
    sf::FloatRect bounds;

    for (int i = 0; i < currentCellsNumber+1; ++i)
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

    // for (int i = 0; i < 6; ++i)
    // {
    //     truthTableHeaderText[i].setString(truthTableHeader[i]);
    //     truthTableHeaderText[i].setCharacterSize(20);
    //     truthTableHeaderText[i].setFillColor(sf::Color::Black);
    //     bounds = truthTableHeaderText[i].getLocalBounds();
    //     truthTableHeaderText[i].setPosition(
    //         truthTableWidthOffset + (26 - bounds.width) / 2 + i*28 - bounds.left,
    //         truthTableHeightOffset);

    //     window.draw(truthTableHeaderText[i]);
    // }

    // for (int i = 1; i < 17; ++i)
    // {
    //     std::bitset<4> x(value);

    //     for (int j = 0; j < 4; ++j)
    //     {
    //         index = (i-1)*4 + j;
    //         bit = '0' + x[3-j];

    //         truthTableVariablesValues[index].setString(bit);
    //         truthTableVariablesValues[index].setCharacterSize(20);
    //         truthTableVariablesValues[index].setFillColor(sf::Color::Black);
    //         bounds = truthTableVariablesValues[index].getLocalBounds();
    //         truthTableVariablesValues[index].setPosition(
    //             truthTableWidthOffset + (26 - bounds.width) / 2 + j*28 - bounds.left,
    //             truthTableHeightOffset + i*28);

    //         window.draw(truthTableVariablesValues[index]);
    //     }

    //     ++value;
    // }

    // truthTableStateCircle[0].setRadius(6.f);
    // bounds = truthTableStateCircle[0].getLocalBounds();
    // float stateCirclePosX, stateCirclePosY;

    // for (int index = 0; index < 16; ++index)
    // {
    //     stateCirclePosY = truthTableHeightOffset + kmapToTruthTable[index]*28 + (26 - bounds.height) / 2;
        
    //     if (kmapObject.getCellValue(index) == Value::zero)
    //     {
    //         stateCirclePosX = truthTableWidthOffset + (26 - bounds.width) / 2 + 4*28 - bounds.left;
    //     }
    //     else if (kmapObject.getCellValue(index) == Value::one)
    //     {
    //         stateCirclePosX = truthTableWidthOffset + (26 - bounds.width) / 2 + 5*28 - bounds.left;
    //     }

    //     truthTableStateCircle[index].setRadius(6.f);
    //     truthTableStateCircle[index].setFillColor(sf::Color(97, 157, 216));
    //     truthTableStateCircle[index].setPosition(stateCirclePosX, stateCirclePosY);
    //     window.draw(truthTableStateCircle[index]);
    // }
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
    int index = row*4 + col;
    changeCellValue(index, intToValue[x]);
    performMapMinimizing();
}

void MapInterface::performMap4x4Minimizing()
{
    kmapObject.reset();
    kmapObject.findGroups();
    kmapObject.findAlgebraicMinterms();
}

void MapInterface::performMap2x4Minimizing()
{
    kmap2x4Object.reset();
    kmap2x4Object.findGroups();
    kmap2x4Object.findAlgebraicMinterms();
}

void MapInterface::performMapMinimizing()
{
    switch (currentMapType)
    {
        case MapType::map4x4:
            performMap4x4Minimizing();
            break;
        case MapType::map2x4:
            performMap2x4Minimizing();
            break;
    }
}

Value MapInterface::getCellValue(int cellIndex)
{
    switch (currentMapType)
    {
        case MapType::map4x4:
            return kmapObject.getCellValue(cellIndex);
        case MapType::map2x4:
            return kmap2x4Object.getCellValue(cellIndex);
        default:
            return Value::zero;
    }
}

void MapInterface::changeCellValue(int cellIndex)
{
    switch (currentMapType)
    {
        case MapType::map4x4:
            kmapObject.changeCellValue(cellIndex);
            break;
        case MapType::map2x4:
            kmap2x4Object.changeCellValue(cellIndex);
            break;
    }
}

void MapInterface::changeCellValue(int cellIndex, Value valueToSet)
{
    switch (currentMapType)
    {
        case MapType::map4x4:
            kmapObject.changeCellValue(cellIndex, valueToSet);
            break;
        case MapType::map2x4:
            kmap2x4Object.changeCellValue(cellIndex, valueToSet);
            break;
    }
}

int MapInterface::getOnes()
{
    switch (currentMapType)
    {
        case MapType::map4x4:
            return kmapObject.getOnes();
        case MapType::map2x4:
            return kmap2x4Object.getOnes();
        default:
            return 0;
    }
}

void MapInterface::setUpMap2x4()
{
    kmap2x4Object.fillMapWithZeroValues();
    kmap2x4Object.reset();
    kmap2x4Object.findAlgebraicMinterms();
    squareQuads = &kmap2x4Object.getSquareQuads();
    rectQuads = &kmap2x4Object.getRectQuads();
    pairs = &kmap2x4Object.getPairs();
    singleGroups = &kmap2x4Object.getSingleGroups();
    minterms = &kmap2x4Object.getAlgebraicMinterms();
    currentMapType = MapType::map2x4;
    currentMapHeight = map2x4Height;
    currentMapWidth = map2x4Width;
    currentVariables1 = "AB";
    currentVariables2 = "C";
    currentVariables1HeightOffset = ABVariablesHeightOffset;
    currentVariables1WidthOffset = ABVariablesWidthOffset;
    currentVariables2HeightOffset = CVariableHeightOffset;
    currentVariables2WidthOffset = CDVariablesWidthOffset;
    currentCellsNumber = map2x4CellsNumber;
}

void MapInterface::setUpMap4x4()
{
    kmapObject.fillMapWithZeroValues();
    kmapObject.reset();
    kmapObject.findAlgebraicMinterms();
    octets = &kmapObject.getOctets();
    squareQuads = &kmapObject.getSquareQuads();
    rectQuads = &kmapObject.getRectQuads();
    pairs = &kmapObject.getPairs();
    singleGroups = &kmapObject.getSingleGroups();
    minterms = &kmapObject.getAlgebraicMinterms();
    currentMapType = MapType::map4x4;
    currentMapHeight = map4x4Height;
    currentMapWidth = map4x4Width;
    currentVariables1 = "AB";
    currentVariables2 = "CD";
    currentVariables1HeightOffset = ABVariablesHeightOffset;
    currentVariables1WidthOffset = ABVariablesWidthOffset;
    currentVariables2HeightOffset = CDVariablesHeightOffset;
    currentVariables2WidthOffset = CDVariablesWidthOffset;
    currentCellsNumber = map4x4CellsNumber;
}

void MapInterface::incrementCurrentColorIndex()
{
    if (currentColorIndex + 1 > 7)
    {
        currentColorIndex = 0;
    }
    else
    {
        ++currentColorIndex;
    }
}

void MapInterface::loop()
{
    window.create(sf::VideoMode(windowWidth, windowHeight), "Karnaugh map simulator!");
    window.setVerticalSyncEnabled(true);

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
