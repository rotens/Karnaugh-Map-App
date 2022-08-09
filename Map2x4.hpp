#pragma once
#include <iostream>
#include <set>
#include <vector>
#include "other.hpp"

class Kmap2x4Cell;

class Map2x4 
{
public:
    Map2x4();
    ~Map2x4();
    void printKmap();
    void printSquareQuads();
    void printRectQuads();
    void printPairs();
    void initializeElementsWithGivenValues(const std::vector<Value>& values);
    Value getCellValue(int cellIndex);
    bool isCellDone(int cellIndex);
    Groups& getPairs() { return pairs; }
    Groups& getSquareQuads() { return squareQuads; }
    Groups& getRectQuads() { return rectQuads; }
    void findPossiblePairs();
    void pairCells(int);
    void findSquareQuads();
    void findRectQuads();
    void findGroups();
    Kmap2x4Cell& getCell(int cellIndex) { return *kmap[cellIndex]; }
    void decrementGroupingPossibilities();
    bool hasAllCellsGrouped();
    void printEverything();
    void findAlgebraicMintermsForGivenGroup(const Groups&);
    void findAlgebraicMinterms();
    void findAlgebraicMintermOfSingleGroup(int);
    void findSingleGroups();
    std::string getProduct(std::vector<std::string>&);
    void printAlgebraicMinterms();
    std::vector<std::string>& getAlgebraicMinterms() { return algebraicMinterms; }

    
    friend class MapTest;
    
private:
    std::vector<Kmap2x4Cell*> kmap{8, nullptr};
    Groups pairs;
    Groups squareQuads;
    Groups rectQuads;
    std::vector<int> singleGroups;
    std::set<int> justGroupedCells;
    int ones{0};
    std::vector<std::string> algebraicMinterms;
    bool pairFound{false};
};
