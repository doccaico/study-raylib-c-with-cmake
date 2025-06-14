#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "raylib.h"

#include "yac_dynamic_array.h"

#ifdef _DEBUG
#define WINDOW_TITLE "flappy (debug)"
#else
#define WINDOW_TITLE "flappy"
#endif

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360
#define FPS 60

#define JUMP -4.0
// 壁の追加間隔
#define INTERVAL 120
// 壁の初期x座標
#define WALL_START_X 640
// 穴のy座標の最大値
#define HOLE_Y_MAX 150
// GOPHERの幅
#define GOPHER_WIDTH 60
// GOPHERの高さ
#define GOPHER_HEIGHT 75
// 穴のサイズ（高さ）
#define HOLE_HEIGHT 170
// 壁の高さ
#define WALL_HEIGHT 360
// 壁の幅
#define WALL_WIDTH 20

typedef enum {
    SCENE_GAME_TITLE,
    SCENE_GAME_PLAY,
    SCENE_GAME_OVER,
} Scene;

typedef struct {
    float x;
    float y;
    int width;
    int height;
} Gopher;

typedef struct {
    int wall_x;
    int hole_y;
} Wall;

typedef struct {
    Wall* items;
    size_t len;
    size_t capacity;
} DaWall;

typedef struct {
    Gopher gopher;
    float velocity;
    float gravity;
    int frames;
    int old_score;
    int new_score;
    const char* score_string;
    DaWall walls;
    Scene scene;
} Game;

typedef enum {
    TEXTURE_GOPHER,
    TEXTURE_SKY,
    TEXTURE_WALL,
} TEXTURE;


// Forward Declaration
void loadTextures(void);
void initGame(Game* game);
void drawTitle(Game* game);
void drawGame(Game* game);
void drawGameOver(Game* game);

// Global Variables
const char* file_names[3] = {
    "gopher",
    "sky",
    "wall",
};
Texture2D textures[3];
const char* assets_path = "assets/";
const char* suffix = ".png";


void loadTextures(void)
{
    char path[32];
    size_t len = sizeof(file_names) / sizeof(file_names[0]);
    for (size_t i = 0; i < len; ++i) {
        strcpy(path, assets_path);
        strcat(path, file_names[i]);
        strcat(path, suffix);

        Image asset = LoadImage(path);

        Texture2D texture = LoadTextureFromImage(asset);
        textures[i] = texture;

        UnloadImage(asset);
    }
}

void initGame(Game* game)
{
    game->gopher =
        (Gopher){
            .x      = 200.0f,
            .y      = 150.0f,
            .width  = 60,
            .height = 75,
        };
    game->velocity = 0.0f;
    game->gravity = 0.1f;
    game->frames = 0;
    game->old_score = 0;
    game->new_score = 0;
    game->score_string = "Score: 0";
    game->scene = SCENE_GAME_TITLE;
    game->walls = (DaWall){0};
}

void drawTitle(Game* game)
{
    DrawTexture(textures[TEXTURE_SKY], 0, 0, WHITE);
    DrawText("Click!", (WINDOW_WIDTH / 2) - 40, WINDOW_HEIGHT / 2, 20, WHITE);
    DrawTexture(textures[TEXTURE_GOPHER], (int)game->gopher.x, (int)game->gopher.y, WHITE);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        game->scene = SCENE_GAME_PLAY;
    }
}

