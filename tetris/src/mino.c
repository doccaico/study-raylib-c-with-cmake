#include "raylib.h"

// #include "game_grid.h"
#include "mino.h"
#include "utils.h"
#include "containers/vec.h"


// box_mino mino_init(Color color, bool is_dynamic)
// {
// 	// Mino* mino = malloc_or_die(sizeof(Mino));
//     // mino->color = color;
//     // mino->is_dynamic = is_dynamic;
//
//     // box_mino bm = box_mino_init();
//
//     return box_mino_from(c_literal(Mino){ .color=color, .is_dynamic=is_dynamic });
//
//     // bm.get->color = color;
//     // mino->color = color;
//     // mino->is_dynamic = is_dynamic;
//
//     // return bm;
// }

Mino* mino_init(Color color, bool is_dynamic)
{
	Mino* mino = malloc_or_die(sizeof(Mino));
    mino->color = color;
    mino->is_dynamic = is_dynamic;

    // box_mino bm = box_mino_init();

    // return box_mino_from(c_literal(Mino){ .color=color, .is_dynamic=is_dynamic });

    // bm.get->color = color;
    // mino->color = color;
    // mino->is_dynamic = is_dynamic;

    return mino;
}

// Color mino_get_color(box_mino bm) {
//     return *bm.get->color;
// }
//
// bool mino_get_is_dynamic(Mino* mino) {
//     return mino->is_dynamic;
// }
//
// void mino_set_is_dynamic(Mino* mino, bool new_state) {
//     mino->is_dynamic = new_state;
// }
