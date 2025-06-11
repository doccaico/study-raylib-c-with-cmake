#ifndef SHAPE_MANAGER_H_
#define SHAPE_MANAGER_H_

#include "structs.h"
#include "constants.h"

#include "containers/vec.h"


vec_mino shape_manager_get_piece(PieceType type, GridPos *pivot);


#endif // SHAPE_MANAGER_H_
