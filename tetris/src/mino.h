#ifndef MINO_H_
#define MINO_H_

#include "raylib.h"

#include "constants.h"


typedef struct {
    Color color;
    bool is_dynamic;
} Mino;


void mino_init(Color color, bool is_dynamic);
Color mino_get_color(Mino* mino);
bool mino_get_is_dynamic(Mino* mino);
Mino* mino_set_is_dynamic(Mino* mino, bool new_state);


#endif // MINO_H_
