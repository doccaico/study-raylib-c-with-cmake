#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_grid.h"

#include "constants.h"
#include "mino.h"
#include "shape_manager.h"
#include "utils.h"

#include "raylib.h"
#include "yac_dynamic_array.h"
#define YAC_STRING_VIEW_IMPLEMENTATION
#include "yac_string_view.h"
#define YAC_STRING_BUILDER_IMPLEMENTATION
#include "yac_string_builder.h"
#include "yac_types.h"

static void InputStateInit(InputState* input_state);
static GridPos GameGridGetRotationAroundPivot(GameGrid* gg, GridPos pos, bool clockwise);
static PieceType GameGridPickNewPiece(GameGrid* gg);
static bool GameGridMoveDynamicMinos(GameGrid* gg, int right, int down);
static bool GameGridRotateDynamicMinos(GameGrid* gg, bool clockwise);
static void GameGridHoldPiece(GameGrid* gg);
static bool GameGridDeleteDynamicMinos(GameGrid* gg);
static bool GameGridSpawnNewTetromino(GameGrid* gg);


void GameGridInit(GameGrid* gg)
{
    gg->grid = (DaDaMino){0};

    for (size_t i = 0; i < CELLS_Y; ++i) {
        DaMino da_mino = {0};
        YacDynamicArrayAppend(&gg->grid, da_mino);
    }

    for (size_t i = 0; i < CELLS_Y; ++i) {
        for (size_t j = 0; j < CELLS_X; ++j) {
        YacDynamicArrayAppend(&gg->grid.items[i], NULL);
        }
    }

    gg->bag = (DaPieceType){0};

    gg->upcoming_pieces = (DaPieceType){0};
    for (size_t i = 0; i < MAX_PIECE_QUEUE; ++i) {
        YacDynamicArrayAppend(&gg->upcoming_pieces, GameGridPickNewPiece(gg));
    }

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

void GameGridDeinit(GameGrid* gg)
{
    for (size_t i = 0; i < gg->grid.len; ++i) {
        for (size_t j = 0; j < gg->grid.items[i].len; ++j) {
            if (gg->grid.items[i].items[j] != NULL) {
                free(gg->grid.items[i].items[j]);
            }
        }
        YacDynamicArrayClearAndFree(gg->grid.items[i]);
    }

    YacDynamicArrayClearAndFree(gg->grid);
    YacDynamicArrayClearAndFree(gg->upcoming_pieces);
    YacDynamicArrayClearAndFree(gg->bag);

    if (gg->held_piece != NULL) {
        free(gg->held_piece);
    }
}

bool GameGridUpdate(GameGrid* gg)
{
    gg->next_update--;
    gg->last_movement_update++;

    //
    // Input handling
    //

    // KEY_RIGHT
    if (IsKeyDown(KEY_RIGHT)) {
        if (!gg->input_state.right_pressed_last_frame) {
            GameGridMoveDynamicMinos(gg, 1, 0);
            gg->right_key_hold_time = 0;
        } else {
            gg->right_key_hold_time++;
            if (gg->right_key_hold_time >= DAS_DELAY && (gg->right_key_hold_time - DAS_DELAY) % ARR_RATE == 0) {
                GameGridMoveDynamicMinos(gg, 1, 0);
            }
        }
    } else {
        gg->right_key_hold_time = 0;
    }

    // KEY_LEFT
    if (IsKeyDown(KEY_LEFT)) {
        if (!gg->input_state.left_pressed_last_frame) {
            GameGridMoveDynamicMinos(gg, -1, 0);
            gg->left_key_hold_time = 0;
        } else {
            gg->left_key_hold_time++;
            if (gg->left_key_hold_time >= DAS_DELAY && (gg->left_key_hold_time - DAS_DELAY) % ARR_RATE == 0) {
                GameGridMoveDynamicMinos(gg, -1, 0);
            }
        }
    } else {
        gg->left_key_hold_time = 0;
    }

    // KEY_DOWN
    if (IsKeyDown(KEY_DOWN) && !gg->input_state.down_pressed_last_frame)
        gg->next_update = 0;

    // KEY_SPACE
    if (!IsKeyDown(KEY_SPACE) && gg->input_state.space_pressed_last_frame) {
        while (GameGridMoveDynamicMinos(gg, 0, 1));
        gg->last_movement_update = PIECE_STATIFICATION_DELAY;
    }

    // KEY_UP
    if (!IsKeyDown(KEY_UP) && gg->input_state.up_pressed_last_frame)
        GameGridRotateDynamicMinos(gg, false);

    // KEY_A
    if (!IsKeyDown(KEY_A) && gg->input_state.a_pressed_last_frame)
        GameGridRotateDynamicMinos(gg, false);

    // KEY_D
    if (!IsKeyDown(KEY_D) && gg->input_state.d_pressed_last_frame)
        GameGridRotateDynamicMinos(gg, true);

    // KEY_LEFT_SHIFT
    if (!IsKeyDown(KEY_LEFT_SHIFT) && gg->input_state.shift_pressed_last_frame)
        GameGridHoldPiece(gg);

    gg->input_state.right_pressed_last_frame = IsKeyDown(KEY_RIGHT);
    gg->input_state.left_pressed_last_frame = IsKeyDown(KEY_LEFT);
    gg->input_state.down_pressed_last_frame = IsKeyDown(KEY_DOWN);
    gg->input_state.up_pressed_last_frame = IsKeyDown(KEY_UP);
    gg->input_state.space_pressed_last_frame = IsKeyDown(KEY_SPACE);
    gg->input_state.a_pressed_last_frame = IsKeyDown(KEY_A);
    gg->input_state.d_pressed_last_frame = IsKeyDown(KEY_D);
    gg->input_state.shift_pressed_last_frame = IsKeyDown(KEY_LEFT_SHIFT);

    // Update grid state
    if (gg->next_update == 0) {

        gg->next_update = (unsigned int)(DROP_SPEED * gg->speed);
        if (gg->input_state.down_pressed_last_frame && gg->next_update > SPED_UP_DROP_SPEED)
            gg->next_update = SPED_UP_DROP_SPEED;

        bool midAir = true;
        for (size_t i = 0; i < gg->grid.len; ++i) {
            for (size_t j = 0; j < gg->grid.items[i].len; ++j) {
                if (gg->grid.items[i].items[j] == NULL)
                    continue;
                Mino* dm = gg->grid.items[i].items[j];
                if (dm == NULL || !dm->isDynamic)
                    continue;
                if (i + 1 < gg->grid.len && gg->grid.items[i + 1].items[j] != NULL) {
                    Mino* dmGoal = gg->grid.items[i + 1].items[j];
                    if (dmGoal == NULL || !dmGoal->isDynamic)
                        midAir = false;
                } else if (i + 1 == gg->grid.len)
                    midAir = false;
            }
        }
        if (gg->last_movement_update < PIECE_STATIFICATION_DELAY && !midAir)
            return true;

        // Make minos static
        if (!GameGridMoveDynamicMinos(gg, 0, 1)) {
            for (size_t i = 0; i < gg->grid.len; ++i) {
                for (size_t j = 0; j < gg->grid.items[i].len; ++j) {
                    if (gg->grid.items[i].items[j] == NULL)
                        continue;
                    Mino* dm = gg->grid.items[i].items[j];
                    if (dm == NULL || !dm->isDynamic)
                        continue;
                    gg->grid.items[i].items[j]->isDynamic = false;
                }
            }

            // Check for full rows
            int clearedRows = 0;
            for (size_t i = 0; i < gg->grid.len; ++i) {
                bool rowFull = true;
                for (size_t j = 0; j < gg->grid.items[i].len; ++j) {
                    if (gg->grid.items[i].items[j] == NULL) {
                        rowFull = false;
                        break;
                    }
                }
                if (rowFull) {
                    clearedRows++;
                    for (size_t j = 0; j < gg->grid.items[i].len; ++j) {
                        free(gg->grid.items[i].items[j]);
                        gg->grid.items[i].items[j] = NULL;
                    }
                    for (size_t k = i; k != 0; --k) {
                        for (size_t j = 0; j < gg->grid.items[k].len; ++j) {
                            gg->grid.items[k].items[j] = gg->grid.items[k - 1].items[j];
                            gg->grid.items[k - 1].items[j] = NULL;
                        }
                    }
                }
            }
            gg->lines_cleared += clearedRows;
            unsigned int prevLevel = gg->level;
            gg->level = gg->lines_cleared / 10;
            if (gg->level != prevLevel)
                gg->speed *= LEVEL_UP_SPEED_INCREASE;
            if (clearedRows > 0) {
                switch (clearedRows) {
                    case 1:
                        gg->score += 40 * (gg->level + 1);
                        break;
                    case 2:
                        gg->score += 100 * (gg->level + 1);
                        break;
                    case 3:
                        gg->score += 300 * (gg->level + 1);
                        break;
                    case 4:
                        gg->score += 1200 * (gg->level + 1);
                        break;
                    default:
                        puts("What? How did you clear something other than 0-4 rows?");
                }
            }

            // Then, spawn in a new piece
            if (!GameGridSpawnNewTetromino(gg))
                return false;
        }
    }

    return true;
}

void GameGridDraw(GameGrid* gg)
{

    // Draw grid
    for (int i = 0; i < CELLS_X; ++i) {
        for (int j = 0; j < CELLS_Y; ++j) {
            int x = i * CELL_SIZE;
            int y = j * CELL_SIZE;
            DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, GRID_COLOR);
        }
    }

    // Draw minos
    for (size_t i = 0; i < gg->grid.len; ++i)
        for (size_t j = 0; j < gg->grid.items[i].len; ++j)
            if (gg->grid.items[i].items[j] != NULL)
                DrawRectangle(
                        (int)j * CELL_SIZE, (int)i * CELL_SIZE,
                        CELL_SIZE, CELL_SIZE,
                        gg->grid.items[i].items[j]->color);

    // Draw progress
    int leftOffset = (int)gg->grid.items[0].len * CELL_SIZE + 5;
    {
        YacStringBuilder sb = YacStringBuilderCreate(0);
        YacStringBuilderAppendFormat(&sb, "LVL %u", gg->level);
        DrawText(YacStringBuilderGetCstr(&sb), leftOffset, 20, 15, WHITE);
        YacStringBuilderClearAndFree(sb);
    }
    {
        YacStringBuilder sb = YacStringBuilderCreate(0);
        YacStringBuilderAppendFormat(&sb, "SCR %u", gg->score);
        DrawText(YacStringBuilderGetCstr(&sb), leftOffset, 35, 15, WHITE);
        YacStringBuilderClearAndFree(sb);
    }
    {
        YacStringBuilder sb = YacStringBuilderCreate(0);
        YacStringBuilderAppendFormat(&sb, "LNS %u", gg->lines_cleared);
        DrawText(YacStringBuilderGetCstr(&sb), leftOffset, 50, 15, WHITE);
        YacStringBuilderClearAndFree(sb);
    }

    // Draw upcoming pieces
    int margin = 10;
    int pieceSize = 4 * CELL_SIZE;
    for (size_t i = 0; i < PIECE_LOOKAHEAD; ++i) {
        GridPos pos;
        DaMino pieces = ShapeManagerGetPiece(gg->upcoming_pieces.items[i], &pos);
        for (size_t j = 0; j < pieces.len; ++j) {
            if (pieces.items[j] == NULL)
                continue;
            int x = (int)gg->grid.items[0].len * CELL_SIZE + margin + (j % 4) * CELL_SIZE;
            int y = (int)(margin + (i * pieceSize) + (j / 4) * CELL_SIZE + (pieceSize * 2));
            DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, pieces.items[j]->color);
        }
        for (size_t j = 0; j < pieces.len; j++)
            if (pieces.items[j] != NULL) {
                free(pieces.items[j]);
            }

        YacDynamicArrayClearAndFree(pieces);
    }

    // Draw held piece
    DrawRectangle((int)gg->grid.items[0].len * CELL_SIZE + margin, pieceSize, pieceSize, pieceSize, DARKGRAY);
    if (gg->held_piece != NULL) {
        GridPos pos;
        DaMino pieces = ShapeManagerGetPiece(*gg->held_piece, &pos);
        for (size_t j = 0; j < pieces.len; ++j) {
            if (pieces.items[j] == NULL)
                continue;
            int x = (int)gg->grid.items[0].len * CELL_SIZE + margin + (j % 4) * CELL_SIZE;
            int y = (int)(margin + (j / 4) * CELL_SIZE + pieceSize);
            DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, pieces.items[j]->color);
        }
        for (size_t j = 0; j < pieces.len; ++j) {
            if (pieces.items[j] != NULL) {
                free(pieces.items[j]);
            }
        }
        YacDynamicArrayClearAndFree(pieces);
    }
}

