#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Map4x4.hpp"
#include "Map1x2.hpp"
#include "Map2x2.hpp"
#include "MapInterface.hpp"

#define __TEST__ std::cout<<"TEST"<<std::endl;
std::string colorize(std::string str, int color);
constexpr int standardIndicesToKmapIndices[] = {0, 4, 12, 8, 1, 5, 13, 9, 3, 7, 15, 11, 2, 6, 14, 10};
constexpr int kmapIndicesToStandardIndices[] = {0, 4, 12, 8, 1, 5, 13, 9, 3, 7, 15, 11, 2, 6, 14, 10};
// constexpr int kmapIndicesToStandardIndices[] = {1, 5, 13, 9, 2, 6, 14, 10, 4, 8, 16, 12, 3, 7, 15, 11};

class MapTest
{
public:
    void runAllTests();
private:
    void testHorizontalOctetFinding();
    void testVerticalOctetFinding();
    void testVerticalAndHorizontalOctetFinding();
    void testGetRealIndex();
    void testFindPairs();
    void testFindSquareQuads();
    void testFindRectQuads();
    void testFindQuads();
    void testDecrementingPossibilites();
    void testRemovingSquareQuads();
    void testGroupFinding();
    void testFindingMintermsOfSingleGroup();
    template <typename T> 
    void assertEqual(T, T);
    void assert2dVectorsEqual(Groups&, Groups&);
    template <typename T>
    void assertVectorsEqual(std::vector<T>&, std::vector<T>&);
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

void MapTest::testFindPairs()
{
    Map4x4 kmap;
    kmap.initializeElementsWithGivenValues({
        Value::zero, Value::zero, Value::zero, Value::zero,
        Value::one, Value::zero, Value::zero, Value::zero,
        Value::one, Value::one, Value::zero, Value::zero,
        Value::zero, Value::one, Value::zero, Value::zero});
    kmap.findPossiblePairs();
    kmap.pairCells();

    Groups pairs{{4, 8}, {13, 9}};

    std::cout << __func__ << " ";
    assert2dVectorsEqual(kmap.getPairs(), pairs);
    // kmap.printPairs();
    auto& cell1 = kmap.getCell(8);
    auto& cell2 = kmap.getCell(9);
    // assertEqual(cell1.getPairsNumber(), 1);
    // assertEqual(cell2.getPairsNumber(), 1);
    // std::cout << cell1.getPairsNumber() << std::endl;
    // std::cout << cell2.getPairsNumber() << std::endl;
}

void MapTest::testFindQuads()
{
    Map4x4 kmap;
    kmap.initializeElementsWithGivenValues({
        Value::one, Value::one, Value::one, Value::one,
        Value::zero, Value::one, Value::zero, Value::zero,
        Value::zero, Value::one, Value::one, Value::one,
        Value::zero, Value::one, Value::one, Value::one});
    kmap.findPossibleQuads();
    kmap.quadCells();

    Groups rectQuads = {
        {0, 1, 2, 3},
        {5, 9, 13, 1}};

    Groups squareQuads = {{11, 15, 14, 10}};

    std::cout << __func__ << " ";
    assert2dVectorsEqual(kmap.getRectQuads(), rectQuads);
    assert2dVectorsEqual(kmap.getSquareQuads(), squareQuads);
    kmap.printRectQuads();
    kmap.printSquareQuads();
}

void MapTest::testDecrementingPossibilites()
{
    Map4x4 kmap;
    kmap.initializeElementsWithGivenValues({
        Value::zero, Value::zero, Value::zero, Value::zero,
        Value::one, Value::one, Value::zero, Value::zero,
        Value::zero, Value::one, Value::one, Value::one,
        Value::zero, Value::zero, Value::zero, Value::one});
    kmap.findPossiblePairs();
    kmap.pairCells();
    // kmap.findQuads();
    kmap.decrementGroupingPossibilities();

    auto& cell1 = kmap.getCell(9);
    auto pairsNumber1 = cell1.getPairsNumber();
    auto& cell2 = kmap.getCell(10);
    auto pairsNumber2 = cell2.getPairsNumber();

    std::cout << __func__ << " ";
    assertEqual(pairsNumber1, 1);
    assertEqual(pairsNumber2, 1);
    std::cout << pairsNumber1 << std::endl;
    std::cout << pairsNumber2 << std::endl;
}

void MapTest::testRemovingSquareQuads()
{
    Map4x4 kmap;
    auto& cell = kmap.getCell(5);
    auto& quads = cell.getSquareQuads();
    std::vector<int> quad1{0, 1, 2, 3};
    std::vector<int> quad2{0, 5, 6, 7};
    std::vector<int> quad3{0, 1, 4, 5};
    quads.push_back(quad1);
    quads.push_back(quad2);
    quads.push_back(quad3);
    cell.setSquareQuadsNumber(3);
    cell.removeSquareQuadContainingGivenCellIndex(0);
    std::cout << __func__ << " ";
    assertEqual(cell.getSquareQuadsNumber(), 1);
    std::cout << cell.getSquareQuadsNumber() << std::endl;
}

void MapTest::testGroupFinding()
{
    Map4x4 kmap;
    kmap.initializeElementsWithGivenValues({
        Value::zero, Value::zero, Value::one, Value::one,
        Value::zero, Value::one, Value::one, Value::zero,
        Value::zero, Value::one, Value::one, Value::zero,
        Value::one, Value::one, Value::zero, Value::zero});
    kmap.findGroups();
    kmap.findAlgebraicMinterms();
    kmap.printEverything();

    Map4x4 kmap2;
    kmap2.initializeElementsWithGivenValues({
        Value::zero, Value::one, Value::zero, Value::zero,
        Value::zero, Value::one, Value::one, Value::one,
        Value::one, Value::one, Value::one, Value::zero,
        Value::zero, Value::zero, Value::one, Value::zero});
    kmap2.findGroups();
    kmap2.findAlgebraicMinterms();
    kmap2.printEverything();
}

void MapTest::testFindingMintermsOfSingleGroup()
{
    Map4x4 kmap;
    kmap.findAlgebraicMintermOfSingleGroup(0); 
    kmap.findAlgebraicMintermOfSingleGroup(4); 
    kmap.findAlgebraicMintermOfSingleGroup(10); 
    kmap.findAlgebraicMintermOfSingleGroup(15);
    std::vector<std::string> minterms{"!A!B!C!D", "!A!B!CD", "ABCD", "A!BC!D"};
    std::cout << __func__ << " ";
    assertVectorsEqual<std::string>(kmap.getAlgebraicMinterms(), minterms);
}

template <typename T>
void MapTest::assertEqual(T a, T b)
{
    std::cout << (a == b ? colorize("OK", 1) : colorize("NOT OK", 0)) << std::endl;
}

template <typename T>
void MapTest::assertVectorsEqual(std::vector<T>& vec1, std::vector<T>& vec2)
{
    if (vec1.size() != vec2.size())
    {
        std::cout << colorize("NOT OK", 0) << std::endl;
        return;
    }

    if (!std::equal(vec1.begin(), vec1.end(), vec2.begin()))
    {
        std::cout << colorize("NOT OK", 0) << std::endl;
    }
    else
    {
        std::cout << colorize("OK", 1) << std::endl;
    }
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
        if (!std::equal(it1->begin(), it1->end(), it2->begin()))
        {
            std::cout << colorize("NOT OK", 0) << std::endl;
            return;
        }
    }

