#pragma once
#include <iostream>
#include <numeric>
#include <set>
#include <vector>
#include "other.hpp"

class Map4x4;

class KmapCell
{
public:
    KmapCell(Map4x4& kmapObject, int cellIndex);
    void setCellValue(Value cellValue) { this->cellValue = cellValue; }
    Value getCellValue() { return this->cellValue; }
    void decrementPairsNumber() { --this->pairsNumber; }
    void decrementSquareQuadsNumber() { --this->squareQuadsNumber; }
    void decrementRectQuadsNumber() { --this->rectQuadsNumber; }
    void incrementPairsNumber() { ++this->pairsNumber; }
    void incrementSquareQuadsNumber() { ++this->squareQuadsNumber; }
    void incrementRectQuadsNumber() { ++this->rectQuadsNumber; }
    void setDone() { done = true; }
    bool isDone() { return this->done; }
    int getPairsNumber() { return this->pairsNumber; }
    int getSquareQuadsNumber() { return this->squareQuadsNumber; }
    void setSquareQuadsNumber(int number) { this->squareQuadsNumber = number; }
    int getNumberOfPairsWithSharing() { return this->numberOfPairsWithSharing; }
    int getNumberOfSquareQuadsWithSharing() { return this->numberOfSquareQuadsWithSharing; }
    int getNumberOfRectQuadsWithSharing() { return this->numberOfRectQuadsWithSharing; }
    int getRectQuadsNumber() { return this->rectQuadsNumber; }
    int getQuadsNumber() { return this->squareQuadsNumber + this->rectQuadsNumber; }
    int getNumberOfQuadsWithSharing() { 
        return this->numberOfSquareQuadsWithSharing + this->numberOfRectQuadsWithSharing; }
    int getIndex() { return this->cellIndex; }
    std::vector<int>& getPairs() { return this->pairs; }
    Groups& getSquareQuads() { return this->squareQuads; }
    Groups& getRectQuads() { return this->rectQuads; }
    void findPairs();
    void findPairsWithSharing();
    void findPairsWithoutSharing();
    void findSquareQuads();
    void findSquareQuadsWithSharing();
    void findRectQuads();
    void findRectQuadsWithSharing();
    void removePairContainingGivenCellIndex(int);
    void removeSquareQuadContainingGivenCellIndex(int);
    void removeRectQuadContainingGivenCellIndex(int);
    bool hasPairsOrQuads();

private:
    int cellIndex;
    Map4x4& kmapObject;
    Value cellValue{Value::zero};
    bool done{false};
    int pairsNumber{0};
    int squareQuadsNumber{0};
    int rectQuadsNumber{0};
    int numberOfPairsWithSharing{0};
    int numberOfSquareQuadsWithSharing{0};
    int numberOfRectQuadsWithSharing{0};
    std::vector<int> squareQuadsSharingCounters;
    std::vector<int> rectQuadsSharingCounters;
    std::vector<int> pairs;
    Groups squareQuads;
    Groups rectQuads;
    std::vector<int> pairsWithSharing;
    Groups squareQuadsWithSharing;
    Groups rectQuadsWithSharing;
};