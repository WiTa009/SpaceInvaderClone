#pragma once

#ifndef GAME_H
#define GAME_H

#include "enemy.h"
#include "player.h"
#include "bullet.h"


class Game {
public:
    size_t window_width, window_height;
    size_t num_enemies;
    Enemy** enemies; // Array of pointers to Enemy objects
    size_t num_bullets;
    Player player;

    size_t player_score;

    uint8_t* death_counters;

    //Bullet player_bullets[10]; // Array of bullets

    Game(size_t width, size_t height, size_t num_enemies, size_t max_bullets);
    ~Game(); // Destructor to clean up enemies array

    Enemy** create_enemies(size_t num_enemies);
    Player create_player();
  
};



#endif // GAME_H