void drawGame(Game* game)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        game->velocity = JUMP;
    }
    game->velocity += game->gravity;
    game->gopher.y += game->velocity;

    game->frames += 1;
    if (game->frames % INTERVAL == 0) {
        int min = 0, max = HOLE_Y_MAX - 1;
        int rand_num = rand() % (max - min + 1) + min;
        YacDynamicArrayAppend(&game->walls, ((Wall){.wall_x = WALL_START_X, .hole_y = rand_num}));
    }

    // wallを左へ移動
    for (size_t i = 0; i < game->walls.len; ++i) {
        game->walls.items[i].wall_x -= 2;
    }

    // スコアを計算
    for (size_t i = 0; i < game->walls.len; ++i) {
        if (game->walls.items[i].wall_x < (int)game->gopher.x) {
            game->new_score = (int)i + 1;
        }
    }
    // スコアの文字列を生成
    if (game->new_score != game->old_score) {
        game->score_string = TextFormat("Score: %d", game->new_score);
        game->old_score = game->new_score;
    }

    DrawTexture(textures[TEXTURE_SKY], 0, 0, WHITE);
    DrawTexture(textures[TEXTURE_GOPHER], (int)game->gopher.x, (int)game->gopher.y, WHITE);


    for (size_t i = 0; i < game->walls.len; ++i) {
        int wall_x = game->walls.items[i].wall_x;
        int hole_y = game->walls.items[i].hole_y;
        float x = game->gopher.x;
        float y = game->gopher.y;

        // 上の壁の描画
        DrawTexture(textures[TEXTURE_WALL], wall_x, hole_y - WALL_HEIGHT, WHITE);
        // 下の壁の描画
        DrawTexture(textures[TEXTURE_WALL], wall_x, hole_y + HOLE_HEIGHT, WHITE);

        // gopherを表す四角形を作る
        int g_left = (int)x;
        int g_top = (int)y;
        int g_right = (int)x + GOPHER_WIDTH;
        int g_bottom = (int)y + GOPHER_HEIGHT;

        // 上の壁を表す四角形を作る
        int w_left = wall_x;
        int w_top = hole_y - WALL_HEIGHT;
        int w_right = wall_x + WALL_WIDTH;
        int w_bottom = hole_y;

        // 上の壁との当たり判定
        if (g_left < w_right && w_left < g_right && g_top < w_bottom && w_top < g_bottom) {
            game->scene = SCENE_GAME_OVER;
        }

        // 下の壁を表す四角形を作る
        w_left = wall_x;
        w_top = hole_y + HOLE_HEIGHT;
        w_right = wall_x + WALL_WIDTH;
        w_bottom = hole_y + HOLE_HEIGHT + WALL_HEIGHT;

        // 下の壁との当たり判定
        if (g_left < w_right && w_left < g_right && g_top < w_bottom && w_top < g_bottom) {
            game->scene = SCENE_GAME_OVER;
        }
    }

    // スコアを描画
    DrawText(game->score_string, 10, 10, 20, RED);

    // 上の壁との当たり判定
    if (game->gopher.y < 0) {
        game->scene = SCENE_GAME_OVER;
    }
    // 地面との当たり判定
    if (360 - GOPHER_HEIGHT < game->gopher.y) {
        game->scene = SCENE_GAME_OVER;
    }
}

void drawGameOver(Game* game)
{
    DrawTexture(textures[TEXTURE_SKY], 0, 0, WHITE);
    DrawTexture(textures[TEXTURE_GOPHER], (int)game->gopher.x, (int)game->gopher.y, WHITE);

    for (size_t i = 0; i < game->walls.len; ++i) {
        int wall_x = game->walls.items[i].wall_x;
        int hole_y = game->walls.items[i].hole_y;
        DrawTexture(textures[TEXTURE_WALL], wall_x, hole_y - WALL_HEIGHT, WHITE);
        DrawTexture(textures[TEXTURE_WALL], wall_x, hole_y + HOLE_HEIGHT, WHITE);
    }
    DrawText("Game Over", (WINDOW_WIDTH / 2) - 60, (WINDOW_HEIGHT / 2) - 60, 20, WHITE);

    game->score_string = TextFormat("Score: %d", game->new_score);
    DrawText(game->score_string, (WINDOW_WIDTH / 2) - 60, (WINDOW_HEIGHT / 2) - 40, 20, WHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        game->scene = SCENE_GAME_TITLE;
        YacDynamicArrayClearAndFree(game->walls);
        initGame(game);
    }
}

int main(void)
{
    srand((unsigned int)time(NULL));

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);

    loadTextures();

    Game game;
    initGame(&game);

    RenderTexture2D render_texture = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

    while (!WindowShouldClose()) {
        BeginTextureMode(render_texture);
        {
            switch (game.scene) {
                case SCENE_GAME_TITLE:
                    drawTitle(&game);
                    break;
                case SCENE_GAME_PLAY:
                    drawGame(&game);
                    break;
                case SCENE_GAME_OVER:
                    drawGameOver(&game);
                    break;
                default:
                    break;
            }
        }
        EndTextureMode();

        BeginDrawing();
        {
            float w = (float)render_texture.texture.width;
            float h = (float)render_texture.texture.height;
            Rectangle source = {0, 0, w, h};
            Rectangle dest = source;
            source.height = -source.height;
            DrawTexturePro(
                    render_texture.texture,
                    source,
                    dest,
                    (Vector2){0.0f, 0.0f},
                    0,
                    WHITE);
        }
        EndDrawing();
    }

    UnloadRenderTexture(render_texture);
    if (!YacDynamicArrayIsNull(game.walls)) {
        YacDynamicArrayClearAndFree(game.walls);
    }

    CloseWindow();

    return 0;
}
