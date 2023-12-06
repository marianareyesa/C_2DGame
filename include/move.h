/*
 * File: move.h
 * Author: Alessandra Gorla
 * Date: November 22, 2023
 * Description: Move header.
 */

struct Coordinate {
    float x;
    float y;
};

Coordinate add(Coordinate x, Coordinate y);

Coordinate mul(float s, Coordinate a);

Coordinate move(Coordinate position, float speed, float delta, bool moveLeft, bool moveRight, bool moveUp, bool moveDown);
