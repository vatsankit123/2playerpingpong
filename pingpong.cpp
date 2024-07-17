#include <iostream>
#include <raylib.h>
#include <vector>
#include <string>

using namespace std;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player1_score = 0;
int player2_score = 0;
const int winning_score = 10;
bool game_over = false;
string winner;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, BLACK);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;  // direction in horizontal
        }
        if (x + radius >= GetScreenWidth()) {  // player2 wins point
            player2_score++;
            ResetBall();
            CheckWinner();
        }
        if (x - radius <= 0) {  // player1 wins point
            player1_score++;
            ResetBall();
            CheckWinner();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = { -1, 1 };
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }

    void CheckWinner() {
        if (player1_score >= winning_score) {
            game_over = true;
            winner = "Player 1";
        } else if (player2_score >= winning_score) {
            game_over = true;
            winner = "Player 2";
        }
    }
};

class Paddle {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;
    int upKey, downKey;
    Color color;

    void Draw() {
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, color);
    }

    void Update() {
        if (IsKeyDown(upKey)) {
            y -= speed;
        }
        if (IsKeyDown(downKey)) {
            y += speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player1, player2;
Sound hitSound;

enum GameState { MENU, GAME, GAME_OVER };
GameState gameState = MENU;

vector<Color> colors = { BLUE, RED, GREEN, YELLOW };
vector<float> sizes = { 80, 120, 160 };
int player1_color_index = 0;
int player2_color_index = 1;
int player1_size_index = 1;
int player2_size_index = 1;

void ResetGame() {
    player1_score = 0;
    player2_score = 0;
    game_over = false;
    ball.ResetBall();
    player1.speed = 6;
    player2.speed = 6;
}

void DrawMenu() {
    DrawText("Customize Paddles", 100, 100, 40, WHITE);
    DrawText("Player 1: Use Arrow Keys", 100, 150, 20, WHITE);
    DrawText("Color: ", 100, 200, 20, WHITE);
    DrawRectangle(200, 200, 50, 50, colors[player1_color_index]);
    DrawText("Size: ", 100, 300, 20, WHITE);
    DrawText(TextFormat("%i", (int)sizes[player1_size_index]), 200, 300, 20, WHITE);

    DrawText("Player 2: Use W/S Keys", 600, 150, 20, WHITE);
    DrawText("Color: ", 600, 200, 20, WHITE);
    DrawRectangle(700, 200, 50, 50, colors[player2_color_index]);
    DrawText("Size: ", 600, 300, 20, WHITE);
    DrawText(TextFormat("%i", (int)sizes[player2_size_index]), 700, 300, 20, WHITE);

    DrawText("Press ENTER to Start", 400, 500, 30, WHITE);
}

int main() {
    cout << "Starting the game" << endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Two Player Ping Pong Game with Customizable Paddles");
    InitAudioDevice();  // Initialize audio device
    hitSound = LoadSound("hit.wav");  // Load sound file
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player1.width = 25;
    player1.height = sizes[player1_size_index];
    player1.x = screen_width - player1.width - 10;
    player1.y = screen_height / 2 - player1.height / 2;
    player1.speed = 6;
    player1.upKey = KEY_UP;
    player1.downKey = KEY_DOWN;
    player1.color = colors[player1_color_index];

    player2.width = 25;
    player2.height = sizes[player2_size_index];
    player2.x = 10;
    player2.y = screen_height / 2 - player2.height / 2;
    player2.speed = 6;
    player2.upKey = KEY_W;
    player2.downKey = KEY_S;
    player2.color = colors[player2_color_index];

    while (!WindowShouldClose()) {  // checks if the escape key is pressed
        BeginDrawing();

        ClearBackground(Dark_Green);

        switch (gameState) {
            case MENU:
                DrawMenu();

                if (IsKeyPressed(KEY_RIGHT)) player1_color_index = (player1_color_index + 1) % colors.size();
                if (IsKeyPressed(KEY_LEFT)) player1_color_index = (player1_color_index - 1 + colors.size()) % colors.size();
                if (IsKeyPressed(KEY_UP)) player1_size_index = (player1_size_index + 1) % sizes.size();
                if (IsKeyPressed(KEY_DOWN)) player1_size_index = (player1_size_index - 1 + sizes.size()) % sizes.size();

                if (IsKeyPressed(KEY_D)) player2_color_index = (player2_color_index + 1) % colors.size();
                if (IsKeyPressed(KEY_A)) player2_color_index = (player2_color_index - 1 + colors.size()) % colors.size();
                if (IsKeyPressed(KEY_W)) player2_size_index = (player2_size_index + 1) % sizes.size();
                if (IsKeyPressed(KEY_S)) player2_size_index = (player2_size_index - 1 + sizes.size()) % sizes.size();

                if (IsKeyPressed(KEY_ENTER)) {
                    player1.color = colors[player1_color_index];
                    player1.height = sizes[player1_size_index];
                    player1.y = screen_height / 2 - player1.height / 2;

                    player2.color = colors[player2_color_index];
                    player2.height = sizes[player2_size_index];
                    player2.y = screen_height / 2 - player2.height / 2;

                    gameState = GAME;
                }
                break;

            case GAME:
                if (!game_over) {
                    ball.Update();
                    player1.Update();
                    player2.Update();

                    // Checking for collision
                    if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player1.x, player1.y, player1.width, player1.height })) {
                        ball.speed_x *= -1;
                        PlaySound(hitSound);
                    }

                    if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player2.x, player2.y, player2.width, player2.height })) {
                        ball.speed_x *= -1;
                        PlaySound(hitSound);
                    }
                }

                DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
                DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
                DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, BLUE);
                ball.Draw();
                player1.Draw();
                player2.Draw();
                DrawText(TextFormat("%i", player2_score), screen_width / 4 - 20, 20, 80, WHITE);
                DrawText(TextFormat("%i", player1_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

                if (game_over) {
                    DrawText(TextFormat("%s Wins!", winner.c_str()), screen_width / 2 - MeasureText(TextFormat("%s Wins!", winner.c_str()), 80) / 2, screen_height / 2 - 40, 80, YELLOW);
                    DrawText("Press SPACE to Restart", screen_width / 2 - MeasureText("Press SPACE to Restart", 40) / 2, screen_height / 2 + 60, 40, WHITE);

                    if (IsKeyPressed(KEY_SPACE)) {
                        ResetGame();
                        gameState = MENU;
                    }
                }
                break;

            default:
                break;
        }

        EndDrawing();
    }

    UnloadSound(hitSound);  // Unload sound
    CloseAudioDevice();  // Close audio device
    CloseWindow();

    return 0;
}
