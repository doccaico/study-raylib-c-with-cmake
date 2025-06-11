#ifndef MINO_H_
#define MINO_H_

#include "../../_vendor/raylib/include/raylib.h"
// #include "raylib.h"

// #include "containers/vec.h"

// #include "game_grid.h"
// #include "constants.h"

// typedef struct {
//     Color color;
//     bool is_dynamic;
// } Mino;

struct Mino_t{
    Color color;
    bool is_dynamic;
};
typedef struct Mino_t Mino;


// box_mino mino_init(Color color, bool is_dynamic);
Mino* mino_init(Color color, bool is_dynamic);

// struct Mino_t;
//

// typedef struct Mino_t Mino;


// #define i_header // header definitions only
// #define T box_mino, Mino //, (c_keyclass)
// #include "stc/box.h"

// box_mino mino_init(Color color, bool is_dynamic);
// void mino_init(Color color, bool is_dynamic);
// Color mino_get_color(box_mino bm);
// bool mino_get_is_dynamic(Mino* mino);
// void mino_set_is_dynamic(Mino* mino, bool new_state);

#endif // MINO_H_
