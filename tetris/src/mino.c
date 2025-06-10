#include "raylib.h"

#include "mino.h"
#include "utils.h"


Mino* mino_init(Color color, bool is_dynamic)
{

	Mino* mino = malloc_or_die(sizeof(Mino));

    mino->color = color;
    mino->is_dynamic = is_dynamic;

    return mino;
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
