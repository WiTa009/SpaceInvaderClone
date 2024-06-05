#pragma once

#ifndef BULLET_H
#define BULLET_H

#include "sprite.h"

class Bullet {
public: 
    size_t x, y;
    int dir;
    Sprite sprite;
    int speed;

    Bullet(size_t x, size_t y, int speed, int dir);

    void move();
};



#endif // BULLET_H
