# About

## About Minesweeper

Minesweeper is a game about uncovering mines on a game board by using hints that are uncovered as the game is played. These hints come in the form of numbered tiles that tell you how many of the eight adjacent tiles contain mines. The game ends when all tiles are uncovered, except for the tiles covering mines.

## About this app

This application solves a game of minesweeper by uncovering tiles that are statistically least likely to contain a mine. This is done by finding the probability that a tile contains a mine (the darker a tile is, the greater the certainty that the tile contains a mine). The following formula is used:

<a href="https://www.codecogs.com/eqnedit.php?latex=P(tile\&space;contains\&space;mine\&space;|\&space;all\&space;uncovered\&space;tiles)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?P(tile\&space;contains\&space;mine\&space;|\&space;all\&space;uncovered\&space;tiles)" title="P(tile contains mine | all uncovered tiles)" /></a>

and uncovering the tile with the lowest probability. This probability can be derived using Bayes' theorem:

<a href="https://www.codecogs.com/eqnedit.php?latex=P(tile\&space;contains\&space;mine\&space;|\&space;all\&space;revealed\&space;tiles)\&space;=\&space;P(all\&space;revealed\&space;tiles\&space;|\&space;tile\&space;contains\&space;mine)\&space;\cdot\&space;P(tile\&space;contains\&space;mine)\&space;/\&space;P(all\&space;revealed\&space;tiles)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?P(tile\&space;contains\&space;mine\&space;|\&space;all\&space;revealed\&space;tiles)\&space;=\&space;P(all\&space;revealed\&space;tiles\&space;|\&space;tile\&space;contains\&space;mine)\&space;\cdot\&space;P(tile\&space;contains\&space;mine)\&space;/\&space;P(all\&space;revealed\&space;tiles)" title="P(tile contains mine | all revealed tiles) = P(all revealed tiles | tile contains mine) \cdot P(tile contains mine) / P(all revealed tiles)" /></a>

Or, more simply,

<a href="https://www.codecogs.com/eqnedit.php?latex=P(tile\&space;contains\&space;mine\&space;|\&space;all\&space;revealed\&space;tiles)\&space;\propto\&space;P(all\&space;revealed\&space;tiles\&space;|\&space;tile\&space;contains\&space;mine)\&space;\cdot\&space;P(tile\&space;contains\&space;mine)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?P(tile\&space;contains\&space;mine\&space;|\&space;all\&space;revealed\&space;tiles)\&space;\propto\&space;P(all\&space;revealed\&space;tiles\&space;|\&space;tile\&space;contains\&space;mine)\&space;\cdot\&space;P(tile\&space;contains\&space;mine)" title="P(tile contains mine | all revealed tiles) \propto P(all revealed tiles | tile contains mine) \cdot P(tile contains mine)" /></a>

And, since all tiles have an equal chance of containing a mine given no other information,

<a href="https://www.codecogs.com/eqnedit.php?latex=P(tile\&space;contains\&space;mine\&space;|\&space;all\&space;revealed\&space;tiles)\&space;\propto\&space;P(all\&space;revealed\&space;tiles\&space;|\&space;tile\&space;contains\&space;mine)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?P(tile\&space;contains\&space;mine\&space;|\&space;all\&space;revealed\&space;tiles)\&space;\propto\&space;P(all\&space;revealed\&space;tiles\&space;|\&space;tile\&space;contains\&space;mine)" title="P(tile contains mine | all revealed tiles) \propto P(all revealed tiles | tile contains mine)" /></a>

Therefore, the probability that a tile contains a mine given all of the revealed evidence is proportional to the probability that the revealed evidence is true given that the tile contains a mine. 

The simplest way to use this to find the correct probability is to observe every possible permutation of the mines in which one of the mines are fixed to the tile for which the probability is being calculated, and count how many of those permutations are valid with respect to the uncovered evidence tiles. This count would then be divided by the total number of permutations observed.

