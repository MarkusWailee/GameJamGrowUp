#pragma once
#include <math.h>
#include <cstdint>
#include <iostream>


#ifndef PI
    constexpr float PI = 3.14159265358979323846f;
#endif

//VECTOR 2
template<typename T>
struct vector2;
template<typename T>
struct vector3;
template<typename T>
struct vector4;



using bvec2 = vector2<bool>;
using bvec3 = vector3<bool>;
using bvec4 = vector4<bool>;

using vec2 = vector2<float>;
using vec3 = vector3<float>;
using vec4 = vector4<float>;

using ivec2 = vector2<int>;
using ivec3 = vector3<int>;
using ivec4 = vector4<int>;

using uvec2 = vector2<unsigned int>;
using uvec3 = vector3<unsigned int>;
using uvec4 = vector4<unsigned int>;



template<typename T>
struct vector2
{
	T x{};
	T y{};

	vector2& operator*=(const T s);
	vector2& operator*=(const vector2<T>& v);
	vector2& operator/=(const T s);
	vector2& operator/=(const vector2<T>& b);
	vector2& operator+=(const vector2<T>& v);
	vector2& operator+=(const T s);
	vector2& operator-=(const vector2<T>& v);
	vector2& operator-=(const T s);

	T& operator[](const int index);
};

template<typename T>
struct vector3
{
    T x{}, y{}, z{};

	vector3& operator*=(const T s);
	vector3& operator*=(const vector3<T>& v);
	vector3& operator/=(const T s);
	vector3& operator/=(const vector3<T>& v);
	vector3& operator+=(const vector3<T>& v);
	vector3& operator+=(const T s);
	vector3& operator-=(const vector3<T>& v);
	vector3& operator-=(const T s);

	T& operator[](const int index);
};

template<typename T>
struct vector4
{
    T x{}, y{}, z{}, w{};
	vector4& operator*=(const T s);
	vector4& operator*=(const vector4<T>& v);
	vector4& operator/=(const T s);
	vector4& operator/=(const vector4<T>& v);
	vector4& operator+=(const vector4<T>& v);
	vector4& operator+=(const T s);
	vector4& operator-=(const vector4<T>& v);
	vector4& operator-=(const T s);

	T& operator[](const int index);
};


template<typename T>
inline vector2<T> operator*(const vector2<T>& v1, const vector2<T>& v2);
template<typename T, typename K>
inline vector2<T> operator*(const vector2<T>& v1, const K s);
template<typename T, typename K>
inline vector2<T> operator*(const K s, const vector2<T>& v1);
template<typename T>
inline vector2<T> operator/(const vector2<T>& v1, const vector2<T>& v2);
template<typename T, typename K>
inline vector2<T> operator/(const vector2<T>& v1, const K s);
template<typename T>
inline vector2<T> operator+(const vector2<T>& v1, const vector2<T>& v2);
template<typename T>
inline vector2<T> operator-(const vector2<T>& v1, const vector2<T>& v2);


template<typename T>
inline vector3<T> operator*(const vector3<T>& v1, const vector3<T>& v2);
template<typename T, typename K>
inline vector3<T> operator*(const vector3<T>& v1, const K s);
template<typename T, typename K>
inline vector3<T> operator*(const K s, const vector3<T>& v1);
template<typename T>
inline vector3<T> operator/(const vector3<T>& v1, const vector3<T>& v2);
template<typename T, typename K>
inline vector3<T> operator/(const vector3<T>& v1, const K s);
template<typename T>
inline vector3<T> operator+(const vector3<T>& v1, const vector3<T>& v2);
template<typename T>
inline vector3<T> operator-(const vector3<T>& v1, const vector3<T>& v2);


