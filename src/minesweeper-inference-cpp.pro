TEMPLATE = app
TARGET = MinesweeperInference

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console \
    static

SOURCES += main.cpp minesweeper.cpp \
    window.cpp \
    sampler.cpp
HEADERS += minesweeper.h \
    window.h \
    sampler.h

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
