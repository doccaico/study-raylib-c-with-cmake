#ifndef CONSTANTS_H_
#define CONSTANTS_H_


#include "raylib.h"

#ifdef _DEBUG
#define WINDOW_TITLE "tetris (debug)"
#else
#define WINDOW_TITLE "tetris"
#endif

#define FPS (60)

// Sizings

#define CELL_SIZE 25
#define CELLS_X 10
#define CELLS_Y 20
#define GRIDE_WIDTH CELL_SIZE * CELLS_X
#define GRIDE_HEIGHT CELL_SIZE * CELLS_Y
#define SIDEBAR_WIDTH (CELL_SIZE * 4) + 20
#define SCREEN_WIDTH GRIDE_WIDTH + SIDEBAR_WIDTH
#define SCREEN_HEIGHT GRIDE_HEIGHT

// Colors

#define GRID_COLOR (Color){50, 50, 50, 255}

// Pieces

typedef enum {
    I,
    J,
    L,
    O,
    T,
    S,
    Z
} PieceType;

#define I_PIECE_COLOR (Color){0, 255, 255, 255}
#define J_PIECE_COLOR (Color){0, 0, 255, 255}
#define L_PIECE_COLOR (Color){255, 165, 0, 255}
#define O_PIECE_COLOR (Color){255, 255, 0, 255}
#define T_PIECE_COLOR (Color){128, 0, 128, 255}
#define S_PIECE_COLOR (Color){0, 255, 0, 255}
#define Z_PIECE_COLOR (Color){255, 0, 0, 255}

// Game settings

#define DROP_SPEED 30 // Every X frames, drop minos by 1 cell
#define SPED_UP_DROP_SPEED 2
#define LEVEL_UP_SPEED_INCREASE 0.85f
#define PIECE_STATIFICATION_DELAY 30

// Backend game settings

#define PIECE_LOOKAHEAD 3
#define MAX_PIECE_QUEUE 20
#define DAS_DELAY 10 // Delay before auto-shift starts (in frames)
#define ARR_RATE 2 // Frames between auto-shift after DAS start


#endif // CONSTANTS_H_