template<typename T>
inline vector4<T> operator*(const vector4<T>& v1, const vector4<T>& v2);
template<typename T, typename K>
inline vector4<T> operator*(const vector4<T>& v1, const K s);
template<typename T, typename K>
inline vector4<T> operator*(const K s, const vector4<T>& v1);
template<typename T>
inline vector4<T> operator/(const vector4<T>& v1, const vector4<T>& v2);
template<typename T, typename K>
inline vector4<T> operator/(const vector4<T>& v1, const K s);
template<typename T>
inline vector4<T> operator+(const vector4<T>& v1, const vector4<T>& v2);
template<typename T>
inline vector4<T> operator-(const vector4<T>& v1, const vector4<T>& v2);


template<typename T>
inline vector2<T> operator*(const vector2<T>& v1, const vector2<T>& v2)
{
    return
    {
        v1.x * v2.x,
        v1.y * v2.y
    };
}
template<typename T, typename K>
inline vector2<T> operator*(const vector2<T>& v1, const K s)
{
    return
    {
        v1.x * (T)s,
        v1.y * (T)s
    };
}
template<typename T, typename K>
inline vector2<T> operator*(const K s, const vector2<T>& v1)
{
    return
    {
        (T)s * v1.x,
        (T)s * v1.y
    };
}
template<typename T>
inline vector2<T> operator/(const vector2<T>& v1, const vector2<T>& v2)
{
    return
    {
        v1.x / v2.x,
        v1.y / v2.y
    };
}
template<typename T, typename K>
inline vector2<T> operator/(const vector2<T>& v1, const K s)
{
    return
    {
        v1.x / (T)s,
        v1.y / (T)s
    };
}
template<typename T>
inline vector2<T> operator+(const vector2<T>& v1, const vector2<T>& v2)
{
    return
    {
        v1.x + v2.x,
        v1.y + v2.y
    };
}
template<typename T>
inline vector2<T> operator-(const vector2<T>& v1, const vector2<T>& v2)
{
    return
    {
        v1.x - v2.x,
        v1.y - v2.y
    };
}


template<typename T>
inline vector3<T> operator*(const vector3<T>& v1, const vector3<T>& v2)
{
    return
    {
        v1.x * v2.x,
        v1.y * v2.y,
        v1.z * v2.z
    };
}
template<typename T, typename K>
inline vector3<T> operator*(const vector3<T>& v1, const K s)
{
    return
    {
        v1.x * (T)s,
        v1.y * (T)s,
        v1.z * (T)s
    };
}
template<typename T, typename K>
inline vector3<T> operator*(const K s, const vector3<T>& v1)
{
    return
    {
        (T)s * v1.x,
        (T)s * v1.y,
        (T)s * v1.z
    };
}
template<typename T>
inline vector3<T> operator/(const vector3<T>& v1, const vector3<T>& v2)
{
    return
    {
        v1.x / v2.x,
        v1.y / v2.y,
        v1.z / v2.z
    };
}
template<typename T, typename K>
inline vector3<T> operator/(const vector3<T>& v1, const K s)
{
    return
    {
        v1.x / (T)s,
        v1.y / (T)s,
        v1.z / (T)s
    };
}
template<typename T>
inline vector3<T> operator+(const vector3<T>& v1, const vector3<T>& v2)
{
    return
    {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z
    };
}
template<typename T>
inline vector3<T> operator-(const vector3<T>& v1, const vector3<T>& v2)
{
    return
    {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z
    };
}


