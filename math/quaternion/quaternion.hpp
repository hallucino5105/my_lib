#ifndef _QUARTERNION_HPP_
#define _QUARTERNION_HPP_


#include <cmath>
#include <functional>
#include <vector/vector.hpp>
#include <matrix/matrix.hpp>
//#include <demangle/demangle.hpp>


namespace hoshi
{



template<size_t T, size_t U, typename P> class matrix_base;



template<typename Precision = double>
class quaternion_base
{
public:
    template<size_t T, size_t U, typename P> friend class matrix_base;


public:
    typedef Precision value_type;
    typedef const Precision const_value_type;

    typedef quaternion_base<value_type> self_type;
    typedef const quaternion_base<value_type> const_self_type;

    typedef vector3d vector_type;
    typedef const vector3d const_vector_type;

    typedef matrix_base<4, 4, value_type> matrix_type;
    typedef const matrix_base<4, 4, value_type> const_matrix_type;


public:
    value_type w;
    vector_type v;
    vector_type::value_type& x;
    vector_type::value_type& y;
    vector_type::value_type& z;


public:
    quaternion_base() : 
        w(static_cast<value_type>(0)), 
        v(static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(0)),
        x(v.x), y(v.y), z(v.z)
    {
    }


    quaternion_base(const_value_type _w, const_value_type _x, const_value_type _y, const_value_type _z) : 
        w(_w), v(_x, _y, _z), x(v.x), y(v.y), z(v.z)
    {
    }


    // quaternion_base(const_value_type _w, const_vector_type& _v) : 
    //     w(_w), v(_v), x(v.x), y(v.y), z(v.z)
    // {
    // }


    quaternion_base(const_value_type angle, const_vector_type& axis) : 
         x(v.x), y(v.y), z(v.z)
    {
        value_type radian = static_cast<value_type>(angle * (M_PI/180.0) / 2.0);
        value_type s = std::sin(radian);
        value_type c = std::cos(radian);

        w = c;
        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
    }


    // rotation quaternion
    //  -- bank, attitude, heading
    //  -- (euler angle)
	quaternion_base(const_value_type theta_x, const_value_type theta_y, const_value_type theta_z) : 
        x(v.x), y(v.y), z(v.z)
	{
        auto rad = [](const_value_type theta){ return static_cast<value_type>(theta * (M_PI/180.0) / 2.0); };
        
        value_type sin_x_2 = 0.5*std::sin(rad(theta_x));
        value_type sin_y_2 = 0.5*std::sin(rad(theta_y));
        value_type sin_z_2 = 0.5*std::sin(rad(theta_z));

        value_type cos_x_2 = 0.5*std::cos(rad(theta_x));
        value_type cos_y_2 = 0.5*std::cos(rad(theta_y));
        value_type cos_z_2 = 0.5*std::cos(rad(theta_z));

        w   = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
        v.x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
        v.y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
        v.z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;
    }


    quaternion_base(const_matrix_type& m) : 
        x(v.x), y(v.y), z(v.z)
    {
        if(m.dimention_row != 4 || m.dimention_col != 4) {
            throw std::runtime_error("damepo0");
        }
        
        //std::cout << m << std::endl;

        value_type wxyz[4];
        wxyz[0] =  m(0, 0) + m(1, 1) + m(2, 2) + 1.0;
        wxyz[1] =  m(0, 0) - m(1, 1) - m(2, 2) + 1.0;
        wxyz[2] = -m(0, 0) + m(1, 1) - m(2, 2) + 1.0;
        wxyz[3] = -m(0, 0) - m(1, 1) + m(2, 2) + 1.0;
    
        size_t biggest = 0;
        for(size_t i = 0; i < 4; ++i) {
            if(wxyz[i] > wxyz[biggest] ) {
                biggest = i;
            }
        }

        if(wxyz[biggest] < static_cast<value_type>(0)) {
            throw std::runtime_error("damepo1");
        }

        value_type* q[4] = { &((*this).w), &((*this).x), &((*this).y), &((*this).z) };
        value_type vl = sqrt(wxyz[biggest]) * 0.5;
        *q[biggest] = vl;
        value_type mult = 0.25 / vl;

        switch(biggest) {
            case 0: // w
                *q[1] = (m(1, 2) - m(2, 1)) * mult;
                *q[2] = (m(2, 0) - m(0, 2)) * mult;
                *q[3] = (m(0, 1) - m(1, 0)) * mult;
                break;

            case 1: // x
                *q[0] = (m(1, 2) - m(2, 1)) * mult;
                *q[2] = (m(0, 1) + m(1, 0)) * mult;
                *q[3] = (m(2, 0) + m(0, 2)) * mult; 
                break;

            case 2: // y
                *q[0] = (m(2, 0) - m(0, 2)) * mult;
                *q[1] = (m(0, 1) + m(1, 0)) * mult;
                *q[3] = (m(1, 2) + m(2, 1)) * mult;
                break;

            case 3: // z
                *q[0] = (m(0, 1) - m(1, 0)) * mult;
                *q[1] = (m(2, 0) + m(0, 2)) * mult;
                *q[2] = (m(1, 2) + m(2, 1)) * mult;
                break;
        }
    }


    quaternion_base(const_self_type& rhs) : 
        x(v.x), y(v.y), z(v.z)
    {
        w = rhs.w;
        v = rhs.v;
    }


    // quaternion_base(self_type&& rhs) :
    //     x(v.x), y(v.y), z(v.z)
    // {
    //     w = static_cast<value_type&&>(rhs.w);
    //     v = static_cast<vector_type&&>(rhs.v);
    // }


    virtual ~quaternion_base()
    {
    }


    self_type& operator=(const_self_type& rhs)
    {
        w = rhs.w;
        v = rhs.v;

        return *this;
    }


public:
    bool operator==(const_self_type& rhs) const
    {
        return ( w == rhs.w && v == rhs.v );
    }


    bool operator!=(const_self_type& rhs) const
    {
        return !(*this == rhs);
    }


public:
    void clear()
    {
        w = static_cast<value_type>(0);
        v.clear();
    }


    void identity()
    {
        w = 1.0;
        v.x = v.y = v.z = 0.0;
    }

 
    self_type operator-() const
    {
        return self_type(-w, -v);
    }


    self_type operator+(const_self_type& rhs) const
    {
        return self_type(w + rhs.w, v + rhs.v);
    }


    self_type operator+(const_value_type rhs) const
    {
        return self_type(w + rhs, v + rhs);
    }


    self_type operator-(const_self_type& rhs) const
    {
        return self_type(w - rhs.w, v - rhs.v);
    }


    self_type operator-(const_value_type rhs) const
    {
        return self_type(w - rhs, v - rhs);
    }


    self_type operator*(const_self_type& rhs) const
    {
        return self_type(
            w*rhs.w - v.dot(rhs.v), 
            v.y*rhs.v.z - v.z*rhs.v.y + w*rhs.v.x + v.x*rhs.w, 
            v.z*rhs.v.x - v.x*rhs.v.z + w*rhs.v.y + v.y*rhs.w, 
            v.x*rhs.v.y - v.y*rhs.v.x + w*rhs.v.z + v.z*rhs.w);
    }


    self_type operator*(const_value_type rhs) const
    {
        return self_type(w * rhs, v * rhs);
    }


    self_type operator/(const_self_type& rhs) const
    {
        self_type tmp(rhs);
        tmp.invert();

        return *this * tmp;
    }


    self_type operator/(const_value_type rhs) const
    {
        assert(rhs != static_cast<value_type>(0));
        return self_type(w / rhs, v / rhs);
    }


    self_type& operator+=(const_self_type& rhs) 
    {
        *this = *this + rhs;
        return *this;
    }


