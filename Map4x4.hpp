#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>
#include "other.hpp"

class Map4x4
{
    std::array<std::array<Value, 4>, 4> kmap;
    int8_t zeroes = 0;
    int8_t ones = 0;
    std::vector<std::array<std::pair<int8_t, int8_t>, 4>> square2x2Groups;
    std::vector<std::array<std::pair<int8_t, int8_t>, 4>> rect4x1Groups;
    std::vector<std::array<std::pair<int8_t, int8_t>, 8>> rect4x2Groups;
    std::vector<std::array<std::pair<int8_t, int8_t>, 2>> rect2x1Groups;
    std::vector<std::pair<int8_t, int8_t>> _1x1Groups;
    std::array<int8_t, 16> cellsInGroup{};
    std::array<int8_t, 16> cellsNumberOfGroups{};

    void findSquare2x2Groups();
    void findRect4x2Groups();
    void findRect4x1Groups();
    void findRect2x1Groups();
    void find1x1Groups();
    bool isGroup(int8_t, int8_t, int8_t, int8_t);
    bool is1x1Group(int8_t, int8_t);
    void addSquare2x2Group(int8_t, int8_t);
    void addRect4x2Group(int8_t, int8_t, int8_t, int8_t);
    void addRect4x1Group(int8_t, int8_t, int8_t, int8_t);
    void addRect2x1Group(int8_t, int8_t, int8_t, int8_t);
    void add1x1Group(int8_t, int8_t);
    bool is4x1GroupContainedIn4x2Group(int8_t, int8_t, int8_t, int8_t);
    template<int N> 
    bool isContainedInGroup(
        std::vector<std::array<std::pair<int8_t, int8_t>, N>>&, int8_t, int8_t, int8_t, int8_t);
    bool isAnyCellNotInGroupExistent();
    void removeRedundantGroups();
    template<int N, typename T> 
    void removeRedundantGroupsFromGivenVector(
        std::vector<std::array<std::pair<int8_t, int8_t>, N>>&);
    template<int N> 
    void decrementNumberOfGroups(std::array<std::pair<int8_t, int8_t>, N>&);

public:
    void printKmap() const;
    void initializeKmap();
    void initializeKmapWith(std::array<std::array<Value, 4>, 4>&);
    void printGroup() const;
    void printSquare2x2Groups() const;
    void printRect4x2Group() const;
    void printRect4x1Group() const;
    void printRect2x1Group() const;
    void print1x1Group() const;
    void countZeroesAndOnes();
    void findGroups();
    void printCellsNumberOfGroups();
};
