#include <raylib/raylib.h>
#include <raylib/rlgl.h>

#include "GameJam.h"
#include "math/vec.h"
#include "ui/ui.hpp"



//This was created for a 2 day game jam
//The code is obviously awful so please no judge

//All game logic will just be in the main function
constexpr int MY_BEST = 24000;
enum Menu
{
    PAUSE,
    WIN,
    PLAYING,
    FREEPLAY,
};
gj::GameManager<128> manager;
gj::Camera camera;
vec2 reset_pos = {64, 96};
const float MAX_TIME = 30;
float score_timer = 0;
int highscore = 0;
int current_score = 0;
int generated_score = 0;
Menu menu = PAUSE;
void AddLife(Color c)
{
    current_score += c.r + c.g + c.b;
}
// I have no time left
int GetGeneratedScore() { return generated_score; }
int GetHighScore() //this was a last minute thought;
{
    //this value was computed when initially computing score after generation
    return highscore;
}
int GetScore() //this was a last minute thought;
{
    return current_score;
}
namespace Player
{
    float player_speed = 12;
    float player_accel = 120;
    float timer = 0;
    float timer2 = 0;
    gj::AnimatedSprite player_asset;
    gj::Entity& player = manager.player;
    void Init();
    void Draw();
    void Input();
    void Update();
}
namespace Seed
{
    void Onkill(gj::Entity& e);
    bool OnCollision(gj::Entity& e, gj::Entity& other, gj::GameManager<MAX_ENTITY>* m);
}

namespace Particle
{
    float timer = 0;
    void ParticleUpdate(gj::Entity& e, gj::GameManager<MAX_ENTITY>* m);
}

gj::BlockWorld world;
namespace World
{
    void Init();
    void GenerateWorld();
    void OnDetection(gj::Entity& self, gj::Entity& other, gj::GameManager<MAX_ENTITY>* m);
}



void DrawGame();
void UpdateGame();
void InitGame(); //Make sure initialize after raylib
void Reset();
void Restart();

// UI COLORS

constexpr int FONT_SIZE0 = 48;
constexpr int FONT_SIZE1 = 64;
constexpr int FONT_SIZE2 = 96;
constexpr int FONT_SIZE3 = 160;
constexpr UI::Color GOLD0 = {225, 185, 20, 255};
constexpr UI::Color ORANGE0 = {255, 127, 80, 255};
constexpr UI::Color BLACK0 = {30, 30, 30, 255};
constexpr UI::Color GREEN0 = {20, 220, 20, 255};
constexpr UI::Color RED0 = {216, 82, 75, 255};
constexpr UI::Color GREEN1 = {100, 255, 100, 255};
constexpr UI::Color PURPLE0 = {80, 80, 220, 255};
constexpr UI::Color PURPLE1 = {100, 100, 255, 255};

void WinMenu(UI::Context* ui);
void StartMenu(UI::Context* ui);
void PlayMenu(UI::Context* ui);
UI::Color GetScoreColor(int amount)
{
    if(amount <= 50)
        return RED0;
    else if(amount <= 75)
        return ORANGE0;
    else if(amount <= 100)
        return GREEN1;
    else if(amount > 100)
        return PURPLE1;
    else
        return RED0;
}


bool DisableCollisions(gj::Entity& a, gj::Entity& b, gj::GameManager<MAX_ENTITY>* m)
{
    if(a.type == b.type)
        return false;
    return true;
}


//return true if you want to enable collision
bool DefaultBox_OnCollision(gj::Entity& self, gj::Entity& other, gj::GameManager<MAX_ENTITY>* m)
{
    if(other.type == gj::EntityType::PLAYER)
    {
        self.color = {255, 0, 0, 255};
        self.AddComponentLifetime(1);
    }
    return true;
}




