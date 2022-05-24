#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>
#include "other.hpp"

class Map4x4;

class KmapCell 
{
public:
    KmapCell(Map4x4& kmapObject);
    void setCellValue(Value);
    Value getCellValue();
    void decrementPairsNumber();
    void decrementQuadsNumber();
    void incrementPairsNumber();
    void incrementQuadsNumber();

private:
    Map4x4& kmapObject;
    int pairsNumber{0};
    int quadsNumber{0};
    Value cellValue{Value::zero};
    bool done{false};
};


// MAP 4x4

class Map4x4 
{
public:
    Map4x4();
    ~Map4x4();
    void printKmap();
    void initializeElementsWithGivenValues(const std::vector<Value>& values);

private:
    std::vector<KmapCell*> kmap{16, nullptr};
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
