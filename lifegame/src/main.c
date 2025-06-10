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

#define WINDOW_WIDTH (480)
#define WINDOW_HEIGHT (640)
#define COL_SIZE (WINDOW_WIDTH / CELL_SIZE + 2)
#define ROW_SIZE (WINDOW_HEIGHT / CELL_SIZE + 2)

int grid[ROW_SIZE][COL_SIZE];
int neighbors[ROW_SIZE][COL_SIZE];
Color cell_color;
Color bg_color;

void initialize_color(void)
{
    cell_color = BLACK;
    bg_color = RAYWHITE;
}

void initialize_grid(void)
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

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(int *array, int n)
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

void next_generation(void)
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

//   rand_range(0, 5) == 0..=5
unsigned char rand_range(int min, int max)
{
    return (unsigned char)(rand() % (max + 1 - min));
}

void change_bg_color(void)
{
    bg_color = (Color){rand_range(0, 255), rand_range(0, 255), rand_range(0, 255), 255};
}

void change_cell_color(void)
{
    cell_color = (Color){rand_range(0, 255), rand_range(0, 255), rand_range(0, 255), 255};
}

int main(void)
{
    srand((unsigned int)time(NULL));

    initialize_color();
    initialize_grid();
    randomize();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            initialize_grid();
            randomize();
        } else if (IsKeyPressed(KEY_B)) {
            change_bg_color();
        } else if (IsKeyPressed(KEY_C)) {
            change_cell_color();
        }

        BeginDrawing();
        ClearBackground(bg_color);
        draw();
        next_generation();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
