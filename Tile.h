#ifndef TILE_H
#define TILE_H

class Tile {
public:
    char color;
    int number;
    bool revealed;

    Tile();
    Tile(char c, int n);
    bool operator<(const Tile& other) const;
};

#endif
