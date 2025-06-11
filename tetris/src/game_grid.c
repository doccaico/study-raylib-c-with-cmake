#include "game_grid.h"
#include "shape_manager.h"
#include "constants.h"
// #include "mino.h"

void input_state_init(InputState* input_state)
{
    input_state->right_pressed_last_frame = false; // move right
    input_state->left_pressed_last_frame = false; // move left
    input_state->down_pressed_last_frame = false; // drop faster
    input_state->up_pressed_last_frame = false; // rotate
    input_state->space_pressed_last_frame = false; // hard drop
    input_state->a_pressed_last_frame = false; // rotate left
    input_state->d_pressed_last_frame = false; // rotate right
    input_state->shift_pressed_last_frame = false; // rotate right
}

void game_grid_init(GameGrid* gg)
{
    gg->grid = (vec_mm){0};

    for (int i = 0; i < MAX_PIECE_QUEUE; ++i) {
        vec_piece_type_push(&gg->upcoming_pieces, game_grid_pick_new_piece(gg));
    }


    gg->upcoming_pieces = (vec_piece_type){0};
    gg->bag = (vec_piece_type){0};
    gg->held_piece = NULL;
    gg->next_update = DROP_SPEED;
    input_state_init(&gg->input_state);
    gg->last_movement_update = 0;
    gg->right_key_hold_time = 0;
    gg->left_key_hold_time = 0;
    gg->level = 0;
    gg->score = 0;
    gg->lines_cleared = 0;
    gg->speed = 1.0f;
}

bool game_grid_update(GameGrid* gg)
{
    gg->next_update--;
    gg->last_movement_update++;

    // Input handling
    if (IsKeyDown(KEY_RIGHT)) {
        if (!gg->input_state.right_pressed_last_frame) {
            game_grid_move_dynamic_minos(1, 0);
            gg->right_key_hold_time = 0;
        } else {
            gg->right_key_hold_time++;
            if (gg->right_key_hold_time >= DAS_DELAY && (gg->right_key_hold_time - DAS_DELAY) % ARR_RATE == 0) {
                // TODO
                game_grid_move_dynamic_minos(1, 0);
            }
        }
    } else

    return true;
}

PieceType game_grid_pick_new_piece(GameGrid* gg)
{
    if (vec_piece_type_is_empty(&gg->bag)) {
        gg->bag = shape_manager_get_bag();
    }
    int piece_index = rand() % vec_piece_type_size(&gg->bag);
    PieceType piece = gg->bag.data[piece_index];
    vec_piece_type_iter iter = vec_piece_type_begin(&gg->bag);
    vec_piece_type_erase_at(&gg->bag, vec_piece_type_advance(iter, piece_index));

    return piece;
}

// Returns whether anything changed
// input should be (-1, 1) to move it left and down
bool game_grid_move_dynamic_minos(int right, int down)
{
    // TODO
    // https://github.com/FreddyMSchubert/Tetrix/blob/main/src/GameGrid.cpp#L323
}
