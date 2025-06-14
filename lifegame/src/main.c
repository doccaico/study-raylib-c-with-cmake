#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#ifdef _DEBUG
#define WINDOW_TITLE "lifegame (debug)"
#else
#define WINDOW_TITLE "lifegame"
#endif

#define CELL_SIZE (2)
#define INITIAL_CELL_COUNT (80)
#define FPS (30)

#define SCREEN_WIDTH (480)
#define SCREEN_HEIGHT (640)
#define COL_SIZE (SCREEN_WIDTH / CELL_SIZE + 2)
#define ROW_SIZE (SCREEN_HEIGHT / CELL_SIZE + 2)

int grid[ROW_SIZE][COL_SIZE];
int neighbors[ROW_SIZE][COL_SIZE];
Color cell_color;
Color bg_color;

void initializeColor(void)
{
    cell_color = BLACK;
    bg_color = RAYWHITE;
}

void initializeGrid(void)
{
    for (int i = 0; i < COL_SIZE; ++i) {
        // top
        grid[0][i] = 0;
        // bottom
        grid[ROW_SIZE - 1][i] = 0;
    }

    for (int i = 0; i < ROW_SIZE; ++i) {
        // left
        grid[i][0] = 0;
        // right
        grid[i][COL_SIZE - 1] = 0;
    }

    for (int i = 1; i < ROW_SIZE - 1; ++i) {
        for (int j = 1; j < COL_SIZE - 1; ++j) {
            grid[i][j] = (1 <= j && j <= INITIAL_CELL_COUNT) ? 1 : 0;
        }
    }
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(int* array, int n)
{
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(&array[i], &array[j]);
    }
}

void randomize(void)
{
    for (int i = 0; i < ROW_SIZE - 1; ++i) {
        shuffle(&grid[i][1], COL_SIZE - 2);
    }
}

void draw(void)
{
    for (int i = 1; i < ROW_SIZE - 1; ++i) {
        for (int j = 1; j < COL_SIZE - 1; ++j) {
            if (grid[i][j] == 1) {
                DrawRectangle(
                        CELL_SIZE * (j - 1),
                        CELL_SIZE * (i - 1),
                        CELL_SIZE,
                        CELL_SIZE,
                        cell_color
                        );
            }
        }
    }
}

void nextGeneration(void)
{
    for (int i = 1; i < ROW_SIZE - 1; ++i) {
        for (int j = 1; j < COL_SIZE - 1; ++j) {
            // top = top-left + top-middle + top-right
            int top = grid[i - 1][j - 1] + grid[i - 1][j] + grid[i - 1][j + 1];
            // middle = left + right
            int middle = grid[i][j - 1] + grid[i][j + 1];
            // bottom = bottom-left + bottom-middle + bottom-right
            int bottom = grid[i + 1][j - 1] + grid[i + 1][j] + grid[i + 1][j + 1];

            neighbors[i][j] = top + middle + bottom;
        }
    }

    for (int i = 1; i < ROW_SIZE - 1; ++i) {
        for (int j = 1; j < COL_SIZE - 1; ++j) {
            switch (neighbors[i][j]) {
                case 2:
                    // Do nothing
                    break;
                case 3:
                    grid[i][j] = 1;
                    break;
                default:
                    grid[i][j] = 0;
                    break;
            }
        }
    }
}

//   randRange(0, 5) == 0..=5
unsigned char randRange(int min, int max)
{
    return (unsigned char)(rand() % (max + 1 - min));
}

void changeBgColor(void)
{
    bg_color = (Color){randRange(0, 255), randRange(0, 255), randRange(0, 255), 255};
}

void changeCellColor(void)
{
    cell_color = (Color){randRange(0, 255), randRange(0, 255), randRange(0, 255), 255};
}

int main(void)
{
    srand((unsigned int)time(NULL));

    initializeColor();
    initializeGrid();
    randomize();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            initializeGrid();
            randomize();
        } else if (IsKeyPressed(KEY_B)) {
            changeBgColor();
        } else if (IsKeyPressed(KEY_C)) {
            changeCellColor();
        }

        BeginDrawing();
        ClearBackground(bg_color);
        draw();
        nextGeneration();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
