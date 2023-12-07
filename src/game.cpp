/*
 * File: game.cpp
 * Author: Alessandra Gorla
 * Date: November 21, 2023
 * Description: Game class to deal with initialization and controller of 2D my game application.
 */
#include <iostream>
#include "game.h"
#include "move.h"
#include <cmath>

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
    initNPC();
    needsNewDirection = true; // Initialize needsNewDirection to indicate a new direction is needed initially
    ghostDirection = generateRandomDirection(); // Initialize direction with a random direction
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
* Evil donuts initializer
*/
int Game::initNPC() {
    ghost.setRadius(RADIUS);
    ghost.setOrigin(RADIUS, RADIUS);
    ghost.setPosition(PLAYER_START_X + 10, PLAYER_START_Y + 10);
    if (!ghostTexture.loadFromFile("resources/evil_donut.png")) {
        return 1;
    }
    ghost.setTexture(&ghostTexture);
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

// Function to generate random directions for ghost's movement
sf::Vector2f Game::generateRandomDirection() {
    // Generate random directions for x and y components
    float randX = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) - 0.5f; // Random value between -0.5 and 0.5
    float randY = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) - 0.5f; // Random value between -0.5 and 0.5
    
    // Adjust the random direction based on the current ghost position to prevent going out of bounds
    sf::Vector2f direction(randX, randY);

    // Get the ghost's current position
    sf::Vector2f ghostPosition = ghost.getPosition();

    // Define bounds based on ghost radius and scene size
    float minX = ghost.getRadius();
    float maxX = SCENE_WIDTH - ghost.getRadius();
    float minY = ghost.getRadius();
    float maxY = SCENE_HEIGHT - ghost.getRadius();

    // Limit the movement direction to keep the ghost within bounds
    if (ghostPosition.x <= minX) {
        direction.x = std::abs(direction.x); // Move away from the left edge
    } else if (ghostPosition.x >= maxX) {
        direction.x = -std::abs(direction.x); // Move away from the right edge
    }

    if (ghostPosition.y <= minY) {
        direction.y = std::abs(direction.y); // Move away from the top edge
    } else if (ghostPosition.y >= maxY) {
        direction.y = -std::abs(direction.y); // Move away from the bottom edge
    }

    return direction;
}

// Update function to move the ghost randomly
void Game::updateGhost(sf::Time delta) {
    // Check if the ghost needs a new direction
    if (needsNewDirection) {
        ghostDirection = generateRandomDirection(); // Get a random direction
        needsNewDirection = false;
    }    

    sf::Vector2f newPosition = ghost.getPosition() + ghostDirection * 800.0f * delta.asSeconds(); // Adjust speed   

    // Check if the new position falls within the scene boundaries
    if (newPosition.x - ghost.getRadius() >= 0 && newPosition.x + ghost.getRadius() <= SCENE_WIDTH &&
        newPosition.y - ghost.getRadius() >= 0 && newPosition.y + ghost.getRadius() <= SCENE_HEIGHT) {
        ghost.setPosition(newPosition); // Update ghost's position
    }
    else {
        // Bounce off the border by changing direction at a random angle
        ghostDirection = generateRandomDirection(); // Get a new random direction
        needsNewDirection = true; // Signal that a new direction is needed
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
    updateGhost(delta);
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
    window.draw(ghost);
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