static void InputStateInit(InputState* input_state)
{
    input_state->right_pressed_last_frame = false; // Move right
    input_state->left_pressed_last_frame = false; // Move left
    input_state->down_pressed_last_frame = false; // Drop faster
    input_state->up_pressed_last_frame = false; // Rotate
    input_state->space_pressed_last_frame = false; // Hard drop
    input_state->a_pressed_last_frame = false; // Rotate left
    input_state->d_pressed_last_frame = false; // Rotate right
    input_state->shift_pressed_last_frame = false; // Rotate right
}

static GridPos GameGridGetRotationAroundPivot(GameGrid* gg, GridPos pos, bool clockwise)
{
    int relX = pos.x - gg->pivot.x;
    int relY = pos.y - gg->pivot.y;

    int newRelX, newRelY;

    if (clockwise) {
        newRelX = relY;
        newRelY = -relX;
    } else {
        newRelX = -relY;
        newRelY = relX;
    }

    GridPos newPos;
    newPos.x = newRelX + gg->pivot.x;
    newPos.y = newRelY + gg->pivot.y;

    return newPos;
}

static PieceType GameGridPickNewPiece(GameGrid* gg)
{
    if (gg->bag.len == 0) {
        gg->bag = ShapeManagerGetBag();
    }
    int piece_index = rand() % gg->bag.len;
    PieceType piece = gg->bag.items[piece_index];
    YacDynamicArrayRemove(&gg->bag, piece_index);
    return piece;
}

