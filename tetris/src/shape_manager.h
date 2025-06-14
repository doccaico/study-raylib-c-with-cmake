#ifndef SHAPE_MANAGER_H_
#define SHAPE_MANAGER_H_

#include "constants.h"
#include "structs.h"
#include "yac_types.h"


DaMino ShapeManagerGetPiece(PieceType type, GridPos *pivot);
DaPieceType ShapeManagerGetBag(void);
// static PieceType PieceTypeFromInt(int n);


#endif // SHAPE_MANAGER_H_
