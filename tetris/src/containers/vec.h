#ifndef VEC_H_
#define VEC_H_


#include "../mino.h"

#define i_header

#define T box_mino, Mino //, (c_keyclass)
#include "stc/box.h"

// #define i_header
#define T vec_mino, box_mino, (c_keypro)
// #define i_keypro box_mino
#include "stc/vec.h"

// #define i_header
#define T vec_mm, vec_mino //, (c_keyclass)
#include "stc/vec.h"


#include "../constants.h"

// #define i_header
#define T vec_piece_type, PieceType
#include "stc/vec.h"

#endif // VEC_H_
