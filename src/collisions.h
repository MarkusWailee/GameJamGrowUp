#pragma once

#include <math/vec.h>
#include <math/rect.h>


namespace gj
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
        float d[4]{};
        d[0] = a.x + a.width - b.x;  //left
        d[1] = b.x + b.width - a.x;  //right
        d[2] = b.y + b.height - a.y;  //top
        d[3] = a.y + a.height - b.y;  //bottom

        int smallest = 0;
        for(int i = 0; i < 4; i++)
        {
            if(d[i] < d[smallest])
                smallest = i;
        }
        distance = d[smallest];
        switch(smallest)
        {
            case 0: callback(vec2(-1, 0), distance); break; //left
            case 1: callback(vec2(1, 0), distance); break; //left
            case 2: callback(vec2(0, 1), distance); break; //top
            case 3: callback(vec2(0, -1), distance); break; //bottom
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