int main(void)
{

    float screenWidth = 1080;
    float screenHeight = 680;



    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Grow Rush");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(0);
    rlDisableBackfaceCulling();




    UI::Init_impl("assets/font/Tiny5-Regular.ttf");
    UI::Context ui(1 * UI::MB);

    InitGame();




    uint64_t compute_score = 0;
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        //Last minute computing score. Just adds up all color values
        if(compute_score > 500 && score_timer < MAX_TIME)
        {
            compute_score = 0;
            current_score = 0;
            for(Color c : world.data)
                AddLife(c);

            current_score -= GetGeneratedScore();
            current_score /= MY_BEST;
        }
        compute_score++;
        if(IsKeyPressed(KEY_F))
        {
            Vector2 c = GetScreenToWorld2D(GetMousePosition(), camera.GetCamera());
            gj::Entity default_entity;
            default_entity.AddComponentPhysics(vec2(0, 100), 1.0, 0.1);
            default_entity.AddComponentVisible();
            default_entity.AddComponentCollision(GetRandomValue(10, 20), GetRandomValue(10, 20));
            default_entity.color = {255, 255, 255, 255};
            default_entity.pos = vec2(c.x, c.y);
            default_entity.color.r = GetRandomValue(0, 255);
            default_entity.color.g = GetRandomValue(0, 255);
            default_entity.color.b = GetRandomValue(0, 255);
            default_entity.AddComponentLifetime(10);
            //default_entity.OnUpdate = DefaultBoxUpdate;
            default_entity.OnCollision = DefaultBox_OnCollision;
            manager.AddEntity(default_entity);
        }
        if(IsKeyPressed(KEY_R))
            Reset();

        if(IsKeyPressed(UI::KEY_ESCAPE))
            menu = PAUSE;

        BeginDrawing();
        //ClearBackground(Color{30, 30, 40, 255});
        vec3 c1 = vec3{30, 30, 40};
        vec3 c2 = vec3(103 ,149 ,156);
        vec3 r = mix(c1, c2, min((float)GetScore() / 100, 1.0f));
        ClearBackground(Color{(unsigned char)r.x, (unsigned char)r.y, (unsigned char)r.z, 255});


        DrawGame();
        if(menu == PLAYING)
        {
            UpdateGame();
            PlayMenu(&ui);
        }
        if(menu == FREEPLAY)
            UpdateGame();
        if(menu == PAUSE)
            StartMenu(&ui);
        if(menu == WIN)
        {
            WinMenu(&ui);
            if(GetScore() > highscore)
                highscore = GetScore();
        }



        //DrawText(TextFormat("fps = %d", GetFPS()), 10, 10, 32, WHITE);
        EndDrawing();
    }


    CloseWindow(); // Close window and OpenGL context

    return 0;
}



void InitGame()
{
    Player::Init();
    World::Init();

    gj::Entity bounds;
    bounds.AddComponentVisible();
    bounds.color = {50, 50, 50, 50};
    bounds.AddComponentCollision(world.GetWidth(), 1);

    //top
    bounds.pos = vec2(0, 0);
    manager.AddEntity(bounds);

    //bottom
    bounds.pos = vec2(0, world.GetHeight() - 1);
    manager.AddEntity(bounds);

    //left
    bounds.width = 1;
    bounds.height = world.GetHeight();
    bounds.pos = vec2(0, 0);
    manager.AddEntity(bounds);

    bounds.pos = vec2(world.GetWidth()-1, 0);
    manager.AddEntity(bounds);



}
void Reset() // only difference is highscore
{
    Player::player.pos = reset_pos;
    current_score = 0;
    score_timer = 0;
    highscore = 0;
    World::GenerateWorld();
}
void Restart()
{
    score_timer = 0;
    current_score = 0;
    Player::player.pos = reset_pos;
    World::GenerateWorld();
}
void UpdateGame()
{
    score_timer += GetFrameTime();
    world.texture.Update(world.data);
    manager.UpdateAll();
    Player::Update();

    if(score_timer > MAX_TIME && menu == PLAYING)
    {
        menu = WIN;
    }
}
void DrawGame()
{
    BeginMode2D(camera.GetCamera());
    manager.DrawEntities();
    Player::Draw();
    world.Draw();
    EndMode2D();

}

