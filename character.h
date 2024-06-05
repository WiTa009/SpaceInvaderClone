#pragma once

#ifndef CHARACTER_H
#define CHARACTER_H

#include "sprite.h"

class Character {
public:
    size_t x, y;
    Sprite sprite;
    

    Character(Sprite sprite, size_t X, size_t Y);
    Character();
};

#endif // CHARACTER_H