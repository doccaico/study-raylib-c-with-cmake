#include <stdlib.h>
#include <time.h>

#include "raylib.h"


#ifdef _DEBUG
#define WINDOW_TITLE "snake (debug)"
#else
#define WINDOW_TITLE "snake"
#endif

#define FRAMES_PER_SECOND 60
#define MS_PER_FRAME (1000 / FPS)
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640
#define GRID_WIDTH 32
#define GRID_HEIGHT 32


int main(void)
{
    srand((unsigned int)time(NULL));

    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT, GRID_WIDTH, GRID_HEIGHT);
    Controller controller;

    Game g;
    GameInit(kGridWidth, kGridHeight);

    GameRun(&g, &controller, &renderer, MS_PER_FRAME);

    puts("Game has terminated successfully!");
    printf("Score: %d\n", GameGetScore(&g));
    printf("Size: %d\n", GameGetSize(&g));

    return 0;
}