namespace Player
{
    void Init()
    {
        player_asset.InitAnimatedSprite("assets/texture/character.png");
        player.pos.x = 64;
        player.pos.y = 64;

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
        player_asset.PlayAnim("run");
        gj::Entity& player = manager.player;
        player.type = gj::EntityType::PLAYER;
        player.AddComponentCollision(8, 8);
        player.AddComponentPhysics(vec2(0, 100), 1, 0.01f); //gravity
        camera.zoom = 6;
    }
    void Draw()
    {
        player_asset.Draw((manager.player.pos), 0, {1,1}, false);
    }
    void Input()
    {
        bool is_key_pressed = false;
        camera.zoom += GetMouseWheelMove() * 0.1;
        timer -= GetFrameTime();
        timer2 -= GetFrameTime();
        if(IsKeyDown(KEY_D))
        {
            player_asset.PlayAnim("run2");
            player.vel.x += GetFrameTime() * player_accel;
            is_key_pressed = true;
        }
        if(IsKeyDown(KEY_A))
        {
            player_asset.PlayAnim("run2");
            player.vel.x -= GetFrameTime() * player_accel;
            is_key_pressed = true;
        }
        if(IsKeyDown(KEY_SPACE))
        {
            is_key_pressed = true;
            timer = 0.1;
            player_asset.PlayAnim("idle2");
            player.vel.y = min(-30.0f, player.vel.y);
            player.vel.y -= GetFrameTime() * 120;
            Particle::timer += GetFrameTime();
            if(Particle::timer > 0.01)
            {
                Particle::timer = 0;
                gj::Entity e;
                e.width = 1;
                e.height = 1;
                e.pos = player.pos + vec2(GetRandomValue(2, 6), 10 + GetRandomValue(-2, 2));
                e.vel = vec2(GetRandomValue(-10, 10), 10);
                e.AddComponentLifetime(0.5);
                e.AddComponentVisible();
                e.AddComponentPhysics(vec2(0, 100), 0.01, 1.0f);
                e.color = {255, 255, 255, 255};
                e.OnUpdate = Particle::ParticleUpdate;
                manager.AddEntity(e);
            }
        }
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && timer2 <= 0 || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            timer2 = 0.3;
            gj::Entity e;
            e.AddComponentCollision(2,2);
            e.AddComponentVisible();
            e.AddComponentPhysics(vec2(0, 100), 0.01, 1.0);
            e.color = GOLD;
            Vector2 m = GetWorldToScreen2D({player.pos.x, player.pos.y}, camera.GetCamera());
            vec2 dir = normalize(vec2((float)GetMouseX(), (float)GetMouseY()) - vec2(m.x, m.y));
            e.pos = player.pos + vec2(4,4) + dir * 10;
            e.vel = dir * 100;
            e.OnKill = Seed::Onkill;
            e.OnCollision = Seed::OnCollision;
            e.AddComponentLifetime(1.5);

            manager.AddEntity(e);

        }
        if(!is_key_pressed)
        {
            player_asset.PlayAnim("idle2");
        }

    }
        //Player namespace
    void Update()
    {
        Input();
        camera.target = player.pos;
        camera.Update();
        player_asset.Update();
        if (player.pos.y > 256 )
            player.pos = vec2(64, 0);

    }
}

namespace Particle
{
    void ParticleUpdate(gj::Entity& e, gj::GameManager<MAX_ENTITY>* m)
    {
        //vec4 col = vec4(e.color.r, e.color.g, e.color.b, e.color.a);
        vec4 c1 = vec4(255, 255, 0, 255);
        vec4 c2 = vec4(255, 0, 0, 0);
        vec4 r = mix(c2, c1, e.lifetime_remaining/e.lifetime_total);
        e.color = {(unsigned char)r.x, (unsigned char)r.y, (unsigned char)r.z, (unsigned char)r.w};
    }
}

