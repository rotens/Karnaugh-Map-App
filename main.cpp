#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Map4x4.hpp"
#include "Map1x2.hpp"
#include "Map2x2.hpp"
#include "MapInterface.hpp"

#define __TEST__ std::cout<<"TEST"<<std::endl;
std::string colorize(std::string str, int color);

class MapTest
{
public:
    void runAllTests();
private:
    void testHorizontalOctetFinding();
    void testVerticalOctetFinding();
    void testVerticalAndHorizontalOctetFinding();
    void testGetRealIndex();
    template <typename T> 
    void assertEqual(T, T);
    void assert2dVectorsEqual(Groups&, Groups&);
};

void MapTest::testHorizontalOctetFinding()
{
    Map4x4 kmap;
    kmap.initializeElementsWithGivenValues({
        Value::one, Value::one, Value::one, Value::one,
        Value::one, Value::one, Value::one, Value::one,
        Value::zero, Value::zero, Value::zero, Value::zero,
        Value::one, Value::one, Value::one, Value::one});
    kmap.findOctets();

    Groups octets{
        {0, 1, 2, 3, 4, 5, 6, 7},
        {12, 13, 14, 15, 0, 1, 2, 3}};

    std::cout << __func__ << " ";
    assert2dVectorsEqual(kmap.getOctets(), octets);
    // kmap.printOctets();
    // std::cout << kmap.getOctets().size() << std::endl;
}

void MapTest::testVerticalOctetFinding()
{
    Map4x4 kmap;
    kmap.initializeElementsWithGivenValues({
        Value::one, Value::one, Value::zero, Value::one,
        Value::one, Value::one, Value::zero, Value::one,
        Value::one, Value::one, Value::zero, Value::one,
        Value::one, Value::one, Value::zero, Value::one});
    kmap.findOctets();

    Groups octets{
        {0, 1, 4, 5, 8, 9, 12, 13},
        {3, 0, 7, 4, 11, 8, 15, 12}};

    std::cout << __func__ << " ";
    assert2dVectorsEqual(kmap.getOctets(), octets);
    // kmap.printOctets();
    // std::cout << kmap.getOctets().size() << std::endl;
}

void MapTest::testVerticalAndHorizontalOctetFinding()
{
    Map4x4 kmap;
    kmap.initializeElementsWithGivenValues({
        Value::one, Value::one, Value::one, Value::one,
        Value::one, Value::one, Value::one, Value::one,
        Value::one, Value::one, Value::zero, Value::one,
        Value::one, Value::one, Value::one, Value::one});
    kmap.findOctets();

    Groups octets{
        {0, 1, 2, 3, 4, 5, 6, 7},
        {12, 13, 14, 15, 0, 1, 2, 3},
        {0, 1, 4, 5, 8, 9, 12, 13},
        {3, 0, 7, 4, 11, 8, 15, 12}};

    std::cout << __func__ << " ";
    assert2dVectorsEqual(kmap.getOctets(), octets);
    // kmap.printOctets();
    // std::cout << kmap.getOctets().size() << std::endl;
}

void MapTest::testGetRealIndex()
{
    std::cout << __func__ << " ";
    int cellIndex = getCellIndex(0, 1, 1);
    assertEqual(cellIndex, 5);
    // std::cout << cellIndex << std::endl;

    std::cout << __func__ << " ";
    cellIndex = getCellIndex(0, -1, -1);
    assertEqual(cellIndex, 15);
    // std::cout << cellIndex << std::endl;
}

template <typename T>
void MapTest::assertEqual(T a, T b)
{
    std::cout << (a == b ? colorize("OK", 1) : colorize("NOT OK", 0)) << std::endl;
}


void MapTest::assert2dVectorsEqual(Groups& vec1, Groups& vec2)
{
    if (vec1.size() != vec2.size())
    {
        std::cout << colorize("NOT OK", 0) << std::endl;
        return;
    }

    for (auto it1 = vec1.begin(), it2 = vec2.begin(); 
         it1 != vec1.end();
         ++it1, ++it2)
    {
        if(!std::equal(it1->begin(), it1->end(), it2->begin()))
        {
            std::cout << colorize("NOT OK", 0) << std::endl;
            return;
        }
    }

    std::cout << colorize("OK", 1) << std::endl;
}

void MapTest::runAllTests()
{
    testHorizontalOctetFinding();
    testVerticalOctetFinding();
    testVerticalAndHorizontalOctetFinding();
    testGetRealIndex();
}

std::string colorize(std::string str, int color) 
{
    switch (color)
    {
    case 0:
        return "\033[1;31m" + str + "\033[0m\n";
    case 1:
        return "\033[1;32m" + str + "\033[0m\n";
    default:
         return str;
    }
}

int main()
{


    // Map1x2 map1x2Object;
    // map1x2Object.changeValue(0, Value::one);
    // map1x2Object.findAlgebraicMinterm();
    // std::cout << map1x2Object.getAlgebraicMinterm() << std::endl;

    // map1x2Object.changeValue(1, Value::one);
    // map1x2Object.findAlgebraicMinterm();
    // std::cout << map1x2Object.getAlgebraicMinterm() << std::endl;

    // map1x2Object.changeValue(0, Value::zero);
    // map1x2Object.findAlgebraicMinterm();
    // std::cout << map1x2Object.getAlgebraicMinterm() << std::endl;


    // std::array<std::array<Value, 2>, 2> testKmap = {{
    //     {Value::zero, Value::one},
    //     {Value::one, Value::zero}
    // }};

    // Map2x2 map2x2Object;
    // map2x2Object.initializeKmapWith(testKmap);
    // map2x2Object.solve();
    // for (const auto& minterm : map2x2Object.getAlgebraicMinterms())
    // {
    //     std::cout << minterm << " + "; 
    // }


    // std::array<std::array<Value, 4>, 4> testKmap = {{
    //     {Value::zero, Value::zero, Value::zero, Value::zero},
    //     {Value::zero, Value::zero, Value::zero, Value::zero},
    //     {Value::zero, Value::zero, Value::zero, Value::zero},
    //     {Value::zero, Value::zero, Value::zero, Value::zero}
    // }};

    // Map4x4 kmapObject;
    // kmapObject.initializeKmapWith(testKmap);
    // kmapObject.countZeroesAndOnes();

    // sf::Font font;
    // if (!font.loadFromFile("fonts/segoeui.ttf"))
    // {
    //     std::cerr << "Can't load font from file" << std::endl;
    //     return -1;
    // }

    // MapInterface mapInterface(font, kmapObject);
    // mapInterface.loop();
   

    // Map4x4 kmap;
    // kmap.printKmap();
    // kmap.initializeElementsWithGivenValues({
    //     Value::one, Value::one, Value::zero, Value::zero,
    //     Value::one, Value::one, Value::zero, Value::zero,
    //     Value::one, Value::one, Value::zero, Value::zero,
    //     Value::one, Value::one, Value::zero, Value::zero});
    // kmap.printKmap();

    MapTest testObject;
    testObject.runAllTests();
    
    // for (int i = 0; i < 16; ++i)
    // {
    //     std::bitset<4> x(i);
    //     std::cout << x[0] << x[1] << x[2] << x[3] << std::endl;
    // }

    return 0;
}
