#pragma once

#include <DataStructures/Memory.hpp>
#include "graphics.h"
#include "collisions.h"



namespace gj
{
    inline vec2 Reflect(vec2 a, vec2 normal)
    {
        vec2 n = project(-1 * a, normal);
        if(dot(a, normal) > 0)
            return vec2();
        return a + n * 2;
    }


    template<typename T, unsigned int CAPACITY>
    using FixedStack = UI::Internal::FixedStack<T, CAPACITY>;

    enum Component : uint64_t
    {
        VISIBLE,
        PHYSICS,
        COLLISION,
        LIFETIME,
    };

    struct Entity
    {
        //physics
        uint64_t id = 0; // must be non 0 for valid entity
        Component component_flags{};
        vec2 gravity;
        vec2 pos;
        vec2 vel;
        float mass = 0;

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
        void AddComponentPhysics(vec2 gravity, float mass);
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
    class EntityManager
    {
        uint64_t total_entities = 0;
        FixedStack<int, CAPACITY> entity_stack{};
        Entity entities[CAPACITY]{};
    public:
        EntityManager();
        int Size() const;
        void AddEntity(Entity e);
        void DestroyEntity(const Entity& e);

        //Updating entities
        void UpdateAll();
        void UpdateKill();
        void UpdatePhysics();
        void UpdateCollisions();
        void UpdateLifetimes();

        void DrawEntities() const;
    };


    inline void SolveCollision(Entity& a, Entity& b)
    {
        auto FinalVelocity = [&](float v1, float m1, float v2, float m2, float e) -> float
        {
            float total_mass = m1 + m2;
            float result = (m1 - e * m2) * v1 / total_mass + (1 + e) * m2 * v2 / total_mass;
            return abs(result);
        };


        CollideShape(a.GetRect(), b.GetRect(), [&](vec2 normal, float distance)
        {
            float inv_mass = a.mass > 0.0f && b.mass > 0.0f? 1.0/(a.mass + b.mass): 0.0f;

            bool a_phys = a.HasComponent(Component::PHYSICS);
            bool b_phys = b.HasComponent(Component::PHYSICS);
            float a_amount = a_phys? 1.0f: 0.0f;
            float b_amount = b_phys? 1.0f: 0.0f;

            if(a_phys && b_phys)
            {
                a_amount = a.mass * inv_mass;
                b_amount = b.mass * inv_mass;
            }

            float a_speed = length(a.vel);
            float b_speed = length(b.vel);
            if(a_phys)
            {
                a_amount += 0.5;
                a.pos += normal * distance * a_amount;
                vec2 n = project(-1 * a.vel, normal);
                vec2 result = a.vel + n * 2;
                //result = normalize(result) * FinalVelocity(a_speed, a.mass, b_speed, b.mass, 1.0);
                a.vel = result;
            }
            if(b_phys)
            {
                b_amount += 0.5;
                b.pos -= normal * distance * b_amount;
                vec2 n = project(-1 * b.vel, normal);
                vec2 result = b.vel + n * 2;
                //result = normalize(result) * FinalVelocity(b_speed, b.mass, a_speed, a.mass, 1.0);
                b.vel = result;
            }

        });
    }
}



namespace gj
{
    inline void Entity::AddComponentCollision(float width, float height)
    {
        SetComponentFlags(Component::COLLISION, true);
        this->width = width;
        this->height = height;
    }

    inline void Entity::AddComponentPhysics(vec2 gravity, float mass)
    {
        SetComponentFlags(Component::PHYSICS, true);
        this->mass = mass > 0? mass: 0.001f;
        this->gravity = gravity;
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
    inline EntityManager<CAPACITY>::EntityManager()
    {
        for(int i = 0; i < CAPACITY; i++)
        {
            entity_stack.Push(i);
        }
    }
    template<uint64_t CAPACITY>
    inline int EntityManager<CAPACITY>::Size() const
    {
        return CAPACITY - entity_stack.Size();
    }
    template<uint64_t CAPACITY>
    inline void EntityManager<CAPACITY>::AddEntity(Entity e)
    {
        if(!entity_stack.IsEmpty())
        {
            assert(entity_stack.Size() != 0 && "You messed up");
            int i = entity_stack.Peek();
            entity_stack.Pop();
            e.id = ++total_entities;
            entities[i] = e;
            std::cout<<"Entity Created id:"<< e.id << Size() <<'/' << CAPACITY<<"\n";
            return;
        }
        assert(0 && "Entity Manager Out of memory");
    }

    template<uint64_t CAPACITY>
    inline void EntityManager<CAPACITY>::DestroyEntity(const Entity& e)
    {
        this->DestroyEntity(e.id);
    }

    template<uint64_t CAPACITY>
    inline void EntityManager<CAPACITY>::UpdateAll()
    {
        UpdateKill();
        UpdateLifetimes();
        UpdatePhysics();
        UpdateCollisions();
    }
    template<uint64_t CAPACITY>
    inline void EntityManager<CAPACITY>::UpdateKill()
    {
        for(int i = 0; i < CAPACITY; i++)
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
    inline void EntityManager<CAPACITY>::UpdatePhysics()
    {
        for(Entity& e: entities)
        {
            if(e.HasComponent(Component::PHYSICS))
            {
                e.vel += e.gravity * GetFrameTime();
                e.pos += e.vel * GetFrameTime();
            }
        }
    }
    template<uint64_t CAPACITY>
    inline void EntityManager<CAPACITY>::UpdateCollisions()
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
    inline void EntityManager<CAPACITY>::UpdateLifetimes()
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
    inline void EntityManager<CAPACITY>::DrawEntities() const
    {
        for(const Entity& e : entities)
        {
            if(!e.HasComponent(gj::VISIBLE))
                continue;
            DrawRectangle(e.pos.x, e.pos.y, e.width, e.height, e.color);
        }
    }

}
