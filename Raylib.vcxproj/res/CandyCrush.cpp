#include "CandyCrush.h"
#include "GameConfig.h"
#include <cmath>
#include <algorithm>
#include <set>

using namespace GameConfig;

CandyCrush::CandyCrush() :
    isDragging(false),
    hasSwapped(false),
    selectedBall({ -1, -1 }),
    targetBall({ -1, -1 }),
    isAnimating(false),
    isFalling(false),
    animationTime(0)
{
    InitializeGrid();
}

void CandyCrush::Update() {
    Vector2 mousePos = GetMousePosition();
    Vector2 gridPos = GetGridPosition(mousePos);

    // Check for matches at the start
    if (!isAnimating && !isFalling && !hasSwapped) {
        if (CheckAllMatches()) {
            RemoveMatches();
            isFalling = true;
        }
    }

    if (isAnimating) {
        UpdateAnimations();
        return;  // Don't handle input during animations
    }

    // Handle spacebar swap
    if (IsKeyPressed(KEY_SPACE) && selectedBall.x >= 0 && selectedBall.y >= 0) {
        Vector2 currentPos = selectedBall;
        Vector2 nextPos = { currentPos.x + 1, currentPos.y };
        if (!IsValidGridPosition(nextPos)) {
            nextPos = { currentPos.x, currentPos.y + 1 };
        }
        if (IsValidGridPosition(nextPos)) {
            if (SwapBalls(currentPos, nextPos)) {
                targetBall = nextPos;
                hasSwapped = true;
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (IsValidGridPosition(gridPos)) {
            selectedBall = gridPos;
            targetBall = { -1, -1 };
            isDragging = true;
            hasSwapped = false;
        }
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (isDragging && !hasSwapped && IsValidGridPosition(gridPos) &&
            IsAdjacent(selectedBall, gridPos) &&
            selectedBall.x >= 0 && selectedBall.y >= 0) {
            // Try to swap on release
            if (SwapBalls(selectedBall, gridPos)) {
                targetBall = gridPos;
                hasSwapped = true;
            }
        }
        isDragging = false;
        if (!hasSwapped) {
            selectedBall = { -1, -1 };
            targetBall = { -1, -1 };
        }
    }

    // After a swap, if matches exist, remove them, then apply gravity and fill.
    // If no matches from the swap, revert hasSwapped.
    if (!isAnimating && hasSwapped) {
        if (CheckAllMatches()) {
            RemoveMatches();
            isFalling = true;
            hasSwapped = false;  // Reset swap state since we've handled the matches
        }
        else {
            // If no matches, swap back
            SwapBalls(targetBall, selectedBall);  // Swap back
            hasSwapped = false;
            selectedBall = { -1, -1 };
            targetBall = { -1, -1 };
        }
    }

    // Handle falling and subsequent matches
    if (isFalling && !isAnimating) {
        if (!ApplyGravity()) {  // If no more candies are falling
            FillEmptySpaces();
            if (CheckAllMatches()) {  // Check for new matches after filling
                RemoveMatches();
                // If new matches occur, they will cause another gravity fill in next frames
                isFalling = true;
            }
            else {
                isFalling = false;  // Done with falling and matching
            }
        }
    }
}

bool CandyCrush::IsAdjacent(Vector2 pos1, Vector2 pos2) const {
    if (pos1.x < 0 || pos1.y < 0 || pos2.x < 0 || pos2.y < 0) return false;
    return (std::fabs(pos1.x - pos2.x) + std::fabs(pos1.y - pos2.y)) == 1.0f;
}

Vector2 CandyCrush::GetGridPosition(Vector2 mousePos) const {
    return {
        (float)((int)mousePos.x / CELL_SIZE),
        (float)((int)mousePos.y / CELL_SIZE)
    };
}

bool CandyCrush::IsValidGridPosition(Vector2 pos) const {
    int x = (int)pos.x;
    int y = (int)pos.y;
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

Vector2 CandyCrush::GridToScreenPos(Vector2 gridPos) const {
    return {
        gridPos.x * CELL_SIZE + CELL_SIZE / 2.0f,
        gridPos.y * CELL_SIZE + CELL_SIZE / 2.0f
    };
}

void CandyCrush::InitializeGrid() {
    grid.resize(GRID_SIZE, std::vector<Candy>(GRID_SIZE));

    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            grid[x][y].type = GetRandomValue(0, 5);
            grid[x][y].color = GetCandyColor(grid[x][y].type);
            Vector2 screenPos = GridToScreenPos({ (float)x, (float)y });
            grid[x][y].position = screenPos;
            grid[x][y].targetPos = { (float)x, (float)y };
            grid[x][y].isMatched = false;
        }
    }
}

void CandyCrush::UpdateAnimations() {
    float dt = GetFrameTime();
    bool stillAnimating = false;

    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            Candy& candy = grid[x][y];
            Vector2 targetScreenPos = GridToScreenPos({ (float)x, (float)y });
            float dx = targetScreenPos.x - candy.position.x;
            float dy = targetScreenPos.y - candy.position.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance > 1.0f) {
                float moveX = dx * (dt * ANIMATION_SPEED);
                float moveY = dy * (dt * ANIMATION_SPEED);

                if (std::fabs(moveX) > std::fabs(dx)) moveX = dx;
                if (std::fabs(moveY) > std::fabs(dy)) moveY = dy;

                candy.position.x += moveX;
                candy.position.y += moveY;
                stillAnimating = true;
            }
            else {
                candy.position = targetScreenPos;
            }
        }
    }

    if (!stillAnimating) {
        isAnimating = false;
        animationTime = 0;
    }
}

