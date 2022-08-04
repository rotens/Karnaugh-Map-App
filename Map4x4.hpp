#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <set>
#include <utility>
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
    bool hasAllCellsWithSameValue();
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

// class Map4x4
// {
//     std::array<std::array<Value, 4>, 4> kmap;
//     int8_t zeroes = 0;
//     int8_t ones = 0;
//     // std::vector<std::array<std::pair<int8_t, int8_t>, 4>> square2x2Groups;
//     std::vector<std::array<std::pair<int8_t, int8_t>, 4>> square2x2Groups = {
//         {{{1, 1}, {1, 2}, {2, 1}, {2, 2}}},
//         // {{{3, 3}, {3, 0}, {0, 3}, {0, 0}}},
//     //     {{{1, 3}, {1, 0}, {2, 3}, {2, 0}}}
//     };
//     std::vector<std::array<std::pair<int8_t, int8_t>, 4>> rect4x1Groups;
//     // std::vector<std::array<std::pair<int8_t, int8_t>, 4>> rect4x1Groups = {
//     //     {{{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
//     //     {{{1, 0}, {1, 1}, {1, 2}, {1, 3}}}
//     // };
//     std::vector<std::array<std::pair<int8_t, int8_t>, 8>> rect4x2Groups;
//     // std::vector<std::array<std::pair<int8_t, int8_t>, 8>> rect4x2Groups = {
//     //     {{{3, 0}, {3, 1}, {3, 2}, {3, 3}, {0, 0}, {0, 1}, {0, 2}, {0, 3}}},
//     //     {{{0, 3}, {1, 3}, {2, 3}, {3, 3}, {0, 0}, {1, 0}, {2, 0}, {3, 0}}}
//     // };
//     // std::vector<std::array<std::pair<int8_t, int8_t>, 2>> rect2x1Groups;
//     std::vector<std::array<std::pair<int8_t, int8_t>, 2>> rect2x1Groups = {
//         // {{{0, 0}, {0, 1}}}, {{{1, 3}, {2, 3}}}, {{{0, 0}, {1, 0}}}
//         {{{2, 1}, {3, 1}}}
//     };
//     // std::vector<std::array<std::pair<int8_t, int8_t>, 2>> rect2x1Groups = {
//     //      {{{1, 3}, {1, 0}}}, {{{3, 1}, {0, 1}}}, {{{2, 1}, {2, 2}}}
//     // };
//     // std::vector<std::pair<int8_t, int8_t>> _1x1Groups;
//     std::vector<std::pair<int8_t, int8_t>> _1x1Groups = {
//         // {2, 2}, {3, 3}, {1, 1}, {0,0}
//         {0, 3}
//     };
//     std::array<int8_t, 16> cellsInGroup{};
//     std::array<int8_t, 16> cellsNumberOfGroups{};
//     std::vector<std::string> algebraicMinterms = {
//         "BD", "A\'BC", "AB\'C\'D\'"
//     };

//     void findSquare2x2Groups();
//     void findRect4x2Groups();
//     void findRect4x1Groups();
//     void findRect2x1Groups();
//     void find1x1Groups();
//     bool isGroup(int8_t, int8_t, int8_t, int8_t);
//     bool is1x1Group(int8_t, int8_t);
//     void addSquare2x2Group(int8_t, int8_t);
//     void addRect4x2Group(int8_t, int8_t, int8_t, int8_t);
//     void addRect4x1Group(int8_t, int8_t, int8_t, int8_t);
//     void addRect2x1Group(int8_t, int8_t, int8_t, int8_t);
//     void add1x1Group(int8_t, int8_t);
//     bool is4x1GroupContainedIn4x2Group(int8_t, int8_t, int8_t, int8_t);
//     template<int N> 
//     bool isContainedInGroup(
//         std::vector<std::array<std::pair<int8_t, int8_t>, N>>&, int8_t, int8_t, int8_t, int8_t);
//     bool isAnyCellNotInGroupExistent();
//     void removeRedundantGroups();
//     template<int N, typename T> 
//     void removeRedundantGroupsFromGivenVector(
//         std::vector<std::array<std::pair<int8_t, int8_t>, N>>&);
//     template<int N> 
//     void decrementNumberOfGroups(std::array<std::pair<int8_t, int8_t>, N>&);
//     template<int N>
//     void findAlgebraicMinterms(std::vector<std::array<std::pair<int8_t, int8_t>, N>>&);
//     std::string getProduct(std::vector<std::string>&);
//     template<int N> 
//     int getNumberOfCellsInGroup(std::vector<std::array<std::pair<int8_t, int8_t>, N>>&);

// public:
//     void printKmap() const;
//     void initializeKmap();
//     void initializeKmapWith(std::array<std::array<Value, 4>, 4>&);
//     void printGroup() const;
//     void printSquare2x2Groups() const;
//     void printRect4x2Group() const;
//     void printRect4x1Group() const;
//     void printRect2x1Group() const;
//     void print1x1Group() const;
//     void countZeroesAndOnes();
//     void findGroups();
//     void solve();
//     void printCellsNumberOfGroups();
//     void printMinterms() const;
//     std::array<std::array<Value, 4>, 4>& getKmap();
//     Value getCellValue(int row, int col);
//     bool changeCellValue(int row, int col, Value value);
//     int8_t getZeroes();
//     std::vector<std::pair<int8_t, int8_t>>& get1x1Groups();
//     std::vector<std::array<std::pair<int8_t, int8_t>, 2>>& get2x1Groups();
//     std::vector<std::array<std::pair<int8_t, int8_t>, 4>>& get4x1Groups();
//     std::vector<std::array<std::pair<int8_t, int8_t>, 4>>& get2x2Groups();
//     std::vector<std::array<std::pair<int8_t, int8_t>, 8>>& get4x2Groups();
//     std::vector<std::string>& getAlgebraicMinterms();
// };
