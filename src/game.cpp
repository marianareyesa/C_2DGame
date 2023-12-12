
#include <iostream>
#include "game.h"
#include "move.h"
#include <cmath>

const float Game::SCENE_WIDTH = 1000.0f;
const float Game::SCENE_HEIGHT = 800.0f;
const float Game::PLAYER_START_X = 400.0f;
const float Game::PLAYER_START_Y = 300.0f;
const float Game::RADIUS = 40.0f;
const float Game::SPEED = 250.0f;

Game::Game() {
    initWindow();
    initBackground();
    initPlayer();
    initNPC();
    needsNewDirection = true; // Initialize needsNewDirection to indicate a new direction is needed initially
    ghostDirection = generateRandomDirection(); // Initialize direction with a random direction
    gameTime = sf::seconds(30); // Set game duration
    score = 0; // Initialize player score
    gameOverFlag = false; // Initialize game over flag

    if (!font.loadFromFile("resources/Roboto-Black.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    // Setup timer text properties
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(10.0f, 10.0f); // Adjust the position of the timer text
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
    for (int i = 0; i < MAX_GHOSTS; ++i) {
        ghosts[i].setRadius(RADIUS);
        ghosts[i].setOrigin(RADIUS, RADIUS);

        float randomX = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (SCENE_WIDTH - 2 * RADIUS) + RADIUS;
        float randomY = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (SCENE_HEIGHT - 2 * RADIUS) + RADIUS;
        ghosts[i].setPosition(randomX, randomY);

        if (!ghostTexture.loadFromFile("resources/evil_donut.png")) {
            return 1;
        }
        ghosts[i].setTexture(&ghostTexture);

        ghostDirections[i] = generateRandomDirection(); // Initialize directions for each ghost
        needsNewDirections[i] = true; // Indicate that a new direction is initially needed
    }
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

bool Game::checkCollision(const sf::Shape& shape1, const sf::Shape& shape2) {
    sf::FloatRect shape1Bounds = shape1.getGlobalBounds();
    sf::FloatRect shape2Bounds = shape2.getGlobalBounds();

    return shape1Bounds.intersects(shape2Bounds);
}

void Game::gameOver() {
    window.clear(sf::Color::Black); // Clear the window
    sf::Text endText;
    endText.setFont(font); // Assuming you have already loaded the font
    endText.setCharacterSize(50);
    endText.setFillColor(sf::Color::White);
    endText.setString("Game Over!\nYour Score: " + std::to_string(score));
    endText.setPosition((SCENE_WIDTH - endText.getLocalBounds().width) / 2, (SCENE_HEIGHT - endText.getLocalBounds().height) / 2);
    window.draw(endText);
    window.display();
    sf::sleep(sf::seconds(5)); // Show the end screen for 3 seconds before closing
    window.close();
}


/**
 * Function to update the position of the player
 */
void Game::update(sf::Time delta, sf::Shape &player) {
    if(!gameOverFlag){
    gameTime -= delta; // Decrement game time by delta

        if (gameTime <= sf::Time::Zero) {
            // Game over condition - handle end of the game
            gameOverFlag = true;
        }
    // Generate a random number between 0 and 1
    float randomProbability = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

    // Define a probability threshold for ghost spawn
    float spawnProbability = 0.02f; // Adjust this value to control spawn frequency

    if (randomProbability < spawnProbability) {
        // Find an inactive ghost and spawn it
        for (int i = 0; i < MAX_GHOSTS; ++i) {
            if (ghosts[i].getPosition().x < 0 || ghosts[i].getPosition().y < 0) {
                // Generate a random direction for the new ghost
                ghostDirections[i] = generateRandomDirection();
                needsNewDirections[i] = false;

                // Set the new ghost's position and update other properties as needed
                float randomX = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (SCENE_WIDTH - 2 * RADIUS) + RADIUS;
                float randomY = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (SCENE_HEIGHT - 2 * RADIUS) + RADIUS;
                ghosts[i].setPosition(randomX, randomY);
                break; // Exit the loop after spawning one ghost
            }
        }
    }

    // Update player and each ghost
    for (int i = 0; i < MAX_GHOSTS; ++i) {
        if (needsNewDirections[i]) {
            ghostDirections[i] = generateRandomDirection();
            needsNewDirections[i] = false;
        }

        sf::Vector2f newPosition = ghosts[i].getPosition() + ghostDirections[i] * 800.0f * delta.asSeconds();

        // Check for window boundaries and adjust ghost movement
        float ghostRadius = ghosts[i].getRadius();
        if (newPosition.x - ghostRadius >= 0 && newPosition.x + ghostRadius <= SCENE_WIDTH &&
            newPosition.y - ghostRadius >= 0 && newPosition.y + ghostRadius <= SCENE_HEIGHT) {
            // Ghost within boundaries, update position
            ghosts[i].setPosition(newPosition);
        } else {
            // Ghost reached boundary, change direction
            ghostDirections[i].x = -ghostDirections[i].x;
            ghostDirections[i].y = -ghostDirections[i].y;
        }

        // Check for collision with player
        if (checkCollision(player, ghosts[i])) {
            // Player collided with the ghost, take appropriate action
            score++;
            ghosts[i].setPosition(-1000, -1000); // Move ghost off-screen (or hide it in any other way)
        }
    }

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
    if(gameOverFlag){
        gameOver();
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

void Game::renderTimer() {
    // Convert the remaining game time to seconds and display it as text
    int seconds = static_cast<int>(gameTime.asSeconds());
    std::string timerString = "Time: " + std::to_string(seconds) + "s";
    timerText.setString(timerString);

    // Render the timer text on the window
    window.draw(timerText);
}

/**
 * Render elements in the window
 */
void Game::render() {
    window.clear(sf::Color::White);
    window.draw(background);
    window.draw(player);
    for (int i = 0; i < MAX_GHOSTS; ++i) {
        window.draw(ghosts[i]);
    }
    renderTimer();
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
