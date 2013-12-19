#ifndef _VECTOR3_HPP_
#define _VECTOR3_HPP_


//vector3.hpp

#include <cstdio>
#include <cassert>
#include <cmath>


namespace hoshi
{


template<typename T = float>
class Vector3Base
{
public:
    typedef T type;
    typedef const T const_type;

    typedef Vector3Base<type> this_type;
    typedef const Vector3Base<type> const_this_type;


public:
    type x, y, z;


public:
    Vector3Base()
        : x(static_cast<type>(0)), y(static_cast<type>(0)), z(static_cast<type>(0))
    {
    }


    Vector3Base(const_type _x, const_type _y, const_type _z)
        : x(_x), y(_y), z(_z)
    {
    }


    Vector3Base(const_this_type& rhs)
        : x(rhs.x), y(rhs.y), z(rhs.z)
    {
    }


    this_type& operator=(const_this_type& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;

        return *this;
    }


public:
    bool operator==(const_this_type& rhs) const
    {
        return ( x == rhs.x && y == rhs.y && z == rhs.z );
    }


    bool operator!=(const_this_type& rhs) const
    {
        return !(*this == rhs);
    }


public:
    void clear()
    {
        x = static_cast<type>(0);
        y = static_cast<type>(0);
        z = static_cast<type>(0);
    }


    this_type operator-() const
    {
        return this_type(-x, -y, -z);
    }


    this_type operator+(const_this_type& rhs) const
    {
        return this_type(x + rhs.x, y + rhs.y, z + rhs.z);
    }


    this_type operator+(const_type rhs) const
    {
        return this_type(x + rhs, y + rhs, z + rhs);
    }


    this_type operator-(const_this_type& rhs) const
    {
        return this_type(x - rhs.x, y - rhs.y, z - rhs.z);
    }


    this_type operator-(const_type rhs) const
    {
        return this_type(x - rhs, y - rhs, z - rhs);
    }


    this_type operator*(const_this_type& rhs) const
    {
        return this_type(x * rhs.x, y * rhs.y, z * rhs.z);
    }


    this_type operator*(const_type rhs) const
    {
        return this_type(x * rhs, y * rhs, z * rhs);
    }


    this_type operator/(const_this_type& rhs) const
    {
        assert( ( rhs.x != static_cast<type>(0) ) && ( rhs.y != static_cast<type>(0) ) && ( rhs.z != static_cast<type>(0) ) );
        return this_type(x / rhs.x, y / rhs.y, z / rhs.z);
    }


    this_type operator/(const_type rhs) const
    {
        assert(rhs != static_cast<type>(0));
        return this_type(x / rhs, y / rhs, z / rhs);
    }


    this_type& operator+=(const_this_type& rhs) 
    {
        *this = *this + rhs;
        return *this;
    }


    this_type& operator-=(const_this_type& rhs) 
    {
        *this = *this - rhs;
        return *this;
    }


    this_type& operator*=(const_type rhs)
    {
        *this = *this * rhs;
        return *this;
    }


    this_type& operator/=(const_type rhs)
    {
        *this = *this / rhs;
        return *this;
    }


public:
    void normalize()
    {
        type n = this->norm();

        assert(n != static_cast<type>(0));

        type tmp = static_cast<type>(1) / n;
        x *= tmp;
        y *= tmp;
        z *= tmp;
    }


    this_type normalized() const
    {
        this_type tmp(*this);
        tmp.normalize();

        return this_type(tmp);
    }


    type norm() const
    {
        return static_cast<type>(std::sqrt(this->snorm()));
    }


    type snorm() const
    {
        return ( x*x + y*y + z*z );
    }


    // 内積
    type dot(const_this_type& rhs) const
    {
        return ( x*rhs.x + y*rhs.y + z*rhs.z );
    }


    // 外積
    this_type cross(const_this_type& rhs) const
    {
        return this_type(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x);
    }


    type distance(const_this_type& rhs) const
    {
        type dx = x - rhs.x;
        type dy = y - rhs.y;
        type dz = z - rhs.z;

        return std::sqrt( dx*dx + dy*dy + dz*dz );
    }


public:
    static type dot(const_this_type& lhs, const_this_type& rhs)
    {
        return lhs.dot(rhs);
    }


    static this_type cross(const_this_type& lhs, const_this_type& rhs)
    {
        return lhs.cross(rhs);
    }


    static type distance(const_this_type& lhs, const_this_type& rhs)
    {
        return lhs.distance(rhs);
    }


public:
    void print()
    {
        std::printf("x=%f, y=%f, z=%f\n", x, y, z);
    }
};


// v2 = 3 + v1とか
//  -- lhsをTにするのはどうやるの？
template<typename T>
//inline Vector3Base<T> operator+(const T lhs, const Vector3Base<T>& rhs)
inline Vector3Base<T> operator+(const long double lhs, const Vector3Base<T>& rhs)
{
    return Vector3Base<T>(rhs.x + lhs, rhs.y + lhs, rhs.z + lhs);
}


template<typename T>
inline Vector3Base<T> operator-(const long double lhs, const Vector3Base<T>& rhs)
{
    return Vector3Base<T>(rhs.x - lhs, rhs.y - lhs, rhs.z - lhs);
}


template<typename T>
inline Vector3Base<T> operator*(const long double lhs, const Vector3Base<T>& rhs)
{
    return Vector3Base<T>(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
}



typedef Vector3Base<float> Vector3;
typedef Vector3Base<float> Vector3f;
typedef Vector3Base<double> Vector3d;


}


#endif

