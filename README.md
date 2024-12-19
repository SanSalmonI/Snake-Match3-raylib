# Candy Crush Snake

**Candy Crush Snake** is a hybrid game that combines elements of the classic "Snake" game with a "Match-3" style candy board (inspired by Candy Crush). Make matches in the candy grid to spawn food for the snake, and control the snake to eat the food and grow!

![Gameplay](Animation.gif)


## Features

- **Match-3 Mechanics**: Swap adjacent candies in the 8x8 grid to create matches of 3 or more. When matches occur, candies are removed, and food spawns in those positions for the snake.
- **Integrated Snake Game**: A simple Snake game is overlaid on the same grid. The snake moves continuously and grows when it eats food spawned by the candy board.
- **Gravity & Refilling**: After candies are matched and removed, gravity pulls remaining candies down, and new candies fill the top. This can create cascades of matches.
- **Simple Controls**:
  - **Mouse Click + Drag** to select and swap candies.
  - **Arrow Keys** to change the snake’s direction.
  - **Spacebar** to attempt a candy swap at the snake’s head or after selecting a candy.
  
## Controls

- **Mouse**: Click and drag on the candy grid to select and swap adjacent candies.
- **Arrow Keys**: Control the snake’s direction (Up, Down, Left, Right).
- **Spacebar**: 
  - If you’ve selected a candy, pressing Space tries to swap it with an adjacent candy in the next frame.
  - If no candy is selected, Space attempts a swap at the snake’s head position (for testing purposes).
  - Pressing Space after game over restarts the snake game.

## How It Works

1. **Candy Board**: The board is an 8x8 grid of randomly generated candy types.
2. **Swapping Candies**: When you swap two adjacent candies:
   - If the swap results in a match (3 or more same-type candies in a row or column), those candies are removed.
   - Food is spawned at those removed candy positions (signaled to the Snake game).
   - Gravity and refill logic ensure the board remains full.
3. **Snake Game**:
   - The snake moves around the grid, attempting to eat the spawned food.
   - Each piece of food eaten increases the snake’s score and length.
   - If the snake collides with a wall or itself, the snake game ends.

## Building & Running

1. **Dependencies**:
   - [Raylib](https://www.raylib.com/) for graphics and input handling.
   - A C++ compiler (GCC, Clang, or MSVC).
   
2. **Compile**:
   ```bash
   g++ main.cpp CandyCrush.cpp PathFinding.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o CandyCrushSnake
