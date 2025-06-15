#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "constants.h"
#include "game_grid.h"


int main(void)
{
    srand((unsigned int)time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);

    GameGrid gg;
    GameGridInit(&gg);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (!GameGridUpdate(&gg)) {
            break; // game over
        }

        GameGridDraw(&gg);

        EndDrawing();
    }

    GameGridDeinit(&gg);

    CloseWindow();

    return 0;
}