<a href="https://www.codecogs.com/eqnedit.php?latex=P(all\&space;revealed\&space;tiles\&space;|\&space;this\&space;tile\&space;contains\&space;mine)&space;=&space;(count\&space;of\&space;valid\&space;mine\&space;placements\&space;with\&space;one\&space;mine\&space;fixed\&space;to\&space;this\&space;tile)\&space;/\&space;(count\&space;of\&space;all\&space;mine\&space;placements\&space;with\&space;one\&space;mine\&space;fixed\&space;to\&space;this\&space;tile)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?P(all\&space;revealed\&space;tiles\&space;|\&space;this\&space;tile\&space;contains\&space;mine)&space;=&space;(count\&space;of\&space;valid\&space;mine\&space;placements\&space;with\&space;one\&space;mine\&space;fixed\&space;to\&space;this\&space;tile)\&space;/\&space;(count\&space;of\&space;all\&space;mine\&space;placements\&space;with\&space;one\&space;mine\&space;fixed\&space;to\&space;this\&space;tile)" title="P(all\ revealed\ tiles\ |\ this\ tile\ contains\ mine) = (count\ of\ valid\ mine\ placements\ with\ one\ mine\ fixed\ to\ this\ tile)\ /\ (count\ of\ all\ mine\ placements\ with\ one\ mine\ fixed\ to\ this\ tile)" /></a>

However, this method becomes impossible for any decently sized Minesweeper game. The total number of mine placements that would have to be observed for each tile is

<a href="https://www.codecogs.com/eqnedit.php?latex=\binom{number\&space;of\&space;tiles\&space;-\&space;1}{number\&space;of\&space;mines\&space;-\&space;1}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\binom{number\&space;of\&space;tiles\&space;-\&space;1}{number\&space;of\&space;mines\&space;-\&space;1}" title="\binom{number\ of\ tiles\ -\ 1}{number\ of\ mines\ -\ 1}" /></a>

which, for the "easy" difficulty in normal Minesweeper games (8x8 grid, 10 mines) is equal to 23,667,689,815 (23 trillion!!) observation, per tile! This makes finding the exact probability that a tile contains a mine basically impossible within a reasonable timeframe on a personal computer.

This app tries to remedy this by taking a sample of random mine positions instead of observing every possible mine position. Presumably, taking a large enough sample of possible mine positions allows for a precise enough calculation to approximate finding the exact probability. This application repeatedly takes samples, normalizes them, and compares them to previous samples. If no tile's probability changed by more than 0.01, the beliefs are considered "converged" and are acted upon by choosing the tile with the lowest probability of holding a mine.

# Performance features

Since solving Minesweeper is very performance heavy, certain performance considerations needed to be made:

## Sampling

Random samples of mine positions are taken instead of observing every possible mine position in order to approximate the probability that a tile contains a mine. Since the probabilities of each tile are independent of each other, one sample is made and reused once for every tile on the board. When a sample is made, (number of mines)-1 mines are randomly distributed across the game grid. When the sample is used, a mine is either placed in the position of the tile whose probability is being calculated (if there isn't already a mine there) or is placed somewhere on the board randomly (if there already is a mine there). Then, the sample is evaluated against all of the revealed evidence to determine whether the sample is valid; if it is, the count is incremented. After all samples are made and validated, the probability is estimated by dividing the count by the number of samples. Samples are taken until convergence, when the generated beliefs are then acted upon.

## Multithreading

Every sample is completely independent from every other sample, resource-wise. This means that each sample can be run on a different thread, with the only shared resource being the "valid" count for each tile. This application implements multithreading using omp pragmas on eight threads. The "valid" counts are kept on memory locations that are exclusive to each sample to mitigate true- and false-sharing in order to minimize bus traffic.

# How to run

The executable and all dependencies are located in win.7z. Run MinesweeperInference.exe to run the app.

# Technologies

Written in C++
GUI created with QT
Multithreading implemented with OMP