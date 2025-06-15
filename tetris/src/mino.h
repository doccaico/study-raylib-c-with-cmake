#ifndef MINO_H_
#define MINO_H_

#include "raylib.h"

#include "structs.h"


typedef struct {
    Color color;
    bool isDynamic;
} Mino;

typedef struct {
    Mino* mino;
    GridPos pos;
} MinoPos;

Mino* MinoInit(Color color, bool isDynamic);


#endif // MINO_H_