// Returns whether anything changed input should be (-1, 1) to move it left and down
static bool GameGridMoveDynamicMinos(GameGrid* gg, int right, int down)
{
    bool changeOccurred = false;

    // 1. Move horizontally
    if (right != 0) {
        // Check if minos can move
        bool dynamicMinosMovable = true;
        bool dynamicMinosPresent = false;

        for (size_t i = 0; i < gg->grid.len; ++i) {
            for (size_t j = 0; j < gg->grid.items[i].len; ++j) {
                Mino *dm = gg->grid.items[i].items[j];
                if (gg->grid.items[i].items[j] == NULL || dm == NULL || !dm->isDynamic)
                    continue;
                dynamicMinosPresent = true;
                if (j + right < 0 || j + right >= gg->grid.items[i].len) {
                    dynamicMinosMovable = false;
                    break;
                }
                if (gg->grid.items[i].items[j + right] == NULL)
                    continue;
                Mino *dmGoal = gg->grid.items[i].items[j];
                if (!dmGoal->isDynamic) {
                    dynamicMinosMovable = false;
                    break;
                }
            }
            if (!dynamicMinosMovable)
                break;
        }

        // Move minos
        size_t start = right > 0 ? gg->grid.items[0].len - 1 : 0;
        size_t end = right > 0 ? -1 : gg->grid.items[0].len;
        size_t step = right > 0 ? -1 : 1;
        if (dynamicMinosMovable && dynamicMinosPresent) {
            gg->pivot.x += right;
            for (size_t i = 0; i < gg->grid.len; ++i) {
                for (size_t j = start; j != end; j += step) {
                    if (gg->grid.items[i].items[j] == NULL)
                        continue;
                    Mino *dm = gg->grid.items[i].items[j];
                    if (!dm->isDynamic)
                        continue;
                    if (gg->grid.items[i].items[j + right] == NULL) {
                        gg->grid.items[i].items[j + right] = gg->grid.items[i].items[j];
                        gg->grid.items[i].items[j] = NULL;
                        changeOccurred = true;
                    }
                }
            }
        }
    }

    // 2. Move vertically
    if (down != 0) {
        // Check if minos can move
        bool dynamicMinosMovable = true;
        bool dynamicMinosPresent = false;
        for (size_t i = 0; i < gg->grid.len; ++i) {
            for (size_t j = 0; j < gg->grid.items[i].len; ++j) {
                Mino *dm = gg->grid.items[i].items[j];
                if (gg->grid.items[i].items[j] == NULL || dm == NULL || !dm->isDynamic)
                    continue;
                dynamicMinosPresent = true;
                if (i + down < 0 || i + down >= gg->grid.len) {
                    dynamicMinosMovable = false;
                    break;
                }
                if (gg->grid.items[i + down].items[j] == NULL)
                    continue;
                Mino *dmGoal = gg->grid.items[i + down].items[j];
                if (!dmGoal->isDynamic) {
                    dynamicMinosMovable = false;
                    break;
                }
            }
            if (!dynamicMinosMovable)
                break;
        }

        // Move minos
        size_t start = down > 0 ? gg->grid.len - 1 : 0;
        size_t end = down > 0 ? -1 : gg->grid.len;
        size_t step = down > 0 ? -1 : 1;
        if (dynamicMinosMovable && dynamicMinosPresent) {
            gg->pivot.y += down;
            for (size_t i = start; i != end; i += step) {
                for (size_t j = 0; j < gg->grid.items[i].len; ++j) {
                    if (gg->grid.items[i].items[j] == NULL)
                        continue;
                    Mino *dm = gg->grid.items[i].items[j];
                    if (dm == NULL || !dm->isDynamic)
                        continue;
                    if (gg->grid.items[i + down].items[j] == NULL) {
                        gg->grid.items[i + down].items[j] = gg->grid.items[i].items[j];
                        gg->grid.items[i].items[j] = NULL;
                        changeOccurred = true;
                    }
                }
            }
        }
    }

    if (changeOccurred)
        gg->last_movement_update = 0;

    return changeOccurred;
}

