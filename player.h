#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class Player : public Character {
public:
    size_t life;
    int player_move_dir;
    int speedX;
    bool shooting;

    Player(Sprite sprite, size_t X, size_t Y, size_t life, int speedX);
    Player();

    void move(size_t game_width);

};

#endif // PLAYER_H
