#include <cstdint>

#include "headers/bullet.h"

Bullet::Bullet(size_t x, size_t y, int speed, int dir) : x(x), y(y), speed(speed), dir(dir){

	sprite.width = 4;
	sprite.height = 8;
	sprite.data = new uint8_t[4 * 8]
	{
		1,1,1,1,	// @@@@
		1,1,1,1,	// @@@@
		1,1,1,1,	// @@
		1,1,1,1,	// @@@@
		1,1,1,1,	// @@@@
		1,1,1,1,	// @@@@
		1,1,1,1,	// @@@@
		1,1,1,1		// @@@@
	};
}

void Bullet::move() {
	// Move up
	if (dir == 1) {
		y += speed;
	}
	if (dir == -1) {
		y -= speed;

	}
}