static bool GameGridRotateDynamicMinos(GameGrid* gg, bool clockwise)
{
    DaMinoPos dynamicMinos = {0};
    DaMinoPos newDynamicMinos = {0};

    // 1. Get a vector of all dynamic minos
    for (size_t i = 0; i < gg->grid.len; ++i) {
        for (size_t j = 0; j < gg->grid.items[i].len; ++j) {
            Mino *dm = gg->grid.items[i].items[j];
            if (gg->grid.items[i].items[j] == NULL || dm == NULL || !dm->isDynamic)
                continue;
            MinoPos mp = {.mino = dm, .pos = (GridPos){(int)i, (int)j}};
            YacDynamicArrayAppend(&dynamicMinos, mp);
        }
    }
    if (dynamicMinos.len == 0)
        return false;

    // 2. Check if minos can rotate
    for (size_t i = 0; i < dynamicMinos.len; ++i) {
        GridPos newPos = GameGridGetRotationAroundPivot(gg, dynamicMinos.items[i].pos, clockwise);
        if (newPos.x < 0 || newPos.x >= gg->grid.items[0].len
                || newPos.y < 0 || newPos.y >= gg->grid.len)
            return false;
        if (gg->grid.items[newPos.y].items[newPos.x] != NULL
                && !gg->grid.items[newPos.y].items[newPos.x]->isDynamic)
            return false;
        YacDynamicArrayAppend(&newDynamicMinos, ((MinoPos){dynamicMinos.items[i].mino, newPos}));
    }

    // 3. Rotate minos
    for (size_t i = 0; i < dynamicMinos.len; ++i) {
        int j = dynamicMinos.items[i].pos.y;
        int k = dynamicMinos.items[i].pos.x;
        gg->grid.items[j].items[k] = NULL;
    }
    for (size_t i = 0; i < newDynamicMinos.len; ++i) {
        int j = newDynamicMinos.items[i].pos.y;
        int k = newDynamicMinos.items[i].pos.x;
        gg->grid.items[j].items[k] = newDynamicMinos.items[i].mino;
    }

    if (!(newDynamicMinos.len == 0))
        gg->last_movement_update = 0;

    bool ret = !(newDynamicMinos.len == 0);

    YacDynamicArrayClearAndFree(dynamicMinos);
    YacDynamicArrayClearAndFree(newDynamicMinos);

    return ret;
}

