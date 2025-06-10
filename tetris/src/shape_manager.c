#include "shape_manager.h"

vec_mino shape_manager_get_piece(PieceType type, GridPos *pivot)
{
    vec_mino piece = vec_mino_with_capacity(8);

    switch (type) {
        case I:
            *pivot = (GridPos){0, 1};
            // TODO
            // piece = {
            //     new Mino(I_PIECE_COLOR, true),
            //     new Mino(I_PIECE_COLOR, true),
            //     new Mino(I_PIECE_COLOR, true),
            //     new Mino(I_PIECE_COLOR, true)
            // };
            vec_mino_push(&piece, mino_init(I_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(I_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(I_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(I_PIECE_COLOR, true));
            break;
        default:
            break;
    }

    return piece;
}
