#pragma once

#include <DataStructures/Memory.hpp>
#include "graphics.h"
#include "collisions.h"
#include "raylib/raylib.h"


constexpr uint64_t MAX_ENTITY = 128;

namespace gj
{
    template<uint64_t CAPACITY> class GameManager;
    inline vec2 Reflect(vec2 a, vec2 normal)
    {
        vec2 n = project(-1 * a, normal);
        if(dot(a, normal) > 0)
            return vec2();
        return a + n * 2;
    }


    template<typename T, unsigned int CAPACITY>
    using FixedStack = UI::Internal::FixedStack<T, CAPACITY>;

    template<typename T, unsigned int CAPACITY>
    using FixedQueue = UI::Internal::FixedQueue<T, CAPACITY>;

    enum Component : uint64_t
    {
        VISIBLE,
        TEXTURE,
        PHYSICS,
        COLLISION,
        LIFETIME,
    };

    class AnimatedSprite
    {
        std::unordered_map<std::string, std::vector<Rect>> map;
        float time = 0;
        float fps = 20;
        std::string current_state;
        int GetCurrentFrame();
    public:
        gx::Texture texture;
        AnimatedSprite(const char* file);
        void Update();
        void AddFrame(const std::string& name, Rect src);
        void PlayAnim(const std::string& name);
        void Draw(vec2 pos, float rotation, vec2 scale, bool flip);
    };

    //defining all my entity types
    enum EntityType : uint8_t // NOTE only specific to this game
    {
        DEFAULT,
        PLAYER,
        FLAME_PARTICLE
    };
    struct Entity
    {
        uint64_t id = 0; // must be non 0 for valid entity
        Component component_flags{};

        EntityType type = DEFAULT;


        void (*OnUpdate)(Entity& self, GameManager<MAX_ENTITY>* manager) = nullptr;

        //physics
        vec2 gravity;
        vec2 pos;
        vec2 vel;
        float bounce = 0;
        float mass = 0;


        Entity* other_collide = nullptr;
        //shape
        float width = 0;
        float height = 0;

        //Visual
        Color color;


        //Life
        float lifetime_remaining = 0;
        bool should_kill = false;


        //Add/Remove components
        void AddComponentCollision(float width, float height);
        void AddComponentPhysics(vec2 gravity, float mass, float bounce);
        void AddComponentVisible();
        void AddComponentLifetime(float time); //seconds

        void Kill();
        void SetComponentFlags(Component comp, bool flag);
        bool HasComponent(Component comp) const;

        //Get important values
        Rect GetRect() const;


        void Draw() const;
        void Print() const;
    };

    template<uint64_t CAPACITY>
    class GameManager
    {
        uint64_t total_entities = 0;
        FixedStack<int, CAPACITY> entity_stack{};
        FixedQueue<Entity, 16> queue{};
        Entity entities[CAPACITY]{}; //first index is player
        void AddEntity_(Entity e);
    public:
        Entity& player = entities[0]; //THIS IS AWFUL BUT I NEED TO FINISH AND NOT THINK
        GameManager();

        int Size() const;
        void AddEntity(const Entity& e);
        void DestroyEntity(const Entity& e);

        //Updating entities
        void UpdateAll();
        void UpdateKill();
        void UpdatePhysics();
        void UpdateCollisions();
        void UpdateLifetimes();

        void DrawEntities() const;
    };


