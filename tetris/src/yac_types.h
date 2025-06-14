#ifndef YAC_TYPES_H_
#define YAC_TYPES_H_

#include "constants.h"
#include "mino.h"

typedef struct {
    Mino** items;
    size_t len;
    size_t capacity;
} DaMino;

typedef struct {
    DaMino* items;
    size_t len;
    size_t capacity;
} DaDaMino;

typedef struct {
    PieceType* items;
    size_t len;
    size_t capacity;
} DaPieceType;

#endif // YAC_TYPES_H_
