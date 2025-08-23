#pragma once
#include <math.h>



template<typename T>
struct rectangle
{

    static bool Overlap(const rectangle& r1, const rectangle& r2);
    static bool Contains(const rectangle& r ,int x, int y);
    static rectangle Intersection(const rectangle& r1, const rectangle& r2);

    T x{};
    T y{};
    T width{};
    T height{};
};

template<typename T>
inline bool rectangle<T>::Overlap(const rectangle& r1, const rectangle& r2)
{
    return (r1.x < r2.x + r2.width && r1.x + r1.width > r2.x &&
            r1.y < r2.y + r2.height && r1.y + r1.height > r2.y);
}

template<typename T>
inline bool rectangle<T>::Contains(const rectangle& r ,int x, int y)
{
    return (x >= r.x && x <= r.x + r.width &&
            y >= r.y && y <= r.y + r.height);
}

template<typename T>
inline rectangle<T> rectangle<T>::Intersection(const rectangle& r1, const rectangle& r2)
{
    rectangle r;
    if(Overlap(r1, r2))
    {
        r.x = max(r1.x, r2.x);
        r.y = max(r1.y, r2.y);
        int outer_x = min(r1.x + r1.width, r2.x + r2.width);
        int outer_y = min(r1.y + r1.height, r2.y + r2.height);
        r.width = abs(outer_x - r.x);
        r.height = abs(outer_y  - r.y);
    }
    return r;
}
