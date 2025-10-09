#ifndef GAME_H
#define GAME_H

#include "Tile.h"
#include <vector>
using namespace std;

class Game {
private:
    vector<Tile> deck;
    vector<Tile> playerTiles;
    vector<Tile> computerTiles;

    Tile lastDrawnPlayer;
    Tile lastDrawnComputer;
    bool playerHasDrawn;
    bool computerHasDrawn;

    void initDeck();
    void dealInitialTiles();
    void displayTiles(const vector<Tile>& tiles, bool showAll);
    void displayBoard();

    bool isGameOver();
    int countHiddenTiles(const vector<Tile>& tiles);

    void playerTurn();
    void computerTurn();
    void showResult();

public:
    Game();
    void play();
};

#endif
