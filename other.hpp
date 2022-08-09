#pragma once
#include <array>
#include <cstdint>
#include <utility>
#include <vector>

using Groups = std::vector<std::vector<int>>;
enum class Value { zero = '0', one = '1', dont_care = 'X' };
template<int N> using VecIter = std::vector<std::array<std::pair<int8_t, int8_t>, N>>::iterator;
inline std::string grayCode[] = {"00", "01", "11", "10"};
inline std::string variables[] = {"A", "B", "C", "D"};

inline std::vector<std::vector<std::pair<int, int>>> quadsOffsets{
    {{0, -1}, {-1, -1}, {-1, 0}},
    {{-1, 0}, {-1, 1}, {0, 1}},
    {{0, 1}, {1, 1}, {1, 0}},
    {{1, 0}, {1, -1}, {0, -1}}};

inline std::array<std::array<int, 4>, 14> properlySortedQuadsIndicesMap{{
    {0, 1, 12, 13}, {12, 13, 0, 1},
    {1, 2, 13, 14}, {13, 14, 1, 2},
    {2, 3, 14, 15}, {14, 15, 2, 3},
    {0, 3, 4, 7}, {3, 0, 7, 4},
    {4, 7, 8, 11}, {7, 4, 11, 8},
    {8, 11, 12, 15}, {11, 8, 15, 12},
    {0, 3, 12, 15}, {15, 12, 3, 0}
}};

int getCellIndex(int, int, int);
int getCellIndex_Map2x4(int, int, int);
int getRealIndex(int);
int getRealIndex_Map2x4(int, int);
