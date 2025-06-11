#ifndef GAME_GRID_H_
#define GAME_GRID_H_

#include "raylib.h"

#include "structs.h"
#include "constants.h"
#include "containers/vec.h"


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
    // vec_mino grid; // origin is top-left
    vec_mm grid; // origin is top-left
    vec_piece_type upcoming_pieces;
    vec_piece_type bag;
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


void input_state_init(InputState* input_state);
void game_grid_init(GameGrid* gg);
bool game_grid_update(GameGrid* gg);
PieceType game_grid_pick_new_piece(GameGrid* gg);
bool game_grid_move_dynamic_minos(int right, int down);


#endif // GAME_GRID_H_
