#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <vector>
#include <queue>
#include <utility>
#include <unordered_set>
#include <climits>
#include <iostream>
#include <QDebug>
#include <omp.h>
#include <ctime>

#include "sampler.h"

using namespace std;

class Minesweeper
{
public:
    int width, height, numMines, numFlags;
    vector<vector<int>> grid;
    vector<vector<int>> originalGrid;
    vector<vector<double>> beliefs;
    vector<vector<double>> oldBeliefs;
    vector<vector<bool>> flipped;
    vector<vector<bool>> flags;
    bool finishedNormalizing;
    bool loss;

    Minesweeper();
    Minesweeper(int w, int h, int n, bool givenGrid = false, vector<vector<int>> g = vector<vector<int>>());

    void init(bool givenGrid = false, vector<vector<int>> g = vector<vector<int>>());
    void flip(int x, int y);
    void updateBeliefs();
    bool continueSampling();
    double normalizeBeliefs();
    bool actOnBeliefs();
    void placeFlags();
    bool finished();
};

#endif // MINESWEEPER_H
