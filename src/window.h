#ifndef GUI_H
#define GUI_H
#include <QApplication>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QLabel>
#include <QWidget>
#include <string>
#include <vector>
#include <iostream>
#include "minesweeper.h"

using namespace std;

class Window : public QWidget {
    Q_OBJECT
private:
    int width, height, tileSize, offsx, offsy;
    Minesweeper *ms;
    vector<QPixmap> numbers, sprites, beliefs, opened;
    QPixmap unopened, flag, explodedMine, wrongMine, mine;
    bool loss;

protected:
    void paintEvent(QPaintEvent*);

public:
    double maxBeliefs;
    bool finishedUpdate;

    void init(int w, int h, Minesweeper *ms);
    void lossScreen(Minesweeper ms);
};

#endif // GUI_H
