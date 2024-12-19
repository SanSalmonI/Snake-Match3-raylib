#include "PathFinding.h"
using namespace GameConfig;

PathFinding::PathFinding() {
    ResetGame();
}

void PathFinding::Update(CandyCrush& candyCrush) {
    // Set up the callback for CandyCrush to spawn food if not already set
    if (!candyCrush.SpawnFood) {
        candyCrush.SpawnFood = std::bind(&PathFinding::OnFoodSpawn, this, std::placeholders::_1);
    }

    if (!gameState.isGameOver) {
        HandleInput(candyCrush);
        UpdateGame();

        // Spawn food if we should
        if (gameState.shouldSpawnFood) {
            SpawnFood();
            gameState.shouldSpawnFood = false;
        }

        // Reset candy swap state when space is released
        if (IsKeyReleased(KEY_SPACE)) {
            candyCrush.ResetSwapState();
        }
    }
    else if (IsKeyPressed(KEY_SPACE)) {
        ResetGame();
    }
}

void PathFinding::OnFoodSpawn(Vector2 pos) {
    // Attempt to place food exactly at pos
    if (!CheckCollision(pos)) {
        // If no collision with snake, place food here
        gameState.foodPosition = pos;
        gameState.shouldSpawnFood = false; // We've placed the food
    }
    else {
        // If collision, fallback to random spawn next update
        gameState.shouldSpawnFood = true;
    }
}

void PathFinding::HandleInput(CandyCrush& candyCrush) {
    // Handle direction changes
    if (IsKeyPressed(KEY_UP) && !IsOppositeDirection(gameState.currentDirection, { 0, -1 })) {
        gameState.nextDirection = { 0, -1 };
    }
    if (IsKeyPressed(KEY_DOWN) && !IsOppositeDirection(gameState.currentDirection, { 0, 1 })) {
        gameState.nextDirection = { 0, 1 };
    }
    if (IsKeyPressed(KEY_LEFT) && !IsOppositeDirection(gameState.currentDirection, { -1, 0 })) {
        gameState.nextDirection = { -1, 0 };
    }
    if (IsKeyPressed(KEY_RIGHT) && !IsOppositeDirection(gameState.currentDirection, { 1, 0 })) {
        gameState.nextDirection = { 1, 0 };
    }

    // Handle ball swapping with space
    if (IsKeyPressed(KEY_SPACE)) {
        Vector2 headPos = gameState.snake[0];
        Vector2 nextPos = {
            headPos.x + gameState.currentDirection.x,
            headPos.y + gameState.currentDirection.y
        };
        candyCrush.SwapBalls(headPos, nextPos);
    }
}

void PathFinding::UpdateGame() {
    gameState.moveTimer += GetFrameTime();

    if (gameState.moveTimer >= MOVE_DELAY) {
        gameState.moveTimer = 0;
        gameState.currentDirection = gameState.nextDirection;

        // Calculate new head position
        Vector2 newHead = gameState.snake[0];
        newHead.x += gameState.currentDirection.x;
        newHead.y += gameState.currentDirection.y;

        // Check for collisions with walls
        if (newHead.x < 0 || newHead.x >= GRID_SIZE ||
            newHead.y < 0 || newHead.y >= GRID_SIZE) {
            gameState.isGameOver = true;
            return;
        }

        // Check for collisions with self
        if (CheckCollision(newHead)) {
            gameState.isGameOver = true;
            return;
        }

        // Move snake
        gameState.snake.insert(gameState.snake.begin(), newHead);

        // Check for food collision
        if (newHead.x == gameState.foodPosition.x &&
            newHead.y == gameState.foodPosition.y) {
            gameState.score++;
            // Food is eaten but we don't set shouldSpawnFood - it will only spawn on matches
            gameState.foodPosition = { -1, -1 };  // Move food off-grid when eaten
        }
        else {
            gameState.snake.pop_back();
        }
    }
}

