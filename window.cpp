#include "window.h"

void Window::init(int w, int h, Minesweeper *_ms) {
    ms = _ms;
    //finishedUpdate = false;
    int playAreaHeight = h-50;
    tileSize = min(w/ms->width, playAreaHeight/ms->height);

    QPixmap spritesheet("C:\\Users\\mwozn\\Documents\\minesweeper-inference-cpp\\assets\\sprites.png");
    QPixmap beliefsheet("C:\\Users\\mwozn\\Documents\\minesweeper-inference-cpp\\assets\\beliefs.png");

    for(int i = 0; i < 12; i++) {
        QRect rect(i*13, 0, 13, 26);
        numbers.push_back(spritesheet.copy(rect));
    }

    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 9; j++) {
            QRect rect(j*16, 23+i*16, 16, 16);
            sprites.push_back(spritesheet.copy(rect));
        }
    }

    for(int i = 0; i < 9; i++) {
        opened.push_back(sprites.at(i));
    }

    beliefs.push_back(sprites.at(9));
    for(int i = 0; i < 20; i++) {
        QRect rect(i*16, 0, 16, 16);
        beliefs.push_back(beliefsheet.copy(rect));
    }

    unopened = sprites.at(9);
    flag = sprites.at(10);
    explodedMine = sprites.at(11);
    wrongMine = sprites.at(12);
    mine = sprites.at(13);

    offsx = (w - ms->width * tileSize)/2;
    offsy = (playAreaHeight - ms->height * tileSize)/2 + 50;
}

void Window::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    for(int i = 0; i < ms->height; i++) {
        for(int j = 0; j < ms->width; j++) {
            QRect dst(j*tileSize+offsx, i*tileSize+offsy, tileSize, tileSize);
            if(ms->loss) {
                if(!ms->flipped.at(i).at(j)) {
                    painter.drawPixmap(dst, flag);
                }
                else if(ms->flipped.at(i).at(j)) {
                    if(ms->grid.at(i).at(j) == -2) {
                        painter.drawPixmap(dst, explodedMine);
                    }
                    else {
                        painter.drawPixmap(dst, opened[ms->grid.at(i).at(j)]);
                    }
                }
            }
            else if(ms->flags.at(i).at(j)) {
                painter.drawPixmap(dst, flag);
            }
            else if(ms->flipped.at(i).at(j)) {
                painter.drawPixmap(dst, opened[ms->grid.at(i).at(j)]);
            }
            else if(true){
                //qDebug() << maxBeliefs << " " << ms->beliefs.at(i).at(j) / maxBeliefs * 19.0 << "\n";
                painter.drawPixmap(dst, beliefs[(int)(ms->beliefs.at(i).at(j) / maxBeliefs * 19.0)]);
            }
            else {
                painter.drawPixmap(dst, unopened);
            }
            //painter.drawPixmap(dst, beliefs.at(0));
        }
    }

    for(int i = 0; i < 3; i++) {
        QRect dst(offsx+10+20*i, 25, 13, 26);
        //painter.drawPixmap(dst, numbers.at(ms->numMines/100));
    }

    finishedUpdate = true;
}

void Window::lossScreen(Minesweeper ms) {
    loss = true;
}
