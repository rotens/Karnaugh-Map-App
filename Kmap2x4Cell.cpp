#include "Kmap2x4Cell.hpp"
#include "Map2x4.hpp"

Kmap2x4Cell::Kmap2x4Cell(Map2x4& kmapObject, int cellIndex)
        : kmapObject(kmapObject), cellIndex(cellIndex) {}
