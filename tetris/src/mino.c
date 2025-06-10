#include "raylib.h"

#include "mino.h"


void mino_init(Mino* mino, Color color, bool is_dynamic) {
    mino->color = color;
    mino->is_dynamic = is_dynamic;
}

Color mino_get_color(Mino* mino) {
    return mino->color;
}

bool mino_get_is_dynamic(Mino* mino) {
    return mino->is_dynamic;
}

void mino_set_is_dynamic(Mino* mino, bool new_state) {
    mino->is_dynamic = new_state;
}
