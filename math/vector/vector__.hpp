#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_



#include <cstdio>
#include <cstdarg>
#include <exception>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>


namespace hoshi
{


template<size_t Dimention, typename Precision = double>
class vector_base
    : public boost::numeric::ublas::vector<Precision>
{
public:
    typedef boost::numeric::ublas::vector<Precision> super_type;
    typedef const boost::numeric::ublas::vector<Precision> const_super_type;

    typedef vector_base self_type;
    typedef const vector_base const_self_type;

    typedef Precision type;
    typedef const Precision const_type;


public:
    size_t dimention;


public:
    vector_base()
        : super_type(Dimention), dimention(Dimention)
    {
        for(auto itr = this->begin(); itr != this->end(); ++itr) {
            *itr = static_cast<Precision>(0);
        }
    }


    vector_base(const_type _1, ...)
        : super_type(Dimention), dimention(Dimention)
    {
        va_list argptr;

        va_start(argptr, _1);
        variableCtor(_1, argptr);
        va_end(argptr);
    }


    vector_base(const_self_type& rhs)
        : super_type(rhs), dimention(Dimention)
    {
    }


    vector_base(const_super_type& rhs)
        : super_type(rhs), dimention(Dimention)
    {
    }


    virtual ~vector_base()
    {
    }


    virtual void variableCtor(const_type _1, va_list chain)
    {
        for(auto itr = this->begin(); itr != this->end(); ++itr) {
            if(itr == this->begin()) {
                *itr = _1;
            } else {
                *itr = static_cast<Precision>(va_arg(chain, Precision));
            }
        }
    }


    // 継承先で方が解決できないので使えない
    // virtual super_type& operator=(const_super_type& rhs)
    // {
    //     for(size_t i = 0; i < this->size(); ++i) {
    //         (*this)[i] = rhs[i];
    //     }

    //     return *this;
    // }


    operator super_type() {
        return self_type()
    }

    self_type& operator=(const_self_type& rhs) { 
        for(size_t i = 0; i < this->size(); ++i) { (*this)[i] = rhs[i]; } 
        return *this; 
    } 
    self_type& operator=(const_super_type& rhs) { 
        return this->operator=(self_type(rhs)); 
    }
public:
    virtual void print()
    {
        std::printf("vector:\n");
        
        for(auto itr = this->begin(); itr != this->end(); ++itr) {
            std::printf("\t%f\n", *itr);
        }
    }


private:
    virtual bool alignmentCheck(const_self_type& rhs) const
    {
        return this->size() == rhs.size();
    }


public:
    virtual bool operator==(const_self_type& rhs) const
    {
        if(!alignmentCheck(rhs)) return false;

        for(size_t i = 0; i < this->size(); ++i) {
            if((*this)[i] != rhs[i]) return false;
        }

        return true;
    }


    virtual bool operator!=(const_self_type& rhs) const
    {
        return !(operator==(rhs));
    }


    virtual self_type operator-() const
    {
        self_type tmp(*this);

        for(auto itr = tmp.begin(); itr != tmp.end(); ++itr) {
            *itr = -(*itr);
        }

        return tmp;
    }


    virtual super_type operator+(const_super_type& rhs) const
    {
        super_type tmp(*this);

        return tmp;
    }


public:
    virtual type snorm() const
    {
        // type tmp = static_cast<type>(0);
        // for(auto itr = this->begin(); itr != this->end(); ++itr) {
        //     tmp += (*itr) * (*itr);
        // }
        // return tmp;
        return norm() * norm();
    }


    virtual type norm() const
    {
        //return static_cast<type>(std::sqrt(snorm()));
        return norm_2(*this);
    }


    virtual void normalize()
    {
        type n = norm();

        assert(n != static_cast<type>(0));

        type tmp = static_cast<type>(1) / n;

        for(auto itr = this->begin(); itr != this->end(); ++itr) {
            *itr *= tmp;
        }
    }


    virtual self_type normalized() const
    {
        self_type tmp(*this);
        
        tmp.normalize();

        return self_type(tmp);
    }
};






// 特殊化
#define VECTOR_TYPES(dimention, precision) \
    public: typedef vector_base<dimention, precision> super_type; \
    public: typedef const vector_base<dimention, precision> const_super_type; \
    public: typedef typename super_type::type type; \
    public: typedef const typename super_type::type const_type; \
    public: typedef vector<dimention, precision> self_type; \
    public: typedef const vector<dimention, precision> const_self_type; 


#define VECTOR_VARIABLE_CTOR_PROCESS() { \
    va_list argptr; \
    va_start(argptr, _1); \
    this->variableCtor(_1, argptr); \
    va_end(argptr); \
    }

