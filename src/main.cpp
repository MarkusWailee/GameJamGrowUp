#include <raylib/raylib.h>
#include <raylib/rlgl.h>

#include "physics.h"
#include "graphics.h"

int main(void)
{
    float screenWidth = 960;
    float screenHeight = 600;




    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(0);

    gx::Shader shader(nullptr, "assets/shader/PixelWorld.frag");
    Camera2D camera{};
    camera.zoom = 1;

    float tex[200 * 200]{};
    for(int i = 0; i < 200 * 200; i++)
    {
        tex[i] = 0.5f;
    }


    //Texture2D texture = LoadTextureFromImage(image);
    Image image = LoadImage("assets/texture/FinnSprite.png");
    gx::Texture texture(tex, 200, 200, PIXELFORMAT_UNCOMPRESSED_R32);

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if(IsKeyDown(KEY_D))
            camera.offset.x += GetFrameTime() * 100;
        if(IsKeyDown(KEY_A))
            camera.offset.x -= GetFrameTime() * 100;



        BeginDrawing();
        ClearBackground(Color{0, 0, 0, 255});

        px::Rect a = {(float)GetMouseX(), (float)GetMouseY(), 100, 100};
        px::Rect b = {200, 200, 100, 100};
        DrawRectangleLines(a.x, a.y, a.width, a.height, WHITE);
        DrawRectangleLines(b.x, b.y, b.width, b.height, WHITE);

        px::CollideShape(a, b, [&](vec2 normal, float distance)
        {
            vec2 result = vec2(a.x, a.y) + normal * distance;
            DrawRectangleLines(result.x, result.y, a.width, a.height, GREEN);
        });

        // gx::Circle c1 = {300, 300, 100};
        // gx::Circle c2 = {(float)GetMouseX(), (float)GetMouseY(), 200};
        //
        // DrawCircle(c1.x, c1.y, c1.r, WHITE);
        // DrawCircle(c2.x, c2.y, c2.r, WHITE);
        // gx::CollideShape(c1, c2, [&](vec2 normal, float distance)
        // {
        //     vec2 result = vec2(c1.x, c1.y) + normal * distance;
        //     DrawCircle(result.x, result.y, c1.r, GREEN);
        // });


        // shader.Start([&]{
        //     BeginMode2D(camera);
        //     shader.SetSampler2D(texture.GetTexture(), "texture1");
        //
        //
        //     rlBegin(RL_QUADS);
        //
        //     rlTexCoord2f(1, 1);
        //     rlVertex2f(300, 100);
        //
        //     rlTexCoord2f(1, 0);
        //     rlVertex2f(300, 0);
        //
        //     rlTexCoord2f(0, 0);
        //     rlVertex2f(0, 0);
        //
        //     rlTexCoord2f(0, 1);
        //     rlVertex2f(0, 100);
        //
        //     rlEnd();
        //     EndMode2D();
        // });

        DrawText(TextFormat("fps = %d", GetFPS()), 10, 10, 32, WHITE);
        EndDrawing();
    }


    CloseWindow(); // Close window and OpenGL context

    return 0;
}
