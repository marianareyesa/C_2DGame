#include "move.h"

Coordinate add(Coordinate a, Coordinate b) {
    Coordinate c = { 0, 0 };
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

Coordinate mul(float s, Coordinate a) {
    Coordinate b = { 0, 0 };
    b.x = s * a.x;
    b.y = s * a.y;
    return b;
}

Coordinate move(Coordinate position, float speed, float delta, bool moveLeft, bool moveRight, bool moveUp, bool moveDown) {
    // Calculate the velocity based on arrow key inputs
    Coordinate velocity = { 0.0f, 0.0f};
    if (moveLeft) {
        velocity.x -= speed;
    }
    if (moveRight) {
        velocity.x += speed;
    }
    if (moveUp) {
        velocity.y -= speed;
    }
    if (moveDown) {
        velocity.y += speed;
    }    

    return add(position, mul(delta, velocity));
}