void PathFinding::Draw() {
    // Draw snake body first (all segments except head)
    for (size_t i = 1; i < gameState.snake.size(); i++) {
        Vector2 pos = gameState.snake[i];
        DrawRectangle(
            (int)(pos.x * CELL_SIZE + 1),
            (int)(pos.y * CELL_SIZE + 1),
            CELL_SIZE - 2,
            CELL_SIZE - 2,
            DARKGREEN
        );
    }

    // Draw snake head as a triangle
    if (!gameState.snake.empty()) {
        Vector2 headPos = gameState.snake[0];
        float rotation = 0.0f;

        // Calculate rotation based on direction
        if (gameState.currentDirection.x == 1) rotation = 0.0f;        // Right
        else if (gameState.currentDirection.x == -1) rotation = 180.0f; // Left
        else if (gameState.currentDirection.y == -1) rotation = 270.0f; // Up
        else if (gameState.currentDirection.y == 1) rotation = 90.0f;   // Down

        // Calculate triangle points
        float centerX = headPos.x * CELL_SIZE + CELL_SIZE / 2;
        float centerY = headPos.y * CELL_SIZE + CELL_SIZE / 2;
        float size = CELL_SIZE * 0.4f;  // Triangle size relative to cell

        // Create base triangle points (pointing right)
        Vector2 p1 = { centerX + size, centerY };           // Tip
        Vector2 p2 = { centerX - size, centerY - size };    // Top back
        Vector2 p3 = { centerX - size, centerY + size };    // Bottom back

        // Rotate points around center
        float rad = rotation * PI / 180.0f;
        Vector2 rotatedP1 = {
            centerX + (p1.x - centerX) * cos(rad) - (p1.y - centerY) * sin(rad),
            centerY + (p1.x - centerX) * sin(rad) + (p1.y - centerY) * cos(rad)
        };
        Vector2 rotatedP2 = {
            centerX + (p2.x - centerX) * cos(rad) - (p2.y - centerY) * sin(rad),
            centerY + (p2.x - centerX) * sin(rad) + (p2.y - centerY) * cos(rad)
        };
        Vector2 rotatedP3 = {
            centerX + (p3.x - centerX) * cos(rad) - (p3.y - centerY) * sin(rad),
            centerY + (p3.x - centerX) * sin(rad) + (p3.y - centerY) * cos(rad)
        };

        DrawTriangle(rotatedP1, rotatedP2, rotatedP3, GREEN);
    }

    // Draw food if valid position
    if (gameState.foodPosition.x >= 0 && gameState.foodPosition.y >= 0) {
        DrawRectangle(
            (int)(gameState.foodPosition.x * CELL_SIZE + 1),
            (int)(gameState.foodPosition.y * CELL_SIZE + 1),
            CELL_SIZE - 2,
            CELL_SIZE - 2,
            RED
        );
    }

    // Draw grid with transparency
    Color gridColor = { 200, 200, 200, 100 };
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, gridColor);
        }
    }

    // Draw UI
    DrawText(TextFormat("Score: %d", gameState.score), 10, 10, 20, DARKGRAY);

    if (gameState.isGameOver) {
        const char* text = "Game Over! Press SPACE to restart";
        int width = MeasureText(text, 20);
        DrawText(text, (GameConfig::SCREEN_SIZE - width) / 2, GameConfig::SCREEN_SIZE / 2, 20, RED);
    }
}

void PathFinding::ResetGame() {
    gameState = GameState();
    gameState.snake = { {5, 5} };  // Start with single segment
    gameState.currentDirection = { 1, 0 };  // Start moving right
    gameState.nextDirection = gameState.currentDirection;
    gameState.isGameOver = false;
    gameState.score = 0;
    gameState.moveTimer = 0;
    gameState.shouldSpawnFood = true;  // Spawn initial food
    SpawnFood();  // Spawn the initial food
}

void PathFinding::SpawnFood(Vector2 pos) {
    if (CheckCollision(pos)) return;  // Don't spawn food on snake
    gameState.foodPosition = pos;
}

void PathFinding::SpawnFood() {
    if (!gameState.shouldSpawnFood) return;

    Vector2 newFood;
    do {
        newFood.x = GetRandomValue(0, GRID_SIZE - 1);
        newFood.y = GetRandomValue(0, GRID_SIZE - 1);
    } while (CheckCollision(newFood));

    gameState.foodPosition = newFood;
    gameState.shouldSpawnFood = false;
}

bool PathFinding::CheckCollision(Vector2 pos) {
    for (const Vector2& segment : gameState.snake) {
        if (segment.x == pos.x && segment.y == pos.y) {
            return true;
        }
    }
    return false;
}

bool PathFinding::IsOppositeDirection(Vector2 dir1, Vector2 dir2) {
    return (dir1.x == -dir2.x && dir1.y == -dir2.y);
}
