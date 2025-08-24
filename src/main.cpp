#include <raylib/raylib.h>
#include <raylib/rlgl.h>

#include "GameJam.h"
#include "math/vec.h"
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


    gj::EntityManager<64> manager;

    gj::Entity temp;
    temp.AddComponentVisible();
    temp.color = {255, 255, 0, 255};
    temp.AddComponentCollision(GetScreenWidth(), 10);

    //top
    temp.pos = vec2(0, 10);
    manager.AddEntity(temp);

    //bottom
    temp.pos = vec2(0, GetScreenHeight() - 20);
    manager.AddEntity(temp);

    //left
    temp.width = 10;
    temp.height = GetScreenHeight();
    temp.pos = vec2(0, 0);
    manager.AddEntity(temp);

    temp.pos = vec2(GetScreenWidth()-20, 0);
    manager.AddEntity(temp);

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if(IsKeyDown(KEY_D))
            camera.offset.x += GetFrameTime() * 100;
        if(IsKeyDown(KEY_A))
            camera.offset.x -= GetFrameTime() * 100;

        if(IsKeyPressed(KEY_F))
        {
            gj::Entity e;
            e.AddComponentVisible();
            e.color = {255, 0, 0, 255};
            e.pos = vec2(GetRandomValue(0, 400), GetRandomValue(0, 400));
            e.AddComponentLifetime(10);
            e.AddComponentPhysics(vec2(0, 100), 1.f);
            e.AddComponentCollision(100, 100);
            manager.AddEntity(e);
        }

        if(IsKeyPressed(KEY_SPACE))
        {
            gj::Entity e;
            e.AddComponentVisible();
            e.color = {255, 255, 255, 255};
            e.pos = vec2(GetScreenWidth()/2, GetScreenHeight()/2);
            e.AddComponentLifetime(5);
            e.AddComponentPhysics(vec2(50, 59), 1.f);
            e.AddComponentCollision(10, 10);
            e.vel = vec2(GetMouseX(), GetMouseY()) - e.pos;
            e.vel = normalize(e.vel) * 700;
            manager.AddEntity(e);

        }


        BeginDrawing();
        ClearBackground(Color{0, 0, 0, 255});

        manager.UpdateAll();

        manager.DrawEntities();
        gj::Rect a = {(float)GetMouseX(), (float)GetMouseY(), 100, 100};
        gj::Rect b = {200, 200, 300, 30};
        DrawRectangleLines(a.x, a.y, a.width, a.height, WHITE);
        DrawRectangleLines(b.x, b.y, b.width, b.height, WHITE);

        gj::CollideShape(a, b, [&](vec2 normal, float distance)
        {
            vec2 result = vec2(a.x, a.y) + normal * distance;
            DrawRectangleLines(result.x, result.y, a.width, a.height, GREEN);
        });

        vec2 p = {400, 400};
        vec2 A = vec2(GetMouseX(), GetMouseY()) - p;
        vec2 B = vec2(0, -100);
        vec2 C = project(A, B);
        DrawLine(p.x, p.y, p.x + A.x, p.y + A.y, WHITE);
        DrawLine(p.x, p.y, p.x + B.x, p.y + B.y, WHITE);
        DrawLine(p.x, p.y, p.x + C.x, p.y + C.y, RED);

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
