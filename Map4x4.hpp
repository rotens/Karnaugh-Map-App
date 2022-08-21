#pragma once
#include <iostream>
#include <set>
#include <vector>
#include "other.hpp"

class KmapCell;

class Map4x4 
{
public:
    Map4x4();
    ~Map4x4();
    void printKmap();
    void printOctets();
    void printSquareQuads();
    void printRectQuads();
    void printPairs();
    void initializeElementsWithGivenValues(const std::vector<Value>& values);
    void findOctets();
    Value getCellValue(int cellIndex);
    bool isCellDone(int cellIndex);
    Groups& getOctets() { return octets; }
    Groups& getPairs() { return pairs; }
    Groups& getSquareQuads() { return squareQuads; }
    Groups& getRectQuads() { return rectQuads; }
    std::vector<int>& getSingleGroups() { return singleGroups; }
    void findPossiblePairs();
    void findPossiblePairsWithSharing();
    void pairCells(int);
    void pairCellsWithSharing();
    void findPossibleQuads();
    void findPossibleQuadsWithSharing();
    void quadCellsWithOnePossibility();
    void quadCellsWithTwoPossibilities();
    void quadCellsWithTwoPossibilitiesAndWithSharing();
    void repeatQuadingCellsWithTwoPossibilities();
    void repeatQuadingCellsWithOnePossibility();
    void repeatPairingCellsWithTwoPossibilities();
    void repeatPairingCellsWithTwoPossibilitiesAndSharing();
    void repeatPairingCellsWithOnePossibility();
    void squareQuadCells(KmapCell*);
    void rectQuadCells(KmapCell*);
    void findGroups();
    KmapCell& getCell(int cellIndex) { return *kmap[cellIndex]; }
    void decrementGroupingPossibilities();
    void decrementGroupingPossibilities2(std::vector<int>&);
    void setGroupingPossibilitiesToTwo();
    bool hasAllCellsGrouped();
    bool hasCellsWithMoreThanTwoPossibilitiesToQuad();
    void printEverything();
    void findAlgebraicMintermsForGivenGroup(const Groups&);
    void findAlgebraicMinterms();
    void findAlgebraicMintermOfSingleGroup(int);
    void findSingleGroups();
    std::string getProduct(std::vector<std::string>&);
    void printAlgebraicMinterms();
    std::vector<std::string>& getAlgebraicMinterms() { return algebraicMinterms; }
    void addRectQuadWithSharing(KmapCell*, int);
    void addSquareQuadWithSharing(KmapCell*, int);
    void insertJustGroupedCells(std::vector<int>&);
    void reset();
    void changeCellValue(int);
    void changeCellValue(int, Value);
    int getOnes() { return ones; }
    std::vector<KmapCell*>& getKmap() { return kmap; }
    void fillMapWithZeroValues();

    friend class MapTest;
    
private:
    std::vector<KmapCell*> kmap{16, nullptr};
    Groups octets;
    Groups pairs;
    Groups squareQuads;
    Groups rectQuads;
    std::vector<int> singleGroups;
    std::set<int> justGroupedCells;
    int ones{0};
    std::vector<std::string> algebraicMinterms;
    bool quadFound1{false};
    bool quadFound2{false};
    bool pairFound1{false};
    bool pairFound2{false};
    bool findingQuadsWithSharingDone{false};
    bool findingPairsWithSharingDone{false};

    void findHorizontalOctets();
    void findVerticalOctets();
};
