#include "game_grid.h"
#include "shape_manager.h"
#include "constants.h"
// #include "mino.h"

#include "yac_dynamic_array.h"

static void InputStateInit(InputState* input_state)
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

void GameGridInit(GameGrid* gg)
{
    gg->grid = (DaDaMino){0};

    gg->upcoming_pieces = (DaPieceType){0};
    for (size_t i = 0; i < MAX_PIECE_QUEUE; ++i) {
        YacDynamicArrayAppend(&gg->upcoming_pieces, GameGridPickNewPiece(gg));
    }

    gg->bag = (DaPieceType){0};
    gg->held_piece = NULL;
    gg->next_update = DROP_SPEED;
    InputStateInit(&gg->input_state);
    gg->last_movement_update = 0;
    gg->right_key_hold_time = 0;
    gg->left_key_hold_time = 0;
    gg->level = 0;
    gg->score = 0;
    gg->lines_cleared = 0;
    gg->speed = 1.0f;
}

bool GameGridUpdate(GameGrid* gg)
{
    gg->next_update--;
    gg->last_movement_update++;

    // Input handling
    if (IsKeyDown(KEY_RIGHT)) {
        if (!gg->input_state.right_pressed_last_frame) {
            GameGridMoveDynamicMinos(1, 0);
            gg->right_key_hold_time = 0;
        } else {
            gg->right_key_hold_time++;
            if (gg->right_key_hold_time >= DAS_DELAY && (gg->right_key_hold_time - DAS_DELAY) % ARR_RATE == 0) {
                // TODO
                GameGridMoveDynamicMinos(1, 0);
            }
        }
    } else

    return true;
}

PieceType GameGridPickNewPiece(GameGrid* gg)
{
    // if (vec_piece_type_is_empty(&gg->bag)) {
    if (gg->bag.len == 0) {
        gg->bag = ShapeManagerGetBag();
    }
    // int piece_index = rand() % vec_piece_type_size(&gg->bag);
    int piece_index = rand() % gg->bag.len;
    // PieceType piece = gg->bag.data[piece_index];
    PieceType piece = gg->bag.items[piece_index];
    // vec_piece_type_iter iter = vec_piece_type_begin(&gg->bag);
    // vec_piece_type_erase_at(&gg->bag, vec_piece_type_advance(iter, piece_index));
    YacDynamicArrayRemove(&gg->bag, piece_index);

    return piece;
}

// Returns whether anything changed
// input should be (-1, 1) to move it left and down
bool GameGridMoveDynamicMinos(int right, int down)
{
    // TODO
    // https://github.com/FreddyMSchubert/Tetrix/blob/main/src/GameGrid.cpp#L323
}
