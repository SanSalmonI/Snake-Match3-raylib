#include "raylib.h"
#include "PathFinding.h"
#include "CandyCrush.h"
#include "GameConfig.h"
#include <iostream>
#include <functional>

int main()
{
    InitWindow(GameConfig::SCREEN_SIZE, GameConfig::SCREEN_SIZE, "Snake and Candy Crush");
    SetTargetFPS(60);

    PathFinding snake;
    CandyCrush candyCrush;

    // Set up food spawning callback
    candyCrush.SpawnFood = [&snake](Vector2 pos) {
        snake.OnFoodSpawn(pos);
    };

    while (!WindowShouldClose())    
    {
        // Update
        snake.Update(candyCrush);
        candyCrush.Update();

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw games in correct order
        candyCrush.Draw();  // Draw candy crush first (background)
        snake.Draw();       // Draw snake game on top

        EndDrawing();
    }

    CloseWindow();
    return 0;
}