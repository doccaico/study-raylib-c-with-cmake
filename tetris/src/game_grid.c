#include "game_grid.h"
#include "constants.h"


void input_state_init(InputState* input_state)
{
    input_state->right_pressed_last_frameg = false; // move right
    input_state->left_pressed_last_frameg = false; // move left
    input_state->down_pressed_last_frameg = false; // drop faster
    input_state->up_pressed_last_frameg = false; // rotate
    input_state->space_pressed_last_frameg = false; // hard drop
    input_state->a_pressed_last_frameg = false; // rotate left
    input_state->d_pressed_last_frameg = false; // rotate right
    input_state->shift_pressed_last_frameg = false; // rotate right
}

void game_grid_init(GameGrid* game_grid)
{
    game_grid->grid = (vec_mino){0};
    game_grid->upcoming_pieces = (vec_piece_type){0};
    game_grid->bag = (vec_piece_type){0};
    game_grid->held_piece = NULL;
    game_grid->next_update = DROP_SPEED;
    input_state_init(&game_grid->input_state);
    game_grid->last_movement_update = 0;
    game_grid->right_key_hold_time = 0;
    game_grid->left_key_hold_time = 0;
    game_grid->level = 0;
    game_grid->score = 0;
    game_grid->lines_cleared = 0;
    game_grid->speed = 1.0f;
}
