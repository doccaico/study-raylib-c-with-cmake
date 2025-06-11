#include "shape_manager.h"
// #include "structs.h"
// #include "constants.h"
// #include "mino.h"

// #include "containers/vec.h"

// box_mino mino_init(Color color, bool is_dynamic);
box_mino mino_init(Color, bool);

vec_mino shape_manager_get_piece(PieceType type, GridPos *pivot)
{
    // vec_mino piece = {0};
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

            // piece = c_make(vec_mino, {
            //         mino_init(I_PIECE_COLOR, true),
            //         mino_init(I_PIECE_COLOR, true),
            //         mino_init(I_PIECE_COLOR, true),
            //         mino_init(I_PIECE_COLOR, true),
            //         });
            break;
        default:
            break;
    }

    return piece;
}
