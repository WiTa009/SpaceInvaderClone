

#include "headers/character.h"

Character::Character(Sprite sprite, size_t X, size_t Y) : sprite(sprite), x(X), y(Y) {
}

Character::Character() : x(0), y(0), sprite({ 0,0, nullptr }) {
}




/*
#include <cstdint>

struct Sprite
{
    size_t width, height;
    uint8_t* data;
};

class Character {
public:
    size_t x, y;
    Sprite sprite;
    Character(Sprite sprite, size_t X, size_t Y) : sprite(sprite), x(X), y(Y) {
    }

    Character() : x(0), y(0), sprite({0,0, nullptr}) {
    }

};

*/