namespace World
{
    void OnDetection(gj::Entity& self, gj::Entity& other, gj::GameManager<MAX_ENTITY>* m)
    {
        if(self.type == gj::EntityType::BLOCK_WORLD && other.HasComponent(gj::Component::COLLISION))
        {
            int o_x = (int)other.pos.x - 4;
            int o_y = (int)other.pos.y - 4;
            int o_w = (int)other.width + 8;
            int o_h = (int)other.width + 8;

            for(int y = o_y; y < o_y + o_h; y++)
            {
                for(int x = o_x; x < o_x + o_w; x++)
                {
                    gj::Entity e;
                    e.AddComponentCollision(1,1);
                    e.pos = {(float)x, (float)y};
                    Color blk = world.GetBlock(e.pos);
                    if(blk.a != 0 && blk.a != 254) //not air or sky
                    {
                        gj::SolveCollision(other, e, m);
                    }
                }
            }

        }
    }
    void Init()
    {
        GenerateWorld();
        world.InitTexture();
        gj::Entity e;
        e.AddComponentVisible();
        e.width = world.GetWidth();
        e.height = world.GetHeight();
        e.color = {0, 0, 0, 0};
        e.OnDetection = OnDetection;
        e.type = gj::EntityType::BLOCK_WORLD;
        manager.AddEntity(e);
        //e.AddComponentCollision()
    }
    float CanPlace(vec2 p)
    {
        float x = p.x / world.GetWidth();
        float y = p.y / world.GetHeight();
        float r = PI * 0.9;
        float amount = 0.1;
        float result = amount * (sin(sin(sin(x * PI) * r + 32) * r) + 1)/2 + 0.6;
        return result;
    };
    void GenerateWorld()
    {

        generated_score = 0;
        float w = (float)world.GetWidth();
        float h = (float)world.GetHeight();
        auto GenColor = [&](vec2 p) -> Color
        {
            //vec2 uv = p / vec2(w,h) * 2 - vec2(1,1);
            //uv.x /= 2;
            vec4 c1 = {50, 50, 50, 255};
            vec4 c2 = {33, 33, 49, 255};
            float result = CanPlace(p);
            vec4 r = mix(c1, c2, (float)GetRandomValue(0, 10) / 10);
            if(p.y > result * h)
                return Color{(unsigned char)r.x, (unsigned char)r.y, (unsigned char)r.z, 255};
            return Color{0, 0, 0, 0};
        };

        for(int y = 0; y < world.GetHeight(); y++)
            for(int x = 0; x < world.GetWidth(); x++)
            {
                Color c = GenColor({(float)x, (float)y});
                world.SetBlock(ivec2{x, y}, c);
                AddLife(c);
            }
        generated_score = current_score;
        current_score = 0;
    }
}