    std::cout << colorize("OK", 1) << std::endl;
}

void MapTest::runAllTests()
{
    // testHorizontalOctetFinding();
    // testVerticalOctetFinding();
    // testVerticalAndHorizontalOctetFinding();
    // testGetRealIndex();
    // testFindPairs();
    // testFindQuads();
    // testDecrementingPossibilites();
    // testRemovingSquareQuads();
    // testGroupFinding();
    testFindingMintermsOfSingleGroup();
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

int convertVariableToIndex(char var)
{
    return 3 - (var - 65);
}

char convertIndexToVariable(int index)
{
    return index + 65;
}

bool calculateProduct(const std::string& product, const std::bitset<4>& arguments)
{
    bool calculatedProduct = true;
    bool negation = false;

    for (const auto variable : product)
    { 
        if (variable == '!')
        {
            negation = true;
            continue;
        }
        
        if (negation)
        {
            calculatedProduct = calculatedProduct && !arguments[convertVariableToIndex(variable)];
            negation = false;
        }
        else
        {
            calculatedProduct = calculatedProduct && arguments[convertVariableToIndex(variable)];
        }
    }

    return calculatedProduct;
}

bool calculateSumOfProducts(const std::vector<std::string>& products, const std::bitset<4>& arguments)
{
    bool sum = false;

    for (const auto& product : products)
    {
        sum = sum || calculateProduct(product, arguments);
    }

    return sum;
}

void initializeKmapWithBitset(const std::bitset<16>& functionValues, Map4x4& kmapObject)
{
    std::vector<Value> kmap(16);

    for (int i = 0; i < kmap.size(); ++i)
    {
        kmap[standardIndicesToKmapIndices[i]] = static_cast<Value>(functionValues[i]); 
    }

    kmapObject.initializeElementsWithGivenValues(kmap);
}

std::vector<std::bitset<4>> createArgumentsCombinations()
{
    std::vector<std::bitset<4>> argumentsCombinations;
    argumentsCombinations.reserve(16);

    for (int i = 0; i < 16; ++i)
    {
        argumentsCombinations.push_back(std::bitset<4>(i));
    }

    return argumentsCombinations;
}

bool isMinimizationCorrect(
    const std::vector<std::string>& minterms, 
    const std::bitset<16>& functionValues, 
    const std::vector<std::bitset<4>>& argumentsCombinations)
{
    for (int i = 0; i < functionValues.size(); ++i)
    {
        int sum = calculateSumOfProducts(minterms, argumentsCombinations[i]);
        std::cout << sum << std::endl;
        if (sum != functionValues[i])
        {
            return false;
        }
    }

    return true;
}

void writeValuesCombinationToFile(
    std::ofstream& file, 
    const std::bitset<16>& values, 
    const std::vector<std::string>& minterms)
{
    for (int i = 0; i < 16; ++i)
    {
        file << values[i];
    }

    file << " ";

    for (const auto& minterm : minterms)
    {
        file << minterm;
    }

    file << "\n";
}

void testAllFunctionValuesCombinations()
{
    auto argumentsCombinations = createArgumentsCombinations();
    std::ofstream output("values.txt");
    int counter = 0;

    for (int i = 1; i < 65536-1; ++i)
    {
        std::bitset<16> valuesCombination(i);
        Map4x4 kmapObject;
        initializeKmapWithBitset(valuesCombination, kmapObject);

        std::vector<std::string> minterms; // temp
        if (!isMinimizationCorrect(minterms, valuesCombination, argumentsCombinations))
        {
            writeValuesCombinationToFile(output, valuesCombination, minterms);
            ++counter;
        } 
    }

    output << "Total: " << counter << "\n";
    output.close();
}

std::bitset<16> createBitsetFromKmap(const std::vector<Value>& values)
{
    std::bitset<16> bitset(0);

    for (int i = 0; i < 16; ++i)
    {
        if (values[i] == Value::one)
        {
            bitset.set(kmapIndicesToStandardIndices[i]);
        }
        else
        {
            bitset.reset(kmapIndicesToStandardIndices[i]);
        }
    }

    return bitset;
}

void testOneCombination(const std::vector<Value>& values)
{
    auto valuesCombination = createBitsetFromKmap(values);
    Map4x4 kmapObject;
    kmapObject.initializeElementsWithGivenValues(values);
    kmapObject.findGroups();
    kmapObject.findAlgebraicMinterms();
    auto argumentsCombinations = createArgumentsCombinations();

    if (isMinimizationCorrect(
            kmapObject.getAlgebraicMinterms(),
            valuesCombination,
            argumentsCombinations))
    {
        std::cout << "MINIMIZATION OK" << std::endl;
    }
    else 
    {
        std::cout << "MINIMIZATION NOT OK" << std::endl;
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

    // testOneCombination({
    //     Value::zero, Value::zero, Value::one, Value::one,
    //     Value::zero, Value::one, Value::one, Value::zero,
    //     Value::zero, Value::one, Value::one, Value::zero,
    //     Value::one, Value::one, Value::zero, Value::zero});
    // testOneCombination({
    //     Value::zero, Value::one, Value::zero, Value::zero,
    //     Value::zero, Value::one, Value::one, Value::one,
    //     Value::one, Value::one, Value::one, Value::zero,
    //     Value::zero, Value::zero, Value::one, Value::zero});
 
    // std::bitset<4> x(0);
    // std::cout << calculateSumOfProducts({"A!C!D", "!AC!D", "BD"}, x) << std::endl;
    

    // testAllFunctionValuesCombinations();



    return 0;
}
