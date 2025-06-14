#include "raylib.h"

#include "yac_dynamic_array.h"

#ifdef _DEBUG
#define WINDOW_TITLE "breakout (debug)"
#else
#define WINDOW_TITLE "breakout"
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20
#define BALL_SIZE 10

#define BRICK_ROWS 5
#define BRICK_COLS 10
#define BRICK_SPACING 5
#define BRICK_WIDTH ((SCREEN_WIDTH - (BRICK_COLS + 1) * BRICK_SPACING) / BRICK_COLS)
#define BRICK_HEIGHT 20

typedef struct {
    Rectangle rect;
    bool active;
} Brick;

typedef struct {
    Brick* items;
    size_t len;
    size_t capacity;
} DaBrick;


DaBrick createBricks(void)
{
    DaBrick bricks = {0};

    for (size_t y = 0; y < BRICK_ROWS; ++y) {
        for (size_t x = 0; x < BRICK_COLS; ++x) {
            Brick brick;

            brick.rect.x = (float)BRICK_SPACING + x * (BRICK_WIDTH + BRICK_SPACING);
            brick.rect.y = (float)BRICK_SPACING + y * (BRICK_HEIGHT + BRICK_SPACING);
            brick.rect.width = BRICK_WIDTH;
            brick.rect.height = BRICK_HEIGHT;
            brick.active = true;

            YacDynamicArrayAppend(&bricks, brick);
        }
    }

    return bricks;
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);

    // Paddle
    Rectangle paddle = {SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2, SCREEN_HEIGHT - 40, PADDLE_WIDTH, PADDLE_HEIGHT};
    float paddle_speed = 6.0f;

    // Ball
    Vector2 ball_pos = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    Vector2 ball_vel = {4.0f, -4.0f};

    // Bricks
    DaBrick bricks = createBricks();

    bool game_over = false;
    bool win = false;

    while (!WindowShouldClose()) {
        // Input
        if (IsKeyDown(KEY_LEFT) && paddle.x > 0)
            paddle.x -= paddle_speed;
        if (IsKeyDown(KEY_RIGHT) && paddle.x + paddle.width < SCREEN_WIDTH)
            paddle.x += paddle_speed;

        if (!game_over) {
            // Ball Movement
            ball_pos.x += ball_vel.x;
            ball_pos.y += ball_vel.y;

            // Wall collision
            if (ball_pos.x <= 0 || ball_pos.x >= SCREEN_WIDTH - BALL_SIZE)
                ball_vel.x *= -1;
            if (ball_pos.y <= 0)
                ball_vel.y *= -1;

            // Bottom (game over)
            if (ball_pos.y >= SCREEN_HEIGHT) {
                game_over = true;
                win = false;
            }

            // Paddle collision
            Rectangle ball_rect = {ball_pos.x, ball_pos.y, (float)BALL_SIZE, (float)BALL_SIZE};
            if (CheckCollisionRecs(ball_rect, paddle)) {
                ball_vel.y *= -1;
                ball_pos.y = paddle.y - BALL_SIZE;
            }

            // Brick collision
            for (size_t i = 0; i < bricks.len; ++i) {
                if (bricks.items[i].active && CheckCollisionRecs(ball_rect, bricks.items[i].rect)) {
                    bricks.items[i].active = false;
                    ball_vel.y *= -1;
                    break;
                }
            }

            // Win check
            win = true;
            for (size_t i = 0; i < bricks.len; ++i) {
                if (bricks.items[i].active) {
                    win = false;
                    break;
                }
            }

            if (win) {
                game_over = true;
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw paddle
        DrawRectangleRec(paddle, DARKGRAY);

        // Draw ball
        DrawRectangle((int)ball_pos.x, (int)ball_pos.y, BALL_SIZE, BALL_SIZE, MAROON);

        // Draw bricks
        for (size_t i = 0; i < bricks.len; ++i) {
            if (bricks.items[i].active) {
                DrawRectangleRec(bricks.items[i].rect, BLUE);
            }
        }

        // Messages
        if (game_over) {
            const char* msg = win ? "YOU WIN!" : "GAME OVER";
            DrawText(msg, SCREEN_WIDTH / 2 - MeasureText(msg, 40) / 2, SCREEN_HEIGHT / 2, 40, RED);
            DrawText("Press R to Restart", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50, 20, DARKGRAY);
            if (IsKeyPressed(KEY_R)) {
                // Reset
                ball_pos = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
                ball_vel = (Vector2){4.0f, -4.0f};
                paddle.x = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;

                // Free
                YacDynamicArrayClearAndFree(bricks);

                bricks = createBricks();
                game_over = false;
                win = false;
            }
        }

        EndDrawing();
    }

    YacDynamicArrayClearAndFree(bricks);

    CloseWindow();

    return 0;
}