bool CandyCrush::SwapBalls(Vector2 pos1, Vector2 pos2) {
    if (!IsValidGridPosition(pos1) || !IsValidGridPosition(pos2)) return false;
    if (!IsAdjacent(pos1, pos2)) return false;

    int x1 = (int)pos1.x;
    int y1 = (int)pos1.y;
    int x2 = (int)pos2.x;
    int y2 = (int)pos2.y;

    // Store current
    int type1 = grid[x1][y1].type;
    int type2 = grid[x2][y2].type;
    Color color1 = grid[x1][y1].color;
    Color color2 = grid[x2][y2].color;
    Vector2 pos1Screen = grid[x1][y1].position;
    Vector2 pos2Screen = grid[x2][y2].position;

    // Perform the swap
    grid[x1][y1].type = type2;
    grid[x1][y1].color = color2;
    grid[x2][y2].type = type1;
    grid[x2][y2].color = color1;

    // Check if this creates any matches
    if (!CheckAllMatches()) {
        // If no matches, swap back
        grid[x1][y1].type = type1;
        grid[x1][y1].color = color1;
        grid[x2][y2].type = type2;
        grid[x2][y2].color = color2;
        return false;
    }

    // Set up animation
    grid[x1][y1].position = pos1Screen;
    grid[x2][y2].position = pos2Screen;
    isAnimating = true;
    animationTime = 0;

    return true;
}

Color CandyCrush::GetCandyColor(int type) const {
    switch (type) {
    case 0: return Red;
    case 1: return Blue;
    case 2: return Green;
    case 3: return Yellow;
    case 4: return Orange;
    case 5: return Purple;
    default: return BLANK;
    }
}

void CandyCrush::ResetSwapState() {
    selectedBall = { -1, -1 };
    targetBall = { -1, -1 };
    isDragging = false;
    hasSwapped = false;
}

void CandyCrush::Draw() {
    // Draw background cells
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            DrawRectangle(
                x * CELL_SIZE,
                y * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE,
                RAYWHITE
            );
        }
    }

    // Draw candies
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            Color ballColor = grid[x][y].color;
            // Highlight selected and target
            if (isDragging) {
                if ((int)selectedBall.x == x && (int)selectedBall.y == y) {
                    ballColor.a = 150;
                }
                else if (hasSwapped && (int)targetBall.x == x && (int)targetBall.y == y) {
                    ballColor.a = 150;
                }
            }

            DrawCircle(
                (int)grid[x][y].position.x,
                (int)grid[x][y].position.y,
                CIRCLE_RADIUS,
                ballColor
            );
        }
    }
}

bool CandyCrush::CheckAllMatches() {
    bool foundMatches = false;

    // Check horizontal matches
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE - 2; x++) {  // -2 because we need at least 3 in a row
            if (grid[x][y].type == -1) continue;  // Skip empty cells
            
            // Check if we have 3 or more of the same type in a row
            if (grid[x][y].type == grid[x + 1][y].type && 
                grid[x][y].type == grid[x + 2][y].type) {
                
                // Mark these positions as matched
                grid[x][y].isMatched = true;
                grid[x + 1][y].isMatched = true;
                grid[x + 2][y].isMatched = true;
                foundMatches = true;
            }
        }
    }

    // Check vertical matches
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE - 2; y++) {  // -2 because we need at least 3 in a row
            if (grid[x][y].type == -1) continue;  // Skip empty cells
            
            // Check if we have 3 or more of the same type in a column
            if (grid[x][y].type == grid[x][y + 1].type && 
                grid[x][y].type == grid[x][y + 2].type) {
                
                // Mark these positions as matched
                grid[x][y].isMatched = true;
                grid[x][y + 1].isMatched = true;
                grid[x][y + 2].isMatched = true;
                foundMatches = true;
            }
        }
    }

    return foundMatches;
}

void CandyCrush::RemoveMatches() {
    bool hasMatches = false;

    // First pass: spawn food at matched locations
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            if (grid[x][y].isMatched) {
                hasMatches = true;
                // Spawn food at this location before removing the candy
                if (SpawnFood) {
                    SpawnFood({ (float)x, (float)y });
                }
                // Immediately remove the matched candy
                grid[x][y].type = -1;
                grid[x][y].color = BLANK;
                grid[x][y].isMatched = false;
            }
        }
    }

    // Set falling flag to true since we removed candies
    if (hasMatches) {
        isFalling = true;
    }
}

bool CandyCrush::ApplyGravity() {
    bool stillFalling = false;

    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = GRID_SIZE - 2; y >= 0; y--) {
            if (grid[x][y].type != -1) {
                int fallDistance = 0;
                for (int k = y + 1; k < GRID_SIZE && grid[x][k].type == -1; k++) {
                    fallDistance++;
                }
                if (fallDistance > 0) {
                    grid[x][y + fallDistance].type = grid[x][y].type;
                    grid[x][y + fallDistance].color = grid[x][y].color;
                    grid[x][y + fallDistance].position = grid[x][y].position;
                    grid[x][y + fallDistance].targetPos = { (float)x, (float)(y + fallDistance) };

                    grid[x][y].type = -1;
                    grid[x][y].color = BLANK;
                    stillFalling = true;
                    isAnimating = true;
                    animationTime = 0;
                }
            }
        }
    }

    return stillFalling;
}

void CandyCrush::FillEmptySpaces() {
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            if (grid[x][y].type == -1) {
                grid[x][y].type = GetRandomValue(0, 5);
                grid[x][y].color = GetCandyColor(grid[x][y].type);

                // Appear from above
                grid[x][y].position = GridToScreenPos({ (float)x, (float)-1 });
                grid[x][y].targetPos = { (float)x, (float)y };

                isAnimating = true;
                animationTime = 0;
            }
        }
    }
}
