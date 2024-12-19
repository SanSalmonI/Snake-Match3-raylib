#pragma once
#include "raylib.h"
#include "GameConfig.h"
#include <vector>
#include <set>
#include <functional>

struct Candy {
    int type;
    Color color;
    Vector2 position;      // Current animated position
    Vector2 targetPos;     // Target grid position
    bool isMatched;        // Whether this candy is part of a match
};

class CandyCrush {
public:
    CandyCrush();
    void Update();
    void Draw();
    bool SwapBalls(Vector2 pos1, Vector2 pos2);  // Returns true if swap was successful
    void ResetSwapState();  // Reset the swap visual state
    std::function<void(Vector2)> SpawnFood;  

    Color Red = { 255, 80, 100, 200 };
    Color Green = { 80, 255, 100, 255 };
    Color Blue = { 100, 190, 255, 255 };
    Color Yellow = { 240, 240, 100, 255 };
    Color Orange = { 255, 165, 0, 255 };
    Color Purple = { 128, 0, 128, 200 };

private:
    static constexpr float CIRCLE_RADIUS = GameConfig::CELL_SIZE / 2.5f;
    static constexpr float ANIMATION_SPEED = 10.0f;
    static constexpr float FALL_SPEED = 15.0f;

    std::vector<std::vector<Candy>> grid;
    Vector2 selectedBall;
    Vector2 targetBall;
    bool isDragging;
    bool hasSwapped;
    bool isAnimating;
    bool isFalling;
    float animationTime;
    std::function<void(Vector2)> onMatchComplete;

    void InitializeGrid();
    Color GetCandyColor(int type) const;
    Vector2 GetGridPosition(Vector2 mousePos) const;
    bool IsValidGridPosition(Vector2 pos) const;
    bool IsAdjacent(Vector2 pos1, Vector2 pos2) const;
    void UpdateAnimations();
    Vector2 GridToScreenPos(Vector2 gridPos) const;

    // Match-3 mechanics
    bool CheckMatches();
    void RemoveMatches();
    bool ApplyGravity();
    void FillEmptySpaces();
    bool FindMatches(int x, int y, std::set<std::pair<int, int>>& matches);

    // Additional matching functions
    bool CheckMatchesAtSwap(Vector2 pos1, Vector2 pos2);
    void FindMatchesInLine(int startX, int startY, bool horizontal, std::set<std::pair<int, int>>& matches);
    void CheckLineForMatches(int x, int y, bool horizontal, std::set<std::pair<int, int>>& matches);

    // Newly added method for global board checks
    bool CheckAllMatches(); // <--- Add this method declaration
};
