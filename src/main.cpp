#include <unistd.h>
#include <iostream>
#include <thread>
#include "minesweeper.h"
//#include "sampler.h"
#include "window.h"

using namespace std;

void comp_loop(Minesweeper *ms, Window *gui) {

    while(1) {
        while(!ms->finished()) {
            if(ms->actOnBeliefs()) {
                ms->loss = true;
                break;
            }
            ms->placeFlags();
            ms->updateBeliefs();
            gui->maxBeliefs = ms->normalizeBeliefs();
            gui->finishedUpdate = false;
            gui->update();
            int blockCompiler = 0;
            while(!gui->finishedUpdate) { blockCompiler++; }
        }

        ms->normalizeBeliefs();
        gui->update();
        gui->show();

        cout << "Press Enter to restart";
        getchar();

        ms->init();
    }
}

int main(int argc, char **argv) {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    srand(time.tv_nsec);
    QApplication app(argc, argv);

    int msWidth = 8;
    int msHeight = 8;
    int numMines = 10;
    //vector<vector<int>> g = {{-2, -2, 2, 1, 1}, {2, 2, 2, -2, 1}, {0, 1, 2, 2, 1}, {1, 2, -2, 1, 0}, {1, -2, 2, 1, 0}};
    Minesweeper ms = Minesweeper(msWidth, msHeight, numMines/*, true, g*/);
    //ms.flipped.at(0).at(0) = true;

    int winWidth = 800;
    int winHeight = 800;
    QWidget widget;
    Window gui;
    gui.init(winWidth, winHeight, &ms);

    gui.resize(winWidth, winHeight);
    //gui.update();
    gui.show();

    thread comp(comp_loop, &ms, &gui);

    app.exec();
}