template<typename T>
inline vector4<T> operator*(const vector4<T>& v1, const vector4<T>& v2)
{
    return
    {
        v1.x * v2.x,
        v1.y * v2.y,
        v1.z * v2.z,
        v1.w * v2.w
    };
}
template<typename T, typename K>
inline vector4<T> operator*(const vector4<T>& v1, const K s)
{
    return
    {
        v1.x * (T)s,
        v1.y * (T)s,
        v1.z * (T)s,
        v1.w * (T)s
    };
}
template<typename T, typename K>
inline vector4<T> operator*(const K s, const vector4<T>& v1)
{
    return
    {
        (T)s * v1.x,
        (T)s * v1.y,
        (T)s * v1.z,
        (T)s * v1.w
    };
}
template<typename T>
inline vector4<T> operator/(const vector4<T>& v1, const vector4<T>& v2)
{
    return
    {
        v1.x / v2.x,
        v1.y / v2.y,
        v1.z / v2.z,
        v1.w / v2.w
    };
}
template<typename T, typename K>
inline vector4<T> operator/(const vector4<T>& v1, const K s)
{
    return
    {
        v1.x / (T)s,
        v1.y / (T)s,
        v1.z / (T)s,
        v1.w / (T)s
    };
}
template<typename T>
inline vector4<T> operator+(const vector4<T>& v1, const vector4<T>& v2)
{
    return
    {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
        v1.w + v2.w
    };
}
template<typename T>
inline vector4<T> operator-(const vector4<T>& v1, const vector4<T>& v2)
{
    return
    {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
        v1.w - v2.w
    };
}




//VECTOR 2
template<typename T>
inline vector2<T>& vector2<T>::operator*=(const T s)
{
	x *= s;
	y *= s;
    return *this;
}
template<typename T>
inline vector2<T>& vector2<T>::operator*=(const vector2<T>& v)
{
	x *= v.x;
	y *= v.y;
    return *this;
}
template<typename T>
inline vector2<T>& vector2<T>::operator/=(const T s)
{
	x /= s;
	y /= s;
    return *this;
}
template<typename T>
inline vector2<T>& vector2<T>::operator/=(const vector2<T>& v)
{
	x /= v.x;
	y /= v.y;
    return *this;
}
template<typename T>
inline vector2<T>& vector2<T>::operator+=(const vector2<T>& v)
{
	x += v.x;
	y += v.y;
    return *this;
}
template<typename T>
inline vector2<T>& vector2<T>::operator+=(const T s)
{
	x += s;
	y += s;
    return *this;
}
template<typename T>
inline vector2<T>& vector2<T>::operator-=(const vector2<T>& v)
{
	x -= v.x;
	y -= v.y;
    return *this;
}
template<typename T>
inline vector2<T>& vector2<T>::operator-=(const T s)
{
	x -= s;
	y -= s;
    return *this;
}
template<typename T>
inline T& vector2<T>::operator[](const int index)
{
	return(&x)[index];
}


//VECTOR 3
template<typename T>
inline vector3<T>& vector3<T>::operator*=(const T s)
{
	x *= s;
	y *= s;
	z *= s;
    return *this;
}
template<typename T>
inline vector3<T>& vector3<T>::operator*=(const vector3<T>& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
    return *this;
}
template<typename T>
inline vector3<T>& vector3<T>::operator/=(const T s)
{
	x /= s;
	y /= s;
	z /= s;
    return *this;
}
template<typename T>
inline vector3<T>& vector3<T>::operator/=(const vector3<T>& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
    return *this;
}
template<typename T>
inline vector3<T>& vector3<T>::operator+=(const vector3<T>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
    return *this;
}
template<typename T>
inline vector3<T>& vector3<T>::operator+=(const T s)
{
	x += s;
	y += s;
	z += s;
    return *this;
}
template<typename T>
inline vector3<T>& vector3<T>::operator-=(const vector3<T>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
    return *this;
}
template<typename T>
inline vector3<T>& vector3<T>::operator-=(const T s)
{
	x -= s;
	y -= s;
	z -= s;
    return *this;
}
template<typename T>
inline T& vector3<T>::operator[](const int index)
{
	return(&x)[index];
}


