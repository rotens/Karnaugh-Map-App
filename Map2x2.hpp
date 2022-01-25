#pragma once
#include <array>
#include <vector>
#include "other.hpp"

class Map2x2
{
    std::array<std::array<Value, 2>, 2> kmap;
    int zeroes{4};
    std::vector<std::pair<int, int>> _1x1Groups;
    std::vector<std::array<std::pair<int, int>, 2>> rect2x1Groups;
    std::vector<std::string> algebraicMinterms;

    void find2x1Groups();
    void find1x1Groups();
    void add2x1Group(int, int, int, int);
    bool is2x1Group(int, int, int, int);
    void findAlgebraicMinterms();
    void findAlgebraicMintermsFor2x1Groups();
    void findAlgebraicMintermsFor1x1Groups();
    bool changeValue(int, int, Value);
    void findGroups();

public:
    void solve();
    void initializeKmapWith(std::array<std::array<Value, 2>, 2>&);
    std::vector<std::string>& getAlgebraicMinterms();
};
