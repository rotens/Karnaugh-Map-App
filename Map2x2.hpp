#pragma once
#include <array>
#include <vector>
#include "other.hpp"

class Map2x2
{
    std::vector<Value> kmap{4, Value::zero};
    int ones{0};
    std::vector<int> singleGroups;
    std::vector<std::vector<int>> pairs;
    std::vector<std::string> algebraicMinterms;

    void findPairs();
    void findSingleGroups();
    void addPair(int, int);
    bool isPair(int, int);
    void findAlgebraicMintermsForPairs();
    void findAlgebraicMintermsForSingleGroups();

public:
    void initializeKmapWith(std::vector<Value>& kmap);
    std::vector<std::string>& getAlgebraicMinterms();
    std::vector<std::vector<int>>& getPairs() { return pairs; }
    std::vector<int>& getSingleGroups() { return singleGroups; }
    void findGroups();
    void findAlgebraicMinterms();
    bool changeValue(int, Value);
};
