#pragma once

#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

class Enemy : public Character {
public:
    static Sprite death_animation;
    bool is_dead;
    bool dying;
    int dying_time;
    Enemy(Sprite sprite, size_t X, size_t Y);
    Enemy();

    virtual ~Enemy();
    void setDeathAnimation();

    //void setEnemyAnimation();
};

class Alien : public Enemy {
public:
    static Sprite enemy_animation[2];

    Alien(Sprite sprite, size_t X, size_t Y);
    Alien();
    void setEnemyAnimation();

};

class DeadAlien : public Enemy {
public:
    static Sprite enemy_animation[2];

    DeadAlien(Sprite sprite, size_t X, size_t Y);
    DeadAlien();
    void setEnemyAnimation();

};

class Eater : public Enemy {
public:
    static Sprite enemy_animation[2];

    Eater(Sprite sprite, size_t X, size_t Y);
    Eater();
    void setEnemyAnimation();

};



#endif // ENEMY_H

/*
class Enemy : public Character {
public:
    static Sprite enemyAnimation[2];
    size_t type;

    Enemy(Sprite sprite, size_t X, size_t Y);
    Enemy();

};

*/