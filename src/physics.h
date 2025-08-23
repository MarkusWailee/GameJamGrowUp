#pragma once

#include <math/vec.h>
#include <math/rect.h>


namespace px
{
    //Shapes
    struct Circle
    {
        float x;
        float y;
        float r;
    };
    using Rect = rectangle<float>;


    template<typename Func>
    inline void CollideShape(Rect a, Rect b, Func callback)
    {
        if(!Rect::Overlap(a, b))
            return;
        vec2 normal;
        float distance = 0;

        vec2 a_mid = vec2(a.x, a.y) + vec2(a.width, a.height) * 0.5f;
        vec2 b_mid = vec2(b.x, b.y) + vec2(b.width, b.height) * 0.5f;
        vec2 axis = b_mid - a_mid;
        if(abs(axis.x) > abs(axis.y)) //should move on x axis
        {
            normal.x = axis.x > 0? -1.0f : 1.0f;
            distance = axis.x > 0? a.x + a.width - b.x: b.x + b.width - a.x;
            callback(normal, distance);
        }
        else // should move on y axis
        {
            normal.y = axis.y > 0? -1.0f : 1.0f;
            distance = axis.y > 0? a.y + a.height - b.y: b.y + b.height - a.y;
            callback(normal, distance);
        }
    }

    template<typename Func>
    inline void CollideShape(Circle a, Circle b, Func callback)
    {
        vec2 normal = vec2(a.x, a.y) - vec2(b.x, b.y);
        float distance = a.r + b.r - length(normal);
        if(distance > 0)
        {
            callback(normalize(normal), distance);
        }
    }

}
