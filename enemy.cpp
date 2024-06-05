
#include <cstdint>

#include "headers/enemy.h"

Enemy::Enemy(Sprite sprite, size_t X, size_t Y) : Character(sprite, X, Y), is_dead(false), dying(false), dying_time(1000) {
    setDeathAnimation();
}

Enemy::Enemy() : Character(), is_dead(false), dying(false), dying_time(1000) {
    setDeathAnimation();
}

Enemy::~Enemy()
{
}

Sprite Enemy::death_animation;


void Enemy::setDeathAnimation() {
    uint8_t* data = new uint8_t[11 * 8]
    {
        0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, // ..@.....@..
            1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, // @..@...@..@
            1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, // @.@@@@@@@.@
            1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, // @@@.@@@.@@@
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@
            0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // .@@@@@@@@@.
            0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, // ..@.....@..
            0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0  // .@.......@.
    };
    death_animation.height = 7;
    death_animation.width = 13;
    death_animation.data = data;
}


Alien::Alien(Sprite sprite, size_t X, size_t Y) : Enemy(sprite, X, Y){
    setEnemyAnimation();
}

Alien::Alien() : Enemy() {
    setEnemyAnimation();

}

Sprite Alien::enemy_animation[2];

void Alien::setEnemyAnimation() {

    uint8_t* data0 = new uint8_t[11 * 8]
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

    uint8_t* data1 = new uint8_t[11 * 8]
    {
        0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, // ..@.....@..
            1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, // @..@...@..@
            1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, // @.@@@@@@@.@
            1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, // @@@.@@@.@@@
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // @@@@@@@@@@@
            0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // .@@@@@@@@@.
            0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, // ..@.....@..
            0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0  // .@.......@.
    };

    enemy_animation[0].width = 11;
    enemy_animation[1].width = 11;
    enemy_animation[0].height = 8;
    enemy_animation[1].height = 8;

    enemy_animation[0].data = data0;
    enemy_animation[1].data = data1;

}


DeadAlien::DeadAlien(Sprite sprite, size_t X, size_t Y) : Enemy(sprite, X, Y) {
    setEnemyAnimation();
}

DeadAlien::DeadAlien() : Enemy() {
    setEnemyAnimation();

}

Sprite DeadAlien::enemy_animation[2];

void DeadAlien::setEnemyAnimation() {

    uint8_t* data0 = new uint8_t[64]
    {
        0,0,0,1,1,0,0,0, // ...@@...
        0,0,1,1,1,1,0,0, // ..@@@@..
        0,1,1,1,1,1,1,0, // .@@@@@@.
        1,1,0,1,1,0,1,1, // @@.@@.@@
        1,1,1,1,1,1,1,1, // @@@@@@@@
        0,1,0,1,1,0,1,0, // .@.@@.@.
        1,0,0,0,0,0,0,1, // @......@
        0,1,0,0,0,0,1,0  // .@....@.
    };

    uint8_t* data1 = new uint8_t[64]
    {
        0,0,0,1,1,0,0,0, // ...@@...
        0,0,1,1,1,1,0,0, // ..@@@@..
        0,1,1,1,1,1,1,0, // .@@@@@@.
        1,1,0,1,1,0,1,1, // @@.@@.@@
        1,1,1,1,1,1,1,1, // @@@@@@@@
        0,0,1,0,0,1,0,0, // ..@..@..
        0,1,0,1,1,0,1,0, // .@.@@.@.
        1,0,1,0,0,1,0,1  // @.@..@.@
    };

    enemy_animation[0].width = 8;
    enemy_animation[1].width = 8;
    enemy_animation[0].height = 8;
    enemy_animation[1].height = 8;

    enemy_animation[0].data = data0;
    enemy_animation[1].data = data1;

}


Eater::Eater(Sprite sprite, size_t X, size_t Y) : Enemy(sprite, X, Y) {
    setEnemyAnimation();
}

Eater::Eater() : Enemy() {
    setEnemyAnimation();

}

Sprite Eater::enemy_animation[2];

void Eater::setEnemyAnimation() {

    uint8_t* data0 = new uint8_t[96]
    {
        0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
        0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        0,0,0,1,1,0,0,1,1,0,0,0, // ...@@..@@...
        0,0,1,1,0,1,1,0,1,1,0,0, // ..@@.@@.@@..
        1,1,0,0,0,0,0,0,0,0,1,1  // @@........@@
    };

    uint8_t* data1 = new uint8_t[96]
    {
        0,0,0,0,1,1,1,1,0,0,0,0, // ....@@@@....
        0,1,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@@.
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        1,1,1,0,0,1,1,0,0,1,1,1, // @@@..@@..@@@
        1,1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@@
        0,0,1,1,1,0,0,1,1,1,0,0, // ..@@@..@@@..
        0,1,1,0,0,1,1,0,0,1,1,0, // .@@..@@..@@.
        0,0,1,1,0,0,0,0,1,1,0,0  // ..@@....@@..
    };

    enemy_animation[0].width = 12;
    enemy_animation[1].width = 8;
    enemy_animation[0].height = 12;
    enemy_animation[1].height = 8;

    enemy_animation[0].data = data0;
    enemy_animation[1].data = data1;

}

/*

#include "character.cpp"

class Enemy : public Character {
public:
    uint8_t type;
    Enemy(Sprite sprite, size_t X, size_t Y) : Character(sprite, X, Y) {
        type = 0;
    }
    Enemy() : Character(), type(0) {
    }

};

*/