    self_type& operator-=(const_self_type& rhs) 
    {
        *this = *this - rhs;
        return *this;
    }


    self_type& operator*=(const_self_type rhs)
    {
        *this = *this * rhs;
        return *this;
    }


    self_type& operator/=(const_self_type rhs)
    {
        *this = *this / rhs;
        return *this;
    }


public:
    void normalize()
    {
        value_type n = this->norm();
        assert(n != static_cast<value_type>(0));

        value_type tmp = static_cast<value_type>(1) / n;
        w *= tmp;
        v = v * tmp;
    }


    self_type normalized() const
    {
        self_type tmp(*this);
        tmp.normalize();

        return self_type(tmp.w, tmp.x, tmp.y, tmp.z);
    }


    // 共役
    void conjugate()
    {
        v = -v;
    }


    self_type conjugated() const
    {
        return self_type(w, -v.x, -v.y, -v.z);
    }


    value_type norm() const
    {
        return static_cast<value_type>(std::sqrt(this->snorm()));
    }


    // norm by square（なんていうの？）
    value_type snorm() const
    {
        return ( w*w + v.snorm() );
    }


    self_type& invert()
    {
        this->conjugate();
        *this = *this / this->snorm();

        return *this;
    }


    value_type dot(const_self_type& rhs) const
    {
        return ( w*rhs.w + v.dot(rhs.v) );
    }


public:
    // linear interpolation
    //  -- t: coefficient(0.0 ~ 1.0)
    self_type lerp(const_self_type& rhs, const_value_type t) const
    {
        return ( (*this)*(1-t) + rhs*t ).normalized();
    }


    // spherical linear interpolation
    self_type slerp(const_self_type& rhs, const_value_type t) const
    {
        self_type tmp;
		value_type dot = this->dot(rhs);

		if(dot < 0) {
			dot = -dot;
			tmp = -rhs;
		} else {
            tmp = rhs;
        }
		
		if(dot < 0.95) {
			value_type angle = std::acos(dot);
			return ((*this) * std::sin(angle*(1-t)) + tmp * std::sin(angle*t)) / std::sin(angle);
		} else {
            return this->lerp(tmp, t);
        }
    }


	// rotates v by this quaternion 
    // （must use rotation quaternion）
	vector_type rotate(const_vector_type& v)
	{   
		self_type q(0, v);
	    self_type cthis(*this);

        cthis.conjugate();

		return ( *this * q * cthis ).v;
	}


    /*
	// converts from a normalized axis - angle pair rotation to a quaternion
    //  -- axis: axis unit vector
    //  -- angle: radian
	static self_type fromAxisAngle(const Vector3d& axis, type angle)
	{
        return self_type( std::cos(angle/2), axis * std::sin(angle/2) );
    }


	// returns the axis and angle of this unit quaternion
	void toAxisAngle(Vector3d& axis, type& angle) const
	{
		angle = std::acos(w);

		// pre-compute to save time
		type sin_theta_inv = 1.0 / std::sin(angle);

		// now the vector
		axis.x = v.x * sin_theta_inv;
		axis.y = v.y * sin_theta_inv;
		axis.z = v.z * sin_theta_inv;

		angle *= 2;
	}
    */


public:
    static value_type dot(const_self_type& lhs, const_self_type& rhs)
    {
        return lhs.dot(rhs);
    }


    static self_type lerp(const_self_type& lhs, const_self_type& rhs, const_value_type t)
    {
        return lhs.lerp(rhs, t);
    }


    static self_type slerp(const_self_type& lhs, const_self_type& rhs, const_value_type t)
    {
        return lhs.slerp(rhs, t);
    }


public:
    void print() const
    {
        std::printf("quaternion:\n");
        std::printf("\tw=%0.6f", w);
        std::printf("\tx=%0.6f", v.x);
        std::printf("\ty=%0.6f", v.y);
        std::printf("\tz=%0.6f\n", v.z);
    }
};



typedef quaternion_base<double> quaternion;


};



#endif