    // I SPAMMED THIS :(
    inline void SolveCollision(Entity& a, Entity& b)
    {
        a.other_collide = &b;
        b.other_collide = &a;
        auto FinalVelocity = [&](float v1, float m1, float v2, float m2, float e) -> float
        {
            float total_mass = m1 + m2;
            float result = (m1 - e * m2) * v1 / total_mass + (1 + e) * m2 * v2 / total_mass;
            return result;
        };

        CollideShape(a.GetRect(), b.GetRect(), [&](vec2 normal, float distance)
        {
            float inv_mass = a.mass > 0.0f && b.mass > 0.0f? 1.0/(a.mass + b.mass): 0.0f;

            bool a_phys = a.HasComponent(Component::PHYSICS);
            bool b_phys = b.HasComponent(Component::PHYSICS);
            float a_amount = a_phys? 1.0f: 0.0f;
            float b_amount = b_phys? 1.0f: 0.0f;

            float bounce = max(a.bounce, b.bounce);
            vec2 av = a.vel;
            vec2 bv = b.vel;
            if(a_phys && b_phys)
            {
                a_amount = a.mass * inv_mass;
                b_amount = b.mass * inv_mass;


                if(abs(normal.x) > 0)
                    a.vel.x = FinalVelocity(av.x, a.mass, bv.x, b.mass, bounce);
                else
                    a.vel.y = FinalVelocity(av.y, a.mass, bv.y, b.mass, bounce);

                if(abs(normal.x) > 0)
                    b.vel.x = FinalVelocity(bv.x, b.mass, av.x, a.mass, bounce);
                else
                    b.vel.y = FinalVelocity(bv.y, b.mass, av.y, a.mass, bounce);
            }
            else if(a_phys)
            {
                if(abs(normal.x) > 0)
                    a.vel.x = -av.x * bounce;
                else
                    a.vel.y = -av.y * bounce;
            }
            else if(b_phys)
            {
                if(abs(normal.x) > 0)
                    b.vel.x = -bv.x * bounce;
                else
                    b.vel.y = -bv.y * bounce;
            }

            if(a_phys)
            {
                a_amount += 0.01;
                a.pos += normal * distance * a_amount; //correcting position
            }
            if(b_phys)
            {
                b_amount += 0.01;
                b.pos -= normal * distance * b_amount; //correcting position
            }
        });
    }
}



namespace gj
{

    inline AnimatedSprite::AnimatedSprite(const char* file) : texture(file), current_state("none")
    {

    }
    inline int AnimatedSprite::GetCurrentFrame()
    {
        if(current_state == "none")
            return 0;
        return (int)(time * fps) % map[current_state].size();
    }
    inline void AnimatedSprite::Update()
    {
        time += GetFrameTime();
    }
    inline void AnimatedSprite::AddFrame(const std::string& name, Rect src)
    {
        map[name].push_back(src);
    }
    inline void AnimatedSprite::PlayAnim(const std::string& name)
    {
        current_state = name;
    }
    inline void AnimatedSprite::Draw(vec2 pos, float rotation, vec2 scale, bool flip)
    {
        if(current_state == "none" || !IsTextureValid(texture.GetTexture()))
        {
            DrawRectangle(pos.x, pos.y, 100, 100, PURPLE);
            return;
        }
        Rect src = map[current_state][this->GetCurrentFrame()];
        float width = src.width;
        float height = src.height;
        if(flip)
        {
            Rect result;
            result.x = src.x + src.width;
            result.width = -src.width;
            src = result;
        }
        DrawTexturePro(texture.GetTexture(), {src.x, src.y, src.width, src.height}, {pos.x, pos.y, scale.x * width, scale.y * height}, {0, 0}, 0, WHITE);
    }


    inline void Entity::AddComponentCollision(float width, float height)
    {
        SetComponentFlags(Component::COLLISION, true);
        this->width = width;
        this->height = height;
    }

    inline void Entity::AddComponentPhysics(vec2 gravity, float mass, float bounce)
    {
        SetComponentFlags(Component::PHYSICS, true);
        this->mass = mass > 0? mass: 0.001f;
        this->gravity = gravity;
        this->bounce = bounce;
    }

    inline void Entity::AddComponentVisible()
    {
        SetComponentFlags(Component::VISIBLE, true);
    }

    inline void Entity::AddComponentLifetime(float time) //seconds
    {
        SetComponentFlags(Component::LIFETIME, true);
        lifetime_remaining = time;
    }

    inline void Entity::Kill()
    {
        should_kill = true;
    }
    inline void Entity::SetComponentFlags(Component comp, bool enable)
    {
        uint64_t result = (uint64_t)component_flags;
        uint64_t flag = (uint64_t)1 << (uint64_t)comp;
        result = enable? result | flag: result & flag;
        component_flags = (Component)result;
    }
    inline bool Entity::HasComponent(Component comp) const
    {
        uint64_t flag = 1 << (uint64_t)comp;
        uint64_t flags = (uint64_t)component_flags;
        return flags & flag;
    }

