# 2D-game
This is the first draft of a simple 2D game using the SFML library.

## Dependencies
This project depends on the [SFML library](https://www.sfml-dev.org/index.php). SFML can be installed via apt on Linux, brew on MacOS  

## Compile and run
Once dependencies are set, you can compile the project with `cmake` in CLion and run it directly. Via command line you can ask `cmake` to generate the `Makefile` and then run `make all`
## Implementation Guidelines

### Game (game.cpp)
- **Description:** 
  - Initializes a game window and controls the game mechanics.
- **Functions:**
  - `initWindow()`: Initializes the game window.
  - `initBackground()`: Loads and sets up the game background.
  - `initPlayer()`: Initializes the player character.
  - `initNPC()`: Initializes non-playable characters (ghosts).
  - `processInput()`: Handles window events.
  - `update()`: Updates game logic and entities.
  - `render()`: Renders game elements.
  - `run()`: Main game loop.

### Move (move.cpp)
- **Description:**
  - Provides functions for player movement.
- **Functions:**
  - `add()`: Adds two coordinates.
  - `mul()`: Multiplies a coordinate by a scalar.
  - `move()`: Calculates player movement based on arrow key inputs.

---

## Assumptions on Environments

- **Environment:** 
  - Developed and tested on environments compatible with SFML.
- **Dependencies:**
  - SFML library installed and linked appropriately.

---

## Users Guide

### Installation
- Provide information on how to install SFML and set up the development environment.
- Instructions on how to compile and run the game.

### Gameplay
- **Objective:** 
  - Control the donut character to eat all the evil donuts while avoiding collision with them.

- **Controls:** 
  - Use the arrow keys (Up, Down, Left, Right) to navigate the donut character on the game scene.
  - Navigate the donut to consume the evil donuts appearing on the screen.

- **Rules:** 
  - The game is timed to eat as many evil donuts as possible.
  - Avoid collision with evil donuts, as they will cause the game to end.
  - Each time the donut consumes an evil donut, the score increases.

- **Game Over:** 
  - The game ends either when the time runs out or if the donut collides with an evil donut.
  - Display the final score when the game ends.

---

## Developers Guide

