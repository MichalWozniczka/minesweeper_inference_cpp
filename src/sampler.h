#ifndef SAMPLER_H
#define SAMPLER_H

#include <stdlib.h>
#include <time.h>
#include "minesweeper.h"

using namespace std;

class Sampler
{
    vector<vector<int>> grid;
    vector<vector<bool>> flipped, mines, flags;
    vector<int> sparePos;
    int width, height, tileCount;
    bool valid;

public:
    Sampler();
    Sampler(vector<vector<int>> g, vector<vector<bool>> f, vector<vector<bool>> fl, int n);

    bool sample(vector<int> pos);
};

#endif // SAMPLER_H