static void GameGridHoldPiece(GameGrid* gg)
{
    if (gg->held_piece == NULL) {
        PieceType* pt = malloc_or_die(sizeof(PieceType));
        *pt = gg->current_piece;
        gg->held_piece = pt;

        GameGridDeleteDynamicMinos(gg);
        GameGridSpawnNewTetromino(gg);
    } else {
        PieceType nextPiece = *gg->held_piece;

        free(gg->held_piece);

        PieceType* pt = malloc_or_die(sizeof(PieceType));
        *pt = gg->current_piece;
        gg->held_piece = pt;

        GameGridDeleteDynamicMinos(gg);
        YacDynamicArrayInsert(&gg->upcoming_pieces, 0, nextPiece);
        GameGridSpawnNewTetromino(gg);
    }
}

static bool GameGridDeleteDynamicMinos(GameGrid* gg)
{
    bool changeOccurred = false;
    for (size_t j = 0; j < gg->grid.len; ++j) {
        for (size_t i = 0; i < gg->grid.items[j].len; ++i) {
            if (gg->grid.items[j].items[i] == NULL) {
                continue;
            }
            Mino *dm = gg->grid.items[j].items[i];
            if (dm == NULL || !dm->isDynamic) {
                continue;
            }
            free(gg->grid.items[j].items[i]);
            gg->grid.items[j].items[i] = NULL;
            changeOccurred = true;
        }
    }
    return changeOccurred;
}

