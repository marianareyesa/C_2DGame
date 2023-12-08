/*
 * File: game.h
 * Author: Alessandra Gorla
 * Date: November 21, 2023
 * Description: Game class header.
 */
#pragma once
#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    int run();

private:
    static const int MAX_GHOSTS = 5;

    sf::RenderWindow window;
    sf::Sprite background;
    sf::Texture backgroundTexture;
    sf::Texture playerTexture;
    sf::Texture ghostTexture;
    sf::CircleShape player;
    sf::CircleShape ghosts[MAX_GHOSTS]; // Array to hold multiple ghosts
    sf::Vector2f ghostDirections[MAX_GHOSTS]; // Hold directions for each ghost
    bool needsNewDirections[MAX_GHOSTS]; // Track if a new direction is needed
    sf::CircleShape ghost;
    sf::Clock clock;
    bool needsNewDirection;
    sf::Vector2f ghostDirection;

    static const float SCENE_WIDTH;
    static const float SCENE_HEIGHT;
    static const float PLAYER_START_X;
    static const float PLAYER_START_Y;
    static const float RADIUS;
    static const float SPEED;

    int initWindow();
    int initBackground();
    int initPlayer();
    int initNPC();
    void processInput();
    sf::Vector2f generateRandomDirection();
    bool checkCollision(const sf::Shape& shape1, const sf::Shape& shape2);
    void update(sf::Time delta, sf::Shape &player);
    void updatePlayerPosition(sf::Vector2f velocity);
    void render();
};
