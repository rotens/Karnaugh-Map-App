#pragma once
#include <array>
#include <cstdint>
#include <utility>
#include <vector>

enum class Value { zero = '0', one = '1', dont_care = 'X' };
template<int N> using VecIter = std::vector<std::array<std::pair<int8_t, int8_t>, N>>::iterator;
inline std::string grayCode[] = {"00", "01", "11", "10"};
inline std::string variables[] = {"A", "B", "C", "D"};

std::pair<int8_t, int8_t> getRealIndices(int8_t, int8_t);
int8_t translateIndices(int8_t, int8_t);
int getCellIndex(int, int, int);