#define VECTOR_COPY_OPERATOR() 


template<size_t Dimention, typename Precision>
class vector
    : public vector_base<Dimention, Precision>
{
    // g++ 4.7 inheriting constructors no support
    // http://gcc.gnu.org/gcc-4.7/cxx0x_status.html
    // using vector_base<Dimention, Precision>::vector_base;

public:
    VECTOR_TYPES(Dimention, Precision);

public:
    vector() : super_type() {}
    vector(const_self_type& rhs) : super_type(rhs) {}
    vector(const_super_type& rhs) : super_type(rhs) {}
    vector(const_type _1, ...) : super_type() { VECTOR_VARIABLE_CTOR_PROCESS(); }

public:
    VECTOR_COPY_OPERATOR();
};


template<typename Precision>
class vector<2, Precision>
    : public vector_base<2, Precision>
{
public:
    VECTOR_TYPES(2, Precision);

public:
    vector() : super_type(), x((*this)[0]), y((*this)[1]) {}
    vector(const_self_type& rhs) : super_type(rhs), x((*this)[0]), y((*this)[1]) {}
    vector(const_super_type& rhs) : super_type(rhs), x((*this)[0]), y((*this)[1]) {}
    vector(const_type _1, ...) : super_type(), x((*this)[0]), y((*this)[1]) { VECTOR_VARIABLE_CTOR_PROCESS(); }

public:
    VECTOR_COPY_OPERATOR();

public:
    type& x;
    type& y;
};


template<typename Precision>
class vector<3, Precision>
    : public vector_base<3, Precision>
{
public:
    VECTOR_TYPES(3, Precision);

public:
    vector() : super_type(), x((*this)[0]), y((*this)[1]), z((*this)[2]) {}
    vector(const_self_type& rhs) : super_type(rhs), x((*this)[0]), y((*this)[1]), z((*this)[2]) {}
    vector(const_super_type& rhs) : super_type(rhs), x((*this)[0]), y((*this)[1]), z((*this)[2]) {}
    vector(const_type _1, ...) : super_type(), x((*this)[0]), y((*this)[1]), z((*this)[2]) { VECTOR_VARIABLE_CTOR_PROCESS(); }

public:
    VECTOR_COPY_OPERATOR();

public:
    type& x;
    type& y;
    type& z;

public:
    type dot(const_self_type& rhs)
    {
        return ( x*rhs.x + y*rhs.y + z*rhs.z );
    }

    self_type cross(const_self_type& rhs)
    {
        return self_type(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x);
    }

    type distance(const_self_type& rhs) const
    {
        type dx = x - rhs.x;
        type dy = y - rhs.y;
        type dz = z - rhs.z;

        return std::sqrt( dx*dx + dy*dy + dz*dz );
    }
};


template<typename Precision>
class vector<4, Precision>
    : public vector_base<4, Precision>
{
public:
    VECTOR_TYPES(4, Precision);

public:
    vector() : super_type(), x((*this)[0]), y((*this)[1]), z((*this)[2]), w((*this)[3]) {}
    vector(const_self_type& rhs) : super_type(rhs), x((*this)[0]), y((*this)[1]), z((*this)[2]), w((*this)[3]) {}
    vector(const_super_type& rhs) : super_type(rhs), x((*this)[0]), y((*this)[1]), z((*this)[2]), w((*this)[3]) {}
    vector(const_type _1, ...) : super_type(), x((*this)[0]), y((*this)[1]), z((*this)[2]), w((*this)[3]) { VECTOR_VARIABLE_CTOR_PROCESS(); }

public:
    VECTOR_COPY_OPERATOR();

public:
    type& x;
    type& y;
    type& z;
    type& w;
};


typedef vector<2, double> vector2d;
typedef vector<3, double> vector3d;
typedef vector<4, double> vector4d;


};



#endif