namespace Seed
{
    void Onkill(gj::Entity& self)
    {
        auto ToColor = [](vec3 c) -> Color
        {
            return {(unsigned char)c.x, (unsigned char)c.y, (unsigned char)c.z, 255};
        };
        auto GenFloat = [](float minimum, float maximum)
        {
            return mix(minimum, maximum, (float)GetRandomValue(0, 1000) / 1000);
        };
        auto GenColor = [](vec3 c1, vec3 c2) -> vec3
        {
            vec3 c = mix(c1, c2, (float)GetRandomValue(0, 100) / 100);
            return c;
        };
        auto RandomCol = [](vec3 c1, vec3 c2) -> vec3
        {
            return GetRandomValue(0, 1)? c1: c2;
        };

        vec3 green1 = vec3(0, 255, 0);
        vec3 green2 = vec3(10, 200, 10);

        vec3 blue1 = vec3(100, 255, 255);
        vec3 blue2 = vec3(0, 255, 255);
        vec3 yellow = vec3(255, 200, 33);
        vec3 white = vec3(255, 255, 255);
        vec3 brown1 = vec3(120, 60, 5);
        vec3 brown2 = vec3(100, 50, 5);


        gj::Entity e;
        e.type = gj::EntityType::EXPLODE_PARTICLE;
        e.AddComponentCollision(2, 2);
        e.AddComponentVisible();
        e.pos = self.pos + vec2(0, 3);
        for(int i = 0; i < 25; i++)
        {
            float v = (float)GetRandomValue(0, 100) / 100;
            e.vel = vec2(cos(v * PI * 2), sin(v * PI * 2)) * 65;
            e.AddComponentLifetime(GenFloat(0.5, 1.5));
            e.AddComponentPhysics(vec2(0, 1), 0.01, 1.0);
            e.color = ToColor(RandomCol(RandomCol(white, yellow), green1));
            int s = GetRandomValue(1, 2);
            e.width = s;
            e.height = s;
            manager.AddEntity(e);
        }

        int size = GetRandomValue(10, 30);
        int x_ = (int)self.pos.x - size/2;
        int y_ = (int)self.pos.y - size/2;


        switch(GetRandomValue(0, 2))
        {
            //Tree generation
            case 0:
            {
                if(world.GetBlock(vec2(x_, y_ + 64)).a == 0)
                {
                    for(; y_ < world.GetHeight(); y_++)
                        if(world.GetBlock(vec2(x_, y_ + 64)).a != 0)
                            break;
                }
                for(int x = 0; x < size; x ++)
                    for(int y = 0; y < size; y ++)
                    {
                        vec2 uv = vec2(x, y) / vec2(size, size) * 2 + vec2(-1, -1);
                        if(length(uv) >= 0.9)
                            continue;
                        vec2 p = vec2(x + x_, y + y_);
                        world.SetBlock(p, ToColor(GenColor(green1, green2)));
                    }
                for(int y = size/2; y < world.GetHeight(); y++)
                {
                    float x = (float)self.pos.x;
                    vec2 p = vec2(x_ + (float)size/2, y + y_);
                    Color blk = world.GetBlock(p);
                    if(blk.a == 0 || blk.a == 254)
                        world.SetBlock(p, ToColor(GenColor({80, 50, 0}, {150, 75, 0})));
                }
                break;
            }

            case 1: // sky
            {
                // size *= 5;
                // for(int x = 0; x < size; x ++)
                //     for(int y = 0; y < size; y ++)
                //     {
                //         vec2 uv = vec2(x, y) / vec2(size, size) * 2 + vec2(-1, -1);
                //         uv.x *= (float)GetRandomValue(5, 10) / 10;
                //         vec2 p = vec2(x + x_, y + y_);
                //         Color c = ToColor(mix(vec3(255, 255, 255), vec3(131, 200, 243), 1.0 - p.y / world.GetHeight() + 0.3));
                //         c.a = 254;
                //         if(length(uv) <= 1.0 && world.GetBlock(p).a == 0)
                //             world.SetBlock(p, Color{c.r, c.g, c.b, 254});
                //     }
                break;
            }

            case 2: //flower
            {
                for(; y_ < world.GetHeight(); y_++)
                {
                    Color blk = world.GetBlock(vec2(x_, y_ + 8));
                    if(blk.a != 0 && blk.a != 254)
                        break;
                }
                vec2 f = vec2((float)x_, (float)y_);
                vec3 col = GenColor(yellow, blue1);
                world.SetBlock(f + vec2(0, 0), ToColor(col));
                world.SetBlock(f + vec2(1, 0), ToColor(col));
                world.SetBlock(f + vec2(-1, 0), ToColor(col));
                world.SetBlock(f + vec2(0, -1), ToColor(col));
                world.SetBlock(f + vec2(0, 1), ToColor(col));
                for(int y = 2; y < world.GetHeight(); y++)
                {
                    float x = (float)self.pos.x;
                    vec2 p = f + vec2(0, y);
                    Color blk = world.GetBlock(p);
                    if(blk.a == 0 || blk.a == 254)
                        world.SetBlock(p, ToColor(GenColor(green1, green2)));
                }
                break;

            }
        }
    }
    bool OnCollision(gj::Entity& self, gj::Entity& other, gj::GameManager<MAX_ENTITY>* m)
    {
        self.Kill();
        auto ToColor = [](vec3 c) -> Color
        {
            return {(unsigned char)c.x, (unsigned char)c.y, (unsigned char)c.z, 255};
        };
        auto GenFloat = [](float minimum, float maximum)
        {
            return mix(minimum, maximum, (float)GetRandomValue(0, 1000) / 1000);
        };
        auto GenColor = [](vec3 c1, vec3 c2) -> vec3
        {
            vec3 c = mix(c1, c2, (float)GetRandomValue(0, 100) / 100);
            return c;
        };
        auto RandomCol = [](vec3 c1, vec3 c2) -> vec3
        {
            return GetRandomValue(0, 1)? c1: c2;
        };

        int size = GetRandomValue(10, 30);
        int x_ = (int)self.pos.x - size/2;
        int y_ = (int)self.pos.y - size/2;

        vec3 green1 = vec3(0, 255, 0);
        vec3 green2 = vec3(10, 200, 10);

        vec3 blue1 = vec3(100, 255, 255);
        vec3 blue2 = vec3(0, 255, 255);
        vec3 yellow = vec3(255, 200, 33);
        vec3 white = vec3(255, 255, 255);
        vec3 brown1 = vec3(120, 60, 5);
        vec3 brown2 = vec3(100, 50, 5);


        int s = size * 2;
        for(int x = 0; x < s; x ++)
            for(int y = 0; y < s; y ++)
            {
                vec2 uv = vec2(x, y) / vec2(s, s) * 2 + vec2(-1, -1);
                uv.x *= (float)GetRandomValue(5, 10) / 10;
                vec2 p = vec2(x + x_, y + y_);
                Color block = world.GetBlock(p);
                vec4 clr = {(float)block.r, (float)block.g, (float)block.b, (float)block.a};
                if(length(uv) >= 0.9 || block.a == 0 || dot(clr, vec4(1,1,1,1)) > 405)
                    continue;
                vec3 color = GenColor(green1, green2);
                float result = World::CanPlace(p);
                if(p.y > result * world.GetHeight() + GenFloat(3, 10))
                    result = 0;
                color = mix(GenColor(brown1, brown2), color, result);
                world.SetBlock(p, ToColor(color));
            }
        return true;
    }
}

