

#include "headers/player.h"

// Define constructor with parameters
Player::Player(Sprite sprite, size_t X, size_t Y, size_t life, int speedX) : Character(sprite, X, Y), life(life), player_move_dir(1), shooting(false) {
}

// Define default constructor
Player::Player() : Character(), life(0), player_move_dir(1), speedX(1), shooting(false) {
}

void Player::move(size_t game_width) {

    // RIGHT
    if (player_move_dir == 1) {
        // Player is to the Right <--
        // Stop from moving outside screen
        if (x + sprite.width + speedX >= game_width - 1) {
            x = game_width - sprite.width - 1;
            player_move_dir = 0;

        }
        else {
            // move player RIGHT
            x += speedX;
        }
    }
    // LEFT
    else if (player_move_dir == -1) {
        // Player is to the Left <--
        // Stop from moving outside screen
        if (x + speedX <= 0) {
            x = 0;
            player_move_dir = 0;

        }
        else {
            // move player LEFT
            x -= speedX;

        }
    }

    /*
    // Move player in the LEFT direction
    if (player_move_dir == - 1){
        // Player is to the Right -->
        if (x + sprite.width + player_move_dir >= game_width - 1) {
            x = game_width - sprite.width - player_move_dir - 1;
            player_move_dir = 0;
        }
    }

    else if (player_move_dir == 1){
        // Player is to the Left <--

    }

    else {

    }

    if (x + sprite.width + player_move_dir >= game_width - 1) {
    }

        // Player is to the Left <--
    else if (x + player_move_dir <= 0) {
    }

    if (x + sprite.width + player_move_dir >= game_width - 1) {
        // Player is to the Right <--
        // Move Player to the Left -->
        x = game_width - sprite.width - player_move_dir - 1;
        player_move_dir *= -1;

    }
    else if (x + player_move_dir <= 0) {
        // Player is to the Left <--
        // Move Player to the Right -->
        x = 0;
        player_move_dir *= -1;
    }
    else x += player_move_dir;
    */
}


 