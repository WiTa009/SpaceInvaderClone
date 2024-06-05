
//#include "headers/enemy.h"
//#include "headers/player.h"

#include "headers/game.h"

// Game Constructor
Game::Game(size_t width, size_t height, size_t num_enemies, size_t max_bullets)
	: window_width(width), window_height(height), num_enemies(num_enemies), player_score(0)
{
	player = create_player();
	enemies = create_enemies(num_enemies);

    // Create a list, each alien has a elemet, when element reaches 0, remove ailen
    death_counters = new uint8_t[num_enemies];
    for (size_t i = 0; i < num_enemies; ++i) {
        death_counters[i] = 10;
    }
}

Game::~Game()
{
}

// Initialize "create_player" function
Player Game::create_player() {
    Player player;
    player.x = 112 - 5;
    player.y = 32;
    player.life = 3;
    player.speedX = 2;

    player.sprite.width = 11;
    player.sprite.height = 7;
    player.sprite.data = new uint8_t[77]
    {
        0,0,0,0,0,1,0,0,0,0,0, // .....@.....
        0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
        0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
        0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
    };

    return player;
}

// Initialize "create_enemies" function
Enemy** Game::create_enemies(size_t num_enemies) {


    Enemy** enemies = new Enemy * [num_enemies];

    /*
    const int width = 11;
    const int height = 8;
    uint8_t* data = new uint8_t[11 * 8]
    {
            0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
            0,0,0,1,0,0,0,1,0,0,0, // ...@...@...
            0,0,1,1,1,1,1,1,1,0,0, // ..@@@@@@@..
            0,1,1,0,1,1,1,0,1,1,0, // .@@.@@@.@@.
            1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
            1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
            1,0,1,0,0,0,0,0,1,0,1, // @.@.....@.@
            0,0,0,1,1,0,1,1,0,0,0  // ...@@.@@...
    };

    */
    // 5 Rows
    for (size_t yi = 0; yi < 5; ++yi)
    {
        // 11 Columns
        for (size_t xi = 0; xi < 11; ++xi)
        {
            if (yi == 0) {
                enemies[yi * 11 + xi] = new Eater();
                enemies[yi * 11 + xi]->x = 16 * xi + 20;
                enemies[yi * 11 + xi]->y = 17 * yi + 128;
            }

            if (yi == 1 || yi == 2) {
                enemies[yi * 11 + xi] = new Alien();
                enemies[yi * 11 + xi]->x = 16 * xi + 20;
                enemies[yi * 11 + xi]->y = 17 * yi + 128;
            }

            if (yi == 3 || yi == 4) {
                enemies[yi * 11 + xi] = new DeadAlien();
                enemies[yi * 11 + xi]->x = 16 * xi + 20;
                enemies[yi * 11 + xi]->y = 17 * yi + 128;
            }


            /*
            enemies[yi * 11 + xi].sprite.width = width;
            enemies[yi * 11 + xi].sprite.height = height;
            enemies[yi * 11 + xi].sprite.data = data;
            */
        }
    }

    return enemies;
}
