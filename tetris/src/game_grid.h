#ifndef GAME_GRID_H_
#define GAME_GRID_H_

#include "raylib.h"

#include "mino.h"
#include "structs.h"

#define T box_mino, Mino
#include "stc/box.h"

#define T vec_mino, box_mino
#include "stc/vec.h"

// #define T vec_mm, vec_mino, (c_valpro)
#define T vec_mm, vec_mino, (c_keyclass)
#include "stc/vec.h"

#define T vec_piece_type, PieceType
#include "stc/vec.h"

typedef struct {
    bool right_pressed_last_frameg; // move right
    bool left_pressed_last_frameg; // move left
    bool down_pressed_last_frameg; // drop faster
    bool up_pressed_last_frameg; // rotate
    bool space_pressed_last_frameg; // hard drop
    bool a_pressed_last_frameg; // rotate left
    bool d_pressed_last_frameg; // rotate right
    bool shift_pressed_last_frameg; // rotate right
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
void game_grid_init(GameGrid* game_grid);
bool game_grid_update(GameGrid* game_grid);
PieceType game_grid_pick_new_piece(GameGrid* game_grid);


#endif // GAME_GRID_H_
