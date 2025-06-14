#include "mino.h"
#include "shape_manager.h"
#include "yac_types.h"

#include "yac_dynamic_array.h"


static PieceType PieceTypeFromInt(int n);


DaMino ShapeManagerGetPiece(PieceType type, GridPos *pivot)
{
    DaMino pieces = {0};

    switch (type) {
        case I:
            *pivot = (GridPos){0, 1};
            YacDynamicArrayAppend(&pieces, MinoInit(I_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(I_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(I_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(I_PIECE_COLOR, true));

            break;
        case J:
            *pivot = (GridPos){1, 1};
            YacDynamicArrayAppend(&pieces, MinoInit(J_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, MinoInit(J_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(J_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(J_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            break;
        case L:
            *pivot = (GridPos){1, 1};
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, MinoInit(L_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, MinoInit(L_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(L_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(L_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            break;
        case O:
            *pivot = (GridPos){0, 0};
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, MinoInit(O_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(O_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, MinoInit(O_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(O_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            break;
        case T:
            *pivot = (GridPos){1, 1};
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, MinoInit(T_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, MinoInit(T_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(T_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(T_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            break;
        case S:
            *pivot = (GridPos){1, 1};
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, MinoInit(S_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(S_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, MinoInit(S_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(S_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, NULL);
            break;
        case Z:
            *pivot = (GridPos){1, 1};
            YacDynamicArrayAppend(&pieces, MinoInit(Z_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(Z_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, NULL);
            YacDynamicArrayAppend(&pieces, MinoInit(Z_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, MinoInit(Z_PIECE_COLOR, true));
            YacDynamicArrayAppend(&pieces, NULL);
            break;
        default:
            break;
    }
    return pieces;
}

DaPieceType ShapeManagerGetBag(void)
{
    DaPieceType bag = {0};

    for (size_t i = 0; i < 7; ++i) {
        YacDynamicArrayAppend(&bag, PieceTypeFromInt((int)i));
    }
    return bag;
}

static PieceType PieceTypeFromInt(int n)
{
    PieceType ret;

    switch (n) {
        case I:
            ret = 0;
            break;
        case J:
            ret = 1;
            break;
        case L:
            ret = 2;
            break;
        case O:
            ret = 3;
            break;
        case T:
            ret = 4;
            break;
        case S:
            ret = 5;
            break;
        case Z:
            ret = 6;
            break;
        default:
            assert(0 && "reach unreachable (PieceTypeFromInt)");
            ret = 0;
            break;
    }
    return ret;
}
