// CoordinateTest.cpp
#include <gtest/gtest.h>
#include "move.h"
#include "game.h"

TEST(CoordinateTest, Add) {
    Coordinate a = {1.0, 2.0};
    Coordinate b = {3.0, 4.0};
    Coordinate result = add(a, b);
    EXPECT_FLOAT_EQ(result.x, 4.0);
    EXPECT_FLOAT_EQ(result.y, 6.0);
}

TEST(CoordinateTest, Mul) {
    Coordinate a = {1.0, 2.0};
    float s = 2.0;
    Coordinate result = mul(s, a);
    EXPECT_FLOAT_EQ(result.x, 2.0);
    EXPECT_FLOAT_EQ(result.y, 4.0);
}

TEST(CoordinateTest, Move) {
    Coordinate position = {1.0, 2.0};
    Coordinate velocity = {2.0, 3.0};
    float delta = 0.5;
    Coordinate result = move(position, velocity, delta);
    EXPECT_FLOAT_EQ(result.x, 2.0);
    EXPECT_FLOAT_EQ(result.y, 3.5);
}

TEST(GameScoreTest, IncrementScoreOnCollision) {
    Game game;
    int initialScore = game.getScore();

    // Simulate collision between player and ghost
    sf::Vector2f ghostPosition = game.getGhostPosition(0);
    game.setGhostPosition(0, game.getPlayerPosition()); // Set ghost at the player's position

    // Update the game, which should handle the collision
    sf::Time delta = sf::milliseconds(100); // Arbitrary delta time
    game.update(delta, game.getPlayer());

    // Check if the score incremented after collision
    int updatedScore = game.getScore();
    EXPECT_EQ(updatedScore, initialScore + 1); // Score should increment by 1 after collision
}

TEST(GameCollisionTest, GhostDisappearsOnCollision) {
    Game game;
    sf::Vector2f initialGhostPosition = game.getGhostPosition(0);

    // Simulate collision between player and ghost
    game.setGhostPosition(0, game.getPlayerPosition()); // Set ghost at the player's position

    // Update the game, which should handle the collision
    sf::Time delta = sf::milliseconds(100); // Arbitrary delta time
    game.update(delta, game.getPlayer());

    // Check if the ghost position changes after collision (ghost disappears)
    sf::Vector2f updatedGhostPosition = game.getGhostPosition(0);
    EXPECT_NE(updatedGhostPosition, initialGhostPosition); // Ghost position should change after collision
}