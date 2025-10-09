#include "Tile.h"

Tile::Tile() : color('B'), number(0), revealed(false) {}

Tile::Tile(char c, int n) : color(c), number(n), revealed(false) {}

bool Tile::operator<(const Tile& other) const {
    if (number != other.number) {
        return number < other.number;
    }
    return color < other.color;
}
