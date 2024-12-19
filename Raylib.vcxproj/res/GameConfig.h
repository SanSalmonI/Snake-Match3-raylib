#pragma once

namespace GameConfig {
    // Screen and grid configuration
    static constexpr int GRID_SIZE = 20;
    static constexpr float CELL_SIZE = 30.0f;
    static constexpr int SCREEN_SIZE = GRID_SIZE * CELL_SIZE;

    // Game timing
    static constexpr float MOVE_DELAY = 0.15f;

    // Candy configuration
    static constexpr int CANDY_GRID_SIZE = 8;
    static constexpr float CANDY_CELL_SIZE = 60.0f;
    static constexpr int NUM_CANDY_COLORS = 4;
}
