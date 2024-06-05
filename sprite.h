#pragma once


#ifndef SPRITE_H
#define SPRITE_H

#include <cstdint>

struct Sprite {
    size_t width, height;
    uint8_t* data;
};

struct SpriteAnimation {
    bool loop;
    size_t num_frames;
    size_t frame_duration;
    size_t time;
    Sprite** frames;
};

#endif // SPRITE_H


