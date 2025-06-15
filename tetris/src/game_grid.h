#ifndef GAME_GRID_H_
#define GAME_GRID_H_

#include <stdbool.h>

#include "structs.h"
#include "yac_types.h"


typedef struct {
    bool right_pressed_last_frame; // move right
    bool left_pressed_last_frame; // move left
    bool down_pressed_last_frame; // drop faster
    bool up_pressed_last_frame; // rotate
    bool space_pressed_last_frame; // hard drop
    bool a_pressed_last_frame; // rotate left
    bool d_pressed_last_frame; // rotate right
    bool shift_pressed_last_frame; // rotate right
} InputState;

typedef struct {
    DaDaMino grid; // origin is top-left
    DaPieceType upcoming_pieces;
    DaPieceType bag;
    PieceType* held_piece;
    PieceType current_piece;
    unsigned int next_update;
    InputState input_state;
    GridPos pivot;
    unsigned int last_movement_update;
    int right_key_hold_time;
    int left_key_hold_time;
    unsigned int level;
    unsigned int score;
    unsigned int lines_cleared;
    float speed;
} GameGrid;


void GameGridInit(GameGrid* gg);
void GameGridDeinit(GameGrid* gg);
bool GameGridUpdate(GameGrid* gg);
void GameGridDraw(GameGrid* gg);


#endif // GAME_GRID_H_
