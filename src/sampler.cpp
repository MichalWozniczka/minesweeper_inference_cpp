#include "sampler.h"

Sampler::Sampler(){

}

Sampler::Sampler(vector<vector<int> > g, vector<vector<bool> > f, vector<vector<bool>> fl, int n) {
    grid = g;
    flipped = f;
    flags = fl;
    height = g.size();
    width = g.at(0).size();
    valid = true;
    tileCount = 0;
    mines = vector<vector<bool>>(height, vector<bool>(width, false));

    vector<int> pos;
    for(int i = 0; i < n-1; i++) {
        pos = {rand()%height, rand()%width};
        while(mines.at(pos.at(0)).at(pos.at(1)) || flipped.at(pos.at(0)).at(pos.at(1)) || flags.at(pos.at(0)).at(pos.at(1))) {
            pos = {rand()%height, rand()%width};
        }
        mines.at(pos.at(0)).at(pos.at(1)) = true;
    }
    sparePos = {rand()%height, rand()%width};
    while(mines.at(sparePos.at(0)).at(sparePos.at(1)) || flipped.at(sparePos.at(0)).at(sparePos.at(1)) || flags.at(sparePos.at(0)).at(sparePos.at(1))) {
        sparePos = {rand()%height, rand()%width};
    }

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(mines.at(i).at(j) || flags.at(i).at(j)) {
                if(flipped.at(i).at(j)) {
                    valid = false;
                    continue;
                }
                for(int a = -1; a <= 1; a++) {
                    for(int b = -1; b <= 1; b++) {
                        if(i+a < 0 || i+a >= height || j+b < 0 || j+b >= width) {
                            continue;
                        }
                        if(flipped.at(i+a).at(j+b)) {
                            grid.at(i+a).at(j+b)--;
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(flipped.at(i).at(j)) {
                if(grid.at(i).at(j) < 0) {
                    valid = false;
                }
                if(grid.at(i).at(j) > 0) {
                    tileCount += grid.at(i).at(j);
                }
            }
        }
    }
}

bool Sampler::sample(vector<int> pos) {
    int tc = tileCount;

    if(flipped.at(pos.at(0)).at(pos.at(1))) {
        return false;
    }

    if(valid) {
        if(mines.at(pos.at(0)).at(pos.at(1))) {
            pos = sparePos;
        }

        int i = pos.at(0);
        int j = pos.at(1);

        for(int a = -1; a <= 1; a++) {
            for(int b = -1; b <= 1; b++) {
                if(i+a < 0 || i+a >= height || j+b < 0 || j+b >= width) {
                    continue;
                }
                if(flipped.at(i+a).at(j+b)) {
                    if(grid.at(i+a).at(j+b)-1 < 0) {
                        return false;
                    }
                    tc--;
                }
            }
        }
    }

    return valid && (tc == 0);
}
