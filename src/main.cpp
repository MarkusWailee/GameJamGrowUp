#include <raylib/raylib.h>
#include <raylib/rlgl.h>

#include "GameJam.h"
#include "math/vec.h"



//This was created for a 2 day game jam
//The code is obviously awful so please no judge

//All game logic will just be in the main function
void DefaultBoxUpdate(gj::Entity& e, gj::GameManager<MAX_ENTITY>* m)
{
    if(e.other_collide)
        e.color = {255, 0, 0, 255};
};

int main(void)
{
    float screenWidth = 960;
    float screenHeight = 600;

    gj::Entity default_entity;
    default_entity.AddComponentCollision(100, 100);
    default_entity.AddComponentPhysics(vec2(0, 1000), 1.0, 0.1);
    default_entity.AddComponentVisible();
    default_entity.color = {255, 255, 255, 255};
    default_entity.OnUpdate = DefaultBoxUpdate;


    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(0);
    rlDisableBackfaceCulling();

    gx::Shader shader(nullptr, "assets/shader/PixelWorld.frag");
    Camera2D camera{};
    camera.zoom = 1;

    gj::AnimatedSprite player_asset("assets/texture/character.png");
    for(int i = 0; i < 5; i++)
    {
        player_asset.AddFrame("run", {(float)i * 8, 8, 8, 8});
    }
    for(int i = 0; i < 5; i++)
    {
        player_asset.AddFrame("run2", {(float)i * 8, 16, 8, 8});
    }
    player_asset.AddFrame("idle", {0, 0, 8, 8});
    player_asset.AddFrame("idle2", {8, 0, 8, 8});
    player_asset.PlayAnim("idle2");

    gj::GameManager<128> manager;

    gj::Entity temp;
    temp.AddComponentVisible();
    temp.color = {255, 255, 0, 255};
    temp.AddComponentCollision(GetScreenWidth(), 10);

    //top
    temp.pos = vec2(0, 0);
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



    gj::Entity player_entity;
    player_entity.AddComponentCollision(80, 80);
    player_entity.AddComponentPhysics(vec2(0, 1000), 10, 0.f); //gravity


    manager.player = player_entity;

    gj::Entity& player = manager.player;
    player.type = gj::EntityType::PLAYER;
    float player_input_timer = 0;
    float player_speed = 100;
    float player_accel = 1000;

    auto ParticleUpate = [](gj::Entity& e, gj::GameManager<MAX_ENTITY>* m) -> void
    {
        //vec4 col = vec4(e.color.r, e.color.g, e.color.b, e.color.a);
        vec4 c1 = vec4(255, 255, 0, 255);
        vec4 c2 = vec4(255, 0, 0, 255);
        vec4 r = mix(c2, c1, 1.0/e.lifetime_remaining);
        e.color = {(unsigned char)r.x, (unsigned char)r.y, (unsigned char)r.z, (unsigned char)r.w};
    };



    auto DrawPlayer = [&]
    {
        player_asset.Draw((manager.player.pos), 0, {10,10}, false);
    };

    auto SlowDownPlayer = [&]
    {
        player.vel.x -= sign(player.vel.x) * GetFrameTime() * 200;
    };

    float timer = 0;
    auto PlayerInput = [&]
    {
        bool is_key_pressed = false;
        if(IsKeyReleased(KEY_D) || IsKeyReleased(KEY_A))
            player_input_timer = 0;
        if(IsKeyDown(KEY_D))
        {
            player.vel.x += GetFrameTime() * player_accel;
            is_key_pressed = true;
            if(player.vel.x < 0)
                SlowDownPlayer();
        }
        if(IsKeyDown(KEY_SPACE))
        {
            player.vel.y = min(-200.0f, player.vel.y);
            player.vel.y -= GetFrameTime() * 1000;
            timer += GetFrameTime();
            if(timer > 0.01)
            {
                timer = 0;
                gj::Entity e;
                e.width = 10;
                e.height = 10;
                e.pos = player.pos + vec2(GetRandomValue(20, 60), 100);
                e.vel = vec2(GetRandomValue(-10, 10), 100);
                e.AddComponentLifetime(1);
                e.AddComponentVisible();
                e.AddComponentPhysics(vec2(0, 100), 0.01, 1.0f);
                e.color = {255, 255, 255, 255};
                e.OnUpdate = ParticleUpate;
                manager.AddEntity(e);
            }
        }
        if(IsKeyDown(KEY_A))
        {
            player.vel.x -= GetFrameTime() * player_accel;
            is_key_pressed = true;
            if(player.vel.x > 0)
                SlowDownPlayer();
        }
        if(!is_key_pressed)
        {
            SlowDownPlayer();
        }
        else
        {
            //player.vel.x = sign(player.vel.x) * max(abs(player.vel.x), player_speed);
        }
    };

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {

        PlayerInput();

        if(IsKeyPressed(KEY_F))
        {
            default_entity.pos = vec2(GetMouseX(), GetMouseY());
            default_entity.color.r = GetRandomValue(0, 255);
            default_entity.color.g = GetRandomValue(0, 255);
            default_entity.color.b = GetRandomValue(0, 255);
            default_entity.width = GetRandomValue(10, 200);
            default_entity.height = GetRandomValue(10, 200);
            manager.AddEntity(default_entity);
        }


        BeginDrawing();
        ClearBackground(Color{30, 30, 40, 255});

        player_asset.Update();
        manager.UpdateAll();

        manager.DrawEntities();
        DrawPlayer();

        DrawText(TextFormat("fps = %d", GetFPS()), 10, 10, 32, WHITE);
        EndDrawing();
    }


    CloseWindow(); // Close window and OpenGL context

    return 0;
}