void WinMenu(UI::Context* ui)
{

    UI::BoxStyle root =
    {
        .flow =
        {
            .axis = UI::Flow::VERTICAL,
            .vertical_alignment = UI::Flow::CENTERED,
            .horizontal_alignment = UI::Flow::CENTERED,
        },
        .width = {GetScreenWidth()},
        .height = {GetScreenHeight()},
        .background_color = {100, 100, 100, 100},
    };

    UI::BoxStyle base =
    {
        .flow = root.flow,
        .width = {100, UI::Unit::CONTENT_PERCENT},
        .height = {100, UI::Unit::CONTENT_PERCENT},
        .padding = {10,10,10,10},
        .background_color = GREEN1,
        .border_color = BLACK0,
        .corner_radius = 5,
        .border_width = 5,
    };
    UI::BoxStyle button =
    {
        .width = {100, UI::Unit::PARENT_PERCENT},
        .height = {100, UI::Unit::CONTENT_PERCENT},
        .min_width = {100, UI::Unit::CONTENT_PERCENT},
        .padding = {5,5,5,5},
        .margin = {5,5,5,5},
        .background_color = PURPLE0,
        .border_color = BLACK0,
        .border_width = 2,
    };
    bool resize = GetScreenWidth() < 1000 || GetScreenHeight() < 600;
    int size = resize ? FONT_SIZE0: FONT_SIZE1;
    UI::Root(ui, root, [&]
    {
        UI::InsertText(UI::Fmt("[S:%d]Times Up!", FONT_SIZE3));
        float scale = (float)GetScore();
        if(scale <= 50)
            UI::InsertText(UI::Fmt("[S:%d][C:%s]Not Bad!", size, UI::RGBAToHex(GetScoreColor(scale))));
        else if(scale <= 75)
            UI::InsertText(UI::Fmt("[S:%d][C:%s]Good!", size, UI::RGBAToHex(GetScoreColor(scale))));
        else if(scale <= 100)
            UI::InsertText(UI::Fmt("[S:%d][C:%s]EXCELLENT!", size, UI::RGBAToHex(GetScoreColor(scale))));
        else if(scale > 100)
            UI::InsertText(UI::Fmt("[S:%d][C:%s]OUTSTANDING!", size, UI::RGBAToHex(GetScoreColor(scale))));
        UI::InsertText(UI::Fmt("[S:%d]Highscore [C:%s]%d%%", size, UI::RGBAToHex(GetScoreColor(GetHighScore())), GetHighScore()));
        UI::InsertText(UI::Fmt("[S:%d]Score [C:%s]%d%%", size, UI::RGBAToHex(GetScoreColor(GetScore())), GetScore()));

        UI::Box().Style(base).Run([&]
        {
            UI::Box("play-button").Style(button)
            .OnHover([&]
            {
                UI::Style().background_color = GOLD0;
                if(UI::IsMouseReleased(UI::MOUSE_LEFT))
                    menu = FREEPLAY;
            })
            .Run([&]{
                UI::InsertText(UI::Fmt("[S:%d]Resume", size));
            });

            UI::Box("main-menu-button").Style(button)
            .OnHover([&]
            {
                UI::Style().background_color = GOLD0;
                if(UI::IsMouseReleased(UI::MOUSE_LEFT))
                {
                    menu = PAUSE;
                }
            })
            .Run([&]{
                UI::InsertText(UI::Fmt("[S:%d]Main Menu", size));
            });
        });
    });

    UI::Draw();
}
void StartMenu(UI::Context* ui)
{
    UI::BoxStyle root =
    {
        .flow =
        {
            .axis = UI::Flow::VERTICAL,
            .vertical_alignment = UI::Flow::CENTERED,
            .horizontal_alignment = UI::Flow::CENTERED,
        },
        .width = {GetScreenWidth()},
        .height = {GetScreenHeight()},
        .background_color = {100, 100, 100, 100},
    };

    UI::BoxStyle base =
    {
        .flow = root.flow,
        .width = {100, UI::Unit::CONTENT_PERCENT},
        .height = {100, UI::Unit::CONTENT_PERCENT},
        .padding = {10,10,10,10},
        .background_color = GREEN1,
        .border_color = BLACK0,
        .corner_radius = 5,
        .border_width = 5,
    };
    UI::BoxStyle button =
    {
        .width = {100, UI::Unit::PARENT_PERCENT},
        .height = {100, UI::Unit::CONTENT_PERCENT},
        .min_width = {100, UI::Unit::CONTENT_PERCENT},
        .padding = {5,5,5,5},
        .margin = {5,5,5,5},
        .background_color = PURPLE0,
        .border_color = BLACK0,
        .border_width = 2,
    };
    UI::BoxStyle floating =
    {

        .x = 30,
        .y = GetScreenHeight() - 80,
        .width = {100, UI::Unit::CONTENT_PERCENT},
        .detach = UI::Detach::ABSOLUTE
    };

    bool resize = GetScreenWidth() < 1000 || GetScreenHeight() < 600;
    int size = resize? FONT_SIZE0: FONT_SIZE1;
    UI::Root(ui, root, [&]
    {
        UI::Box().Style(floating).Run([&]
        {
            UI::InsertText(UI::Fmt("[S:%d]GROW UP Game Jam\n24 August 2025", size / 2));
        });
        UI::InsertText(UI::Fmt("[S:%d]Grow Rush", FONT_SIZE3));
        UI::InsertText(UI::Fmt("[S:%d]Highscore [C:%s]%d%%", FONT_SIZE1, UI::RGBAToHex(GetScoreColor(GetHighScore())), GetHighScore()));
        UI::Box().Style(base).Run([&]
        {
            UI::Box("play-button").Style(button)
            .OnHover([&]
            {
                UI::Style().background_color = GOLD0;
                if(UI::IsMouseReleased(UI::MOUSE_LEFT))
                {
                    if(score_timer >= MAX_TIME) //
                        menu = FREEPLAY;
                    else
                        menu = PLAYING;
                }
            })
            .Run([&]{
                UI::InsertText(UI::Fmt("[S:%d]Play", size));
            });

            UI::Box("restart-button").Style(button)
            .OnHover([&]
            {
                UI::Style().background_color = GOLD0;
                if(UI::IsMouseReleased(UI::MOUSE_LEFT))
                {
                    Restart();
                    UpdateGame();
                    menu = PLAYING;
                }
            })
            .Run([&]{
                UI::InsertText(UI::Fmt("[S:%d]Restart", size));
            });

            UI::Box("center-button").Style(button)
            .OnHover([&]
            {
                UI::Style().background_color = GOLD0;
                if(UI::IsMouseReleased(UI::MOUSE_LEFT))
                {
                    Player::player.pos = reset_pos;
                    UpdateGame();
                    menu = PLAYING;
                }
            })
            .Run([&]{
                UI::InsertText(UI::Fmt("[S:%d]Center Player", size));
            });

            UI::Box("quit-button").Style(button)
            .OnHover([&]
            {
                UI::Style().background_color = GOLD0;
                if(UI::IsMouseReleased(UI::MOUSE_LEFT))
                    exit(0);
            })
            .Run([&]{
                UI::InsertText(UI::Fmt("[S:%d]Quit", size));
            });
        });
        UI::InsertText(UI::Fmt("[S:%d]a  d  space  left-click  esc", FONT_SIZE0));
    });

    UI::Draw();
}

