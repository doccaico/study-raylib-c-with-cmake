#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "game_grid.h"
#include "constants.h"


int main(void)
{
    srand((unsigned int)time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);

    GameGrid grid;
    game_grid_init(&grid);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!game_grid_update(&grid)) {
            break; // game over
        }


        EndDrawing();
    }

    CloseWindow();

    return 0;
}