static bool GameGridSpawnNewTetromino(GameGrid* gg)
{
    PieceType type = gg->upcoming_pieces.items[0];
    YacDynamicArrayRemove(&gg->upcoming_pieces, 0);
    if (gg->upcoming_pieces.len < PIECE_LOOKAHEAD) {
        for (size_t i = gg->upcoming_pieces.len; i < PIECE_LOOKAHEAD; ++i)
            YacDynamicArrayAppend(&gg->upcoming_pieces, GameGridPickNewPiece(gg));
    }
    gg->current_piece = type;

    DaMino pieces = ShapeManagerGetPiece(type, &gg->pivot);
    int spawningOffset = (CELLS_X - 4) / 2;

    gg->pivot.x += spawningOffset;
    for (size_t i = 0; i < pieces.len; ++i) {
        if (pieces.items[i] == NULL)
            continue;
        if (gg->grid.items[i / 4].items[(i % 4) + spawningOffset] != NULL) {
            puts(" ---------------- ");
            puts(" -- Game Over! -- ");
            puts(" ---------------- ");
            YacDynamicArrayClearAndFree(pieces);
            return false;
        }
        gg->grid.items[i / 4].items[(i % 4) + spawningOffset] = pieces.items[i];
    }

    YacDynamicArrayClearAndFree(pieces);

    return true;
}