void PlayMenu(UI::Context* ui)
{
    UI::BoxStyle root =
    {
        .flow =
        {
            .axis = UI::Flow::VERTICAL,
            .vertical_alignment = UI::Flow::START,
            .horizontal_alignment = UI::Flow::CENTERED,
        },
        .width = {GetScreenWidth()},
        .height = {GetScreenHeight()},
        .padding = {20, 20, 20, 20},
    };

    UI::BoxStyle button =
    {
        .width = {80, UI::Unit::PARENT_PERCENT},
        .height = {8},
        .min_width = {100, UI::Unit::CONTENT_PERCENT},
        .background_color = PURPLE0,
        .border_color = BLACK0,
        .border_width = 2,
    };
    int amount = 100 * (score_timer) / MAX_TIME;
    UI::BoxStyle inner=
    {
        .width = {amount, UI::Unit::PARENT_PERCENT},
        .height = {100, UI::Unit::PARENT_PERCENT},
        .background_color = GREEN1,
    };

    UI::BoxStyle text_container =
    {
        .flow = {.axis = UI::Flow::HORIZONTAL, .horizontal_alignment = UI::Flow::SPACE_BETWEEN},
        .width = UI::Unit(80, UI::Unit::PARENT_PERCENT),
    };

    bool resize = GetScreenWidth() < 1000 || GetScreenHeight() < 600;
    int size = resize ? FONT_SIZE0: FONT_SIZE1;
    UI::Root(ui, root, [&]
    {
        UI::Box().Style(button).Run([&]
        {
            UI::Box().Style(inner).Run([&]
            {
            });
        });

        UI::Box().Style(text_container).Run([&]
        {
            UI::InsertText(UI::Fmt("[S:%d]%ds", size, (int)score_timer));
            UI::InsertText(UI::Fmt("[S:%d][C:%s]%d", size, UI::RGBAToHex(GOLD0), (int)GetScore()));
        });
    });

    UI::Draw();
}





