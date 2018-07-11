#include "minesweeper.h"

struct pair_hash {
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};

Minesweeper::Minesweeper() {

}

Minesweeper::Minesweeper(int w, int h, int n, bool givenGrid, vector<vector<int>> g) {
    width = w;
    height = h;
    numMines = n;
    loss = false;

    if(givenGrid) {
        init(true, g);
    }
    else {
        init();
    }
}

//Resets minesweeper grid to default values
void Minesweeper::init(bool givenGrid, vector<vector<int>> g) {
    beliefs = vector<vector<double>>(height, vector<double>(width, 1));
    flipped = vector<vector<bool>>(height, vector<bool>(width, false));
    flags = vector<vector<bool>>(height, vector<bool>(width, false));
    if(givenGrid) {
        grid = g;
        return;
    }
    loss = false;
    grid = vector<vector<int>>(height, vector<int>(width, 0));

    vector<int> pos;
    for(int i = 0; i < numMines; i++) {
        pos = {rand()%height, rand()%width};
        while(grid.at(pos.at(0)).at(pos.at(1)) == -2) {
            pos = {rand()%height, rand()%width};
        }
        grid.at(pos.at(0)).at(pos.at(1)) = -2;
    }

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(grid.at(i).at(j) == -2) {
                continue;
            }
            for(int a = -1; a <= 1; a++) {
                for(int b = -1; b <= 1; b++) {
                    if(i+a < 0 || i+a >= height || j+b < 0 || j+b >= width) {
                        continue;
                    }
                    if(grid.at(i+a).at(j+b) == -2) {
                        grid.at(i).at(j)++;
                    }
                }
            }
        }
    }
    originalGrid = grid;
}

//Flips the tile at the given coordinates and continues flipping all adjacent
//tiles as long as their value is 0
void Minesweeper::flip(int x, int y) {
    flipped.at(x).at(y) = true;
    pair<int, int> curpos(x, y);
    queue<pair<int, int>> q;
    q.push(curpos);
    unordered_set<pair<int, int>, pair_hash> visited;

    int i = 0;
    while(!q.empty()) {
        curpos = q.front();
        q.pop();
        auto search = visited.find(curpos);
        if(search != visited.end()) {
            continue;
        }
        visited.insert(curpos);
        int i = curpos.first;
        int j = curpos.second;
        flipped.at(i).at(j) = true;
        for(int a = -1; a <= 1; a++) {
            for(int b = -1; b <= 1; b++) {
                if(i+a < 0 || i+a >= height || j+b < 0 || j+b >= width || grid.at(i).at(j) != 0) {
                    continue;
                }
                flipped.at(i+a).at(j+b) = true;
                if(grid.at(i+a).at(j+b) == 0) {
                    q.push(pair<int, int>(i+a, j+b));
                }
            }
        }
        i++;
    }
}

//Gets samples of random mine positions to see which positions are most likely
//to contain mines, given the current uncovered tiles
void Minesweeper::updateBeliefs() {
    if(numFlags == numMines) {
        return;
    }
    beliefs = vector<vector<double>>(height, vector<double>(width, 0));
    oldBeliefs = vector<vector<double>>(height, vector<double>(width, 0));

    int numSamples = 2400;
    vector<vector<vector<double>>> samples = vector<vector<vector<double>>>(height, vector<vector<double>>(width, vector<double>(numSamples, 0)));
    do {
#pragma omp parallel for num_threads(8) schedule(dynamic, 100)
        for(int a = 0; a < numSamples; a++) {
            Sampler sampler(grid, flipped, flags, numMines-numFlags);
            for(int i = 0; i < height; i++) {
                for(int j = 0; j < width; j++) {
                    samples.at(i).at(j).at(a) = sampler.sample({i, j});
                }
            }
        }
      for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                for(int a = 0; a < numSamples; a++) {
                    beliefs.at(i).at(j) += samples.at(i).at(j).at(a);
                }
            }
        }
    } while(continueSampling());
}

bool Minesweeper::continueSampling() {
    double maxBeliefs = 0;
    double maxOldBeliefs = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(!flipped.at(i).at(j)) {
                maxBeliefs = max(maxBeliefs, beliefs.at(i).at(j));
                maxOldBeliefs = max(maxOldBeliefs, oldBeliefs.at(i).at(j));
            }
        }
    }
    if(maxBeliefs == maxOldBeliefs) {
        oldBeliefs = beliefs;
        return true;
    }
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(!flipped.at(i).at(j)) {
                if(abs(beliefs.at(i).at(j)/(double)max(maxBeliefs, 1.0) - oldBeliefs.at(i).at(j)/(double)max(maxOldBeliefs, 1.0)) > 0.01) {
                    oldBeliefs = beliefs;
                    return true;
                }
            }
        }
    }
    oldBeliefs = beliefs;
    return false;
}

//Normalizes beliefs by making greatest belief equal to 1 with all other
//beliefs being proportional
double Minesweeper::normalizeBeliefs() {
    double maxBeliefs = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(!flipped.at(i).at(j)) {
                maxBeliefs = max(maxBeliefs, beliefs.at(i).at(j));
            }
        }
    }
    return maxBeliefs;
}

//Flip the tile that has the smallest probability of holding a mine
bool Minesweeper::actOnBeliefs() {
    double minval = INT_MAX;
    vector<int> argmin = {0, 0};
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            double belief = beliefs.at(i).at(j);
            if(belief <= minval && !flipped.at(i).at(j) && !flags.at(i).at(j)) {
                minval = belief;
                argmin = {i, j};
            }
        }
    }

    flip(argmin.at(0), argmin.at(1));
    return grid.at(argmin.at(0)).at(argmin.at(1)) == -2;
}

//Place flags on all tiles that have 100% chance of having a mine
void Minesweeper::placeFlags() {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(!flipped.at(i).at(j) || flipped.at(i).at(j) && grid.at(i).at(j) == 0) {
                continue;
            }
            int activeTileCount = 0;
            for(int a = -1; a <= 1; a++) {
                for(int b = -1; b <= 1; b++) {
                    if(i+a < 0 || i+a >= height || j+b < 0 || j+b >= width || flipped.at(i+a).at(j+b)) {
                        continue;
                    }
                    activeTileCount++;
                }
            }
            if(activeTileCount == grid.at(i).at(j)) {
                for(int a = -1; a <= 1; a++) {
                    for(int b = -1; b <= 1; b++) {
                        if(i+a < 0 || i+a >= height || j+b < 0 || j+b >= width || flipped.at(i+a).at(j+b)) {
                            continue;
                        }
                        flags.at(i+a).at(j+b) = true;
                    }
                }
            }
        }
    }
    numFlags = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(flags.at(i).at(j)) {
                numFlags++;
            }
        }
    }
}

//Returns true if the game is over
bool Minesweeper::finished() {
    int remainingCount = 0;
    int unflippedCount = 0;
    int flagCount = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(beliefs.at(i).at(j) > 0 && !flipped.at(i).at(j)) {
                remainingCount++;
            }
            if(!flipped.at(i).at(j)) {
                unflippedCount++;
            }
        }
    }
    return (numFlags == numMines) && (numMines == unflippedCount);
}
