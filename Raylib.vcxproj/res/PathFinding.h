#pragma once
#include "raylib.h"
#include "GameConfig.h"
#include "CandyCrush.h"
#include <vector>
#include <functional>

class PathFinding {
public:
    PathFinding();
    void Update(CandyCrush& candyCrush);  // Modified to take CandyCrush reference
    void Draw();    // Separated draw function
    bool IsGameOver() const { return gameState.isGameOver; }
    void OnFoodSpawn(Vector2 pos);  // Declaration only, implemented in cpp

private:
    // Constants for game configuration
    static constexpr float MOVE_DELAY = 0.15f;    // Time between movements

    struct GameState {
        std::vector<Vector2> snake;   // Snake body segments
        Vector2 foodPosition;         // Current food position
        Vector2 currentDirection;     // Direction snake is moving
        Vector2 nextDirection;        // Buffered next direction
        bool isGameOver;              // Game over state
        bool shouldSpawnFood;         // Flag to spawn food
        int score;                    // Current score
        float moveTimer;              // Time since last movement

        GameState() {
            snake.clear();
            foodPosition = { -1, -1 };
            currentDirection = { 0, 0 };
            nextDirection = { 0, 0 };
            isGameOver = false;
            shouldSpawnFood = false;
            score = 0;
            moveTimer = 0.0f;
        }
    };

    GameState gameState;

    // Core game functions
    void HandleInput(CandyCrush& candyCrush);  // Modified to handle ball swapping
    void UpdateGame();           // Update game state
    void ResetGame();            // Reset to initial state

    // Helper functions
    void SpawnFood();            // Create new food at random position
    void SpawnFood(Vector2 pos); // Overload to spawn food at specific position
    bool CheckCollision(Vector2 pos);  // Check if position collides with snake
    bool IsOppositeDirection(Vector2 dir1, Vector2 dir2);  // Check if directions are opposite
};
