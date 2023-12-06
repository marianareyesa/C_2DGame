/*
 * File: game.cpp
 * Author: Alessandra Gorla
 * Date: November 21, 2023
 * Description: Game class to deal with initialization and controller of 2D my game application.
 */
#include <iostream>
#include "game.h"
#include "move.h"

const float Game::SCENE_WIDTH = 1000.0f;
const float Game::SCENE_HEIGHT = 800.0f;
const float Game::PLAYER_START_X = 400.0f;
const float Game::PLAYER_START_Y = 300.0f;
const float Game::RADIUS = 40.0f;
const float Game::SPEED = 150.0f;

Game::Game() {
    initWindow();
    initBackground();
    initPlayer();
}
/**
 * Window initializer.
 */
int Game::initWindow() {
    window.create(sf::VideoMode(SCENE_WIDTH, SCENE_HEIGHT), "MkM Game");
    window.setFramerateLimit(120);
    return 0;
}
/**
 * Background initializer.
 */
int Game::initBackground() {
    if (!backgroundTexture.loadFromFile("resources/background.png")) {
        return 1;
    }
    backgroundTexture.setRepeated(true);
    background.setTexture(backgroundTexture);
    background.setTextureRect(sf::IntRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT));
    return 0;
}

/**
 * Player (e.g. PacMan) initializer
 * @return 0 if successfully initialized, 1 otherwise
 */
int Game::initPlayer() {
    player.setRadius(RADIUS);
    player.setOrigin(RADIUS, RADIUS);
    player.setPosition(PLAYER_START_X, PLAYER_START_Y);
    if (!playerTexture.loadFromFile("resources/donut.png")) {
        return 1;
    }
    player.setTexture(&playerTexture);
    return 0;
}

/**
 * Dealing with events on window.
 */
void Game::processInput() {

    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
        }
    }
}

/**
 * Function to update the position of the player
 */
void Game::update(sf::Time delta, sf::Shape &player) {
    bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    bool moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

    Coordinate playerPosition = {player.getPosition().x, player.getPosition().y};
   
    Coordinate newPosition = move(playerPosition, SPEED, 0.001f * delta.asMilliseconds(), moveLeft, moveRight, moveUp, moveDown);
    
    // Ensure the player stays within the scene boundaries
    if (newPosition.x - RADIUS >= 0 && newPosition.x + RADIUS <= SCENE_WIDTH &&
        newPosition.y - RADIUS >= 0 && newPosition.y + RADIUS <= SCENE_HEIGHT) {
        player.setPosition(newPosition.x, newPosition.y);
    }
}

void Game::updatePlayerPosition(sf::Vector2f velocity) {
    sf::Vector2f newPosition = player.getPosition() + velocity;

    // Ensure the player stays within the scene
    if (newPosition.x - RADIUS >= 0 && newPosition.x + RADIUS <= SCENE_WIDTH &&
        newPosition.y - RADIUS >= 0 && newPosition.y + RADIUS <= SCENE_HEIGHT) {
        player.setPosition(newPosition);
    }
}

/**
 * Render elements in the window
 */
void Game::render() {
    window.clear(sf::Color::White);
    window.draw(background);
    window.draw(player);
    window.display();
}
/**
 * Main function to deal with events, update the player and render the updated scene on the window.
 */
int Game::run() {
    while (window.isOpen()) {
        sf::Time delta = clock.restart();
        processInput();
        update(delta, player);
	render();
    }
    return 0;
}