    inline Rect Entity::GetRect() const
    {
        return Rect{pos.x, pos.y, width, height};
    }
    inline void Entity::Draw() const
    {

    }
    inline void Entity::Print() const
    {
        std::cout<<
        "id: " << id <<
        "\nflags: " << component_flags <<
        "\npos: " << pos <<
        "\nvel: " << vel <<
        "\nmass: " << mass <<
        "\nwidth: " << width <<
        "\nheight: " << height <<
        '\n';
    }


    template<uint64_t CAPACITY>
    inline GameManager<CAPACITY>::GameManager()
    {
        for(int i = 1; i < CAPACITY; i++)
        {
            entity_stack.Push(i);
        }
    }

    template<uint64_t CAPACITY>
    inline int GameManager<CAPACITY>::Size() const
    {
        return CAPACITY - entity_stack.Size();
    }
    template<uint64_t CAPACITY>
    inline void GameManager<CAPACITY>::AddEntity(const Entity& e)
    {
        if(!queue.IsFull())
            queue.Push(e);
    }

    template<uint64_t CAPACITY>
    inline void GameManager<CAPACITY>::AddEntity_(Entity e)
    {
        if(!entity_stack.IsEmpty())
        {
            assert(entity_stack.Size() != 0 && "You messed up");
            int i = entity_stack.Peek();
            entity_stack.Pop();
            e.id = ++total_entities;
            entities[i] = e;
            std::cout<<"Entity Created " << Size() <<'/' << CAPACITY<<"\n";
            return;
        }
        //assert(0 && "Entity Manager Out of memory");
    }

    template<uint64_t CAPACITY>
    inline void GameManager<CAPACITY>::DestroyEntity(const Entity& e)
    {
        this->DestroyEntity(e.id);
    }

    template<uint64_t CAPACITY>
    inline void GameManager<CAPACITY>::UpdateAll()
    {
        while(!queue.IsEmpty())
        {
            AddEntity_(queue.Front());
            queue.Pop();
        }
        for(Entity& e : entities)
        {
            if(e.OnUpdate)
                e.OnUpdate(e, this);
            e.other_collide = nullptr;
        }
        UpdateKill();
        UpdateLifetimes();
        UpdatePhysics();
        UpdateCollisions();
    }
    template<uint64_t CAPACITY>
    inline void GameManager<CAPACITY>::UpdateKill()
    {
        if(player.should_kill)
            player = Entity();
        for(int i = 1; i < CAPACITY; i++)
        {
            if(entities[i].should_kill == true)
            {
                assert(!entity_stack.IsFull() && "You messed up the killing");
                entity_stack.Push(i);
                entities[i] = Entity{};
                std::cout<<"Entity Killed " << Size() <<'/' << CAPACITY<<"\n";
            }
        }
    }
    template<uint64_t CAPACITY>
    inline void GameManager<CAPACITY>::UpdatePhysics()
    {
        for(Entity& e: entities)
        {
            if(e.HasComponent(Component::PHYSICS))
            {
                e.vel += e.gravity * GetFrameTime();
                e.pos += e.vel * GetFrameTime();


                float speed = length(e.vel);
                e.vel.x -= 0.001f * e.vel.x * abs(e.vel.x) * GetFrameTime() * 2;
                e.vel.x -= sign(e.vel.x) * GetFrameTime() * 50;

            }
        }
    }
    template<uint64_t CAPACITY>
    inline void GameManager<CAPACITY>::UpdateCollisions()
    {
        for(Entity& a : entities)
        {
            if(!a.HasComponent(Component::COLLISION))
                continue;
            for(Entity& b : entities)
            {
                if(!b.HasComponent(Component::COLLISION) || a.id == b.id)
                    continue;
                SolveCollision(a, b);
            }
        }

    }
    template<uint64_t CAPACITY>
    inline void GameManager<CAPACITY>::UpdateLifetimes()
    {
        for(Entity& e: entities)
        {
            if(e.HasComponent(Component::LIFETIME))
            {
                e.lifetime_remaining -= GetFrameTime();
                if(e.lifetime_remaining <= 0)
                    e.Kill();
            }
        }
    }

    template<uint64_t CAPACITY>
    inline void GameManager<CAPACITY>::DrawEntities() const
    {
        for(const Entity& e : entities)
        {
            if(!e.HasComponent(gj::VISIBLE))
                continue;
            DrawRectangle(e.pos.x, e.pos.y, e.width, e.height, e.color);
        }
    }

}
