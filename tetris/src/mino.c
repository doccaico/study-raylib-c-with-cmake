#include "raylib.h"

#include "mino.h"
#include "utils.h"

Mino* MinoInit(Color color, bool isDynamic)
{
	Mino* mino = malloc_or_die(sizeof(Mino));
    mino->color = color;
    mino->isDynamic = isDynamic;
    return mino;
}
