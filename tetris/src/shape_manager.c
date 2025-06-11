#include "shape_manager.h"
// #include "structs.h"
// #include "constants.h"
#include "mino.h"

// #include "containers/vec.h"

// box_mino mino_init(Color color, bool is_dynamic);

// box_mino mino_init(Color, bool);
// Mino* mino_init(Color, bool);

vec_mino shape_manager_get_piece(PieceType type, GridPos *pivot)
{
    vec_mino piece = vec_mino_with_capacity(8);

    switch (type) {
        case I:
            *pivot = (GridPos){0, 1};
            vec_mino_push(&piece, mino_init(I_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(I_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(I_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(I_PIECE_COLOR, true));

            break;
        case J:
            *pivot = (GridPos){1, 1};
            vec_mino_push(&piece, mino_init(J_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, mino_init(J_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(J_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(J_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            break;
        case L:
            *pivot = (GridPos){1, 1};
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, mino_init(L_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, mino_init(L_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(L_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(L_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            break;
        case O:
            *pivot = (GridPos){0, 0};
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, mino_init(O_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(O_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, mino_init(O_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(O_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            break;
        case T:
            *pivot = (GridPos){1, 1};
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, mino_init(T_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, mino_init(T_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(T_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(T_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            break;
        case S:
            *pivot = (GridPos){1, 1};
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, mino_init(S_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(S_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, mino_init(S_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(S_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, NULL);
            break;
        case Z:
            *pivot = (GridPos){1, 1};
            vec_mino_push(&piece, mino_init(Z_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(Z_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, NULL);
            vec_mino_push(&piece, mino_init(Z_PIECE_COLOR, true));
            vec_mino_push(&piece, mino_init(Z_PIECE_COLOR, true));
            vec_mino_push(&piece, NULL);
            break;
        default:
            break;
    }
    return piece;
}

vec_piece_type shape_manager_get_bag(void)
{
    vec_piece_type bag = vec_piece_type_with_capacity(7);

    for (int i = 0; i < 7; ++i) {
        vec_piece_type_push(&bag, piece_type_from_int(i));
    }
    return bag;
}

PieceType piece_type_from_int(int n)
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
            assert(0 && "reach unreachable (piece_type_from_int)");
            ret = 0;
            break;
    }
    return ret;
}