//VECTOR 4
template<typename T>
inline vector4<T>& vector4<T>::operator*=(const T s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}
template<typename T>
inline vector4<T>& vector4<T>::operator*=(const vector4<T>& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
    return *this;
}
template<typename T>
inline vector4<T>& vector4<T>::operator/=(const T s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
    return *this;
}
template<typename T>
inline vector4<T>& vector4<T>::operator/=(const vector4<T>& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
    return *this;
}
template<typename T>
inline vector4<T>& vector4<T>::operator+=(const vector4<T>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
    return *this;
}
template<typename T>
inline vector4<T>& vector4<T>::operator+=(const T s)
{
	x += s;
	y += s;
	z += s;
	w += s;
    return *this;
}
template<typename T>
inline vector4<T>& vector4<T>::operator-=(const vector4<T>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
    return *this;
}
template<typename T>
inline vector4<T>& vector4<T>::operator-=(const T s)
{
	x -= s;
	y -= s;
	z -= s;
	w -= s;
    return *this;
}
template<typename T>
inline T& vector4<T>::operator[](const int index)
{
	return(&x)[index];
}







//Other Operations
template<typename T>
inline T mix(T a, T b, float amount) { return a + amount * (b - a); }
template <typename T>
inline T min(T a, T b){return a < b? a: b;}
template <typename T>
inline T max(T a, T b){return a > b? a: b;}
template<typename T>
inline T clamp(T value, T minimum, T maximum) { return min(max(value, minimum), maximum); }
template<typename T>
inline float length(const vector2<T>& v) { return std::hypot(v.x, v.y); }
template<typename T>
inline float length(const vector3<T>& v) { return std::hypot(std::hypot(v.x, v.y), v.z); }
template<typename T>
inline float length(const vector4<T>& v) { return std::hypot(std::hypot(std::hypot(v.x, v.y), v.z), v.w); }

template<typename T>
inline T dot(const vector2<T>& L, const vector2<T>& R) { return L.x * R.x + L.y * R.y; }
template<typename T>
inline T dot(const vector3<T>& L, const vector3<T>& R) { return L.x * R.x + L.y * R.y + L.z * R.z; }
template<typename T>
inline T dot(const vector4<T>& L, const vector4<T>& R) { return L.x * R.x + L.y * R.y + L.z * R.z + L.w * R.w; }
template<typename T>
inline vector3<T> cross(const vector3<T>& a, const vector3<T>& b)
{
	return
    {
		a.y * b.z - b.y * a.z,
		b.x * a.z - a.x * b.z,
		a.x * b.y - b.x * a.y
    };
}

inline vector2<float> normalize(const vector2<float>& v)
{
    float d = length(v);
    if(d > 0)
    {
        float d_inv = 1.0f/d;
        return v * d_inv;
    }
    return vector2<float>{};
}
inline vector3<float> normalize(const vector3<float>& v)
{
    float d = length(v);
    if(d > 0)
    {
        float d_inv = 1.0f/d;
        return v * d_inv;
    }
    return vector3<float>{};
}
inline vector4<float> normalize(const vector4<float>& v)
{
    float d = length(v);
    if(d > 0)
    {
        float d_inv = 1.0f/d;
        return v * d_inv;
    }
    return vector4<float>{};
}
inline vector2<float> floor(const vector2<float>& v) {return vector2<float>(floorf(v.x), floorf(v.y));}
inline vector3<float> floor(const vector3<float>& v) {return vector3<float>(floorf(v.x), floorf(v.y), floorf(v.z));}
inline vector4<float> floor(const vector4<float>& v) {return vector4<float>(floorf(v.x), floorf(v.y), floorf(v.z), floorf(v.w));}


//NONMEMBER
template<typename T>
inline std::ostream& operator<<(std::ostream& os, const vector2<T>& v) {
	os << '<' << v.x << ", " << v.y << '>';
	return os;
}
template<typename T>
inline std::ostream& operator<<(std::ostream& os, const vector3<T>& v) {
	os << '<' << v.x << ", " << v.y << ", " << v.z << '>';
	return os;
}
template<typename T>
inline std::ostream& operator<<(std::ostream& os, const vector4<T>& v) {
	os << '<' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << '>';
	return os;
}
