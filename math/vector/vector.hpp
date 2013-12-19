#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_



#include <cstdio>
#include <cstdarg>

#include <exception>
#include <memory>

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>



namespace hoshi
{


template<int Dimention = 2, typename Precision = double>
class vector_base
    : public boost::numeric::ublas::vector<Precision>
{
public:
    typedef boost::numeric::ublas::vector<Precision>        root_type;
    typedef const boost::numeric::ublas::vector<Precision>  const_root_type;
    typedef Precision                                       value_type;
    typedef const Precision                                 const_value_type;
    typedef vector_base<Dimention, value_type>              self_type;
    typedef const vector_base<Dimention, value_type>        const_self_type;


public:
    static const int dimention = Dimention;


public:
    std::shared_ptr<value_type> vec;


public:
    vector_base() : 
        root_type(dimention),
        vec( new value_type[dimention], std::default_delete<value_type[]>() )
    {
        for(auto itr = this->begin(); itr != this->end(); ++itr) {
            *itr = static_cast<value_type>(0);
        }
    }


    template<typename ... Types>
    vector_base(const_value_type first, Types ... chain) : 
        root_type(dimention),
        vec( new value_type[dimention], std::default_delete<value_type[]>() )
    {
        (*this)[0] = first;

        size_t i = 1;
        for(value_type value : { static_cast<value_type>(chain)... }) {
            (*this)[i++] = value;
        }
    }


    vector_base(const_root_type& rhs) : 
        root_type(dimention),
        vec( new value_type[dimention], std::default_delete<value_type[]>() )
    {
        for(size_t i = 0; i < this->size(); ++i) {
            (*this)[i] = rhs[i];
        }
    }


    virtual ~vector_base()
    {
    }


    // operator=は継承できませんよ
    root_type& operator=(const_root_type& rhs)
    {
        for(size_t i = 0; i < this->size(); ++i) {
            (*this)[i] = rhs[i];
        }

        return *this;
    }


protected:
    //virtual void variableCtor(const_value_type _1, va_list chain)
    //{
    //    for(auto itr = this->begin(); itr != this->end(); ++itr) {
    //        if(itr == this->begin()) {
    //            *itr = _1;
    //        } else {
    //            *itr = static_cast<value_type>(va_arg(chain, value_type));
    //        }
    //    }
    //}


public:
    virtual void print() const
    {
        std::printf("vector:\n");
        
        for(auto itr = this->begin(); itr != this->end(); ++itr) {
            std::printf("\t%lf\n", *itr);
        }
    }


protected:
    virtual bool alignmentCheck(const_root_type& rhs) const
    {
        return this->size() == rhs.size();
    }


public:
    virtual bool operator==(const_root_type& rhs) const
    {
        if(!alignmentCheck(rhs)) return false;

        for(size_t i = 0; i < this->size(); ++i) {
            if((*this)[i] != rhs[i]) return false;
        }

        return true;
    }


    virtual bool operator!=(const_root_type& rhs) const
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


    virtual root_type operator+(const_root_type& rhs) const
    {
        // これだと再帰でoperator+がよばれてしまう（なんで？）
        // return *this + rhs;

        root_type tmp(*this);
        return static_cast<root_type>(tmp + rhs);
    }


    virtual root_type operator+(const_value_type rhs) const
    {
        root_type tmp(*this);

        for(auto itr = tmp.begin(); itr != tmp.end(); ++itr) {
            *itr += rhs;
        }

        return tmp;
    }


    virtual root_type& operator+=(const_root_type& rhs)
    {
        *this = *this + rhs;
        return *this;
    }


    virtual root_type operator-(const_root_type& rhs) const
    {
        root_type tmp(*this);
        return static_cast<root_type>(tmp - rhs);
    }


    virtual root_type operator-(const_value_type rhs) const
    {
        root_type tmp(*this);

        for(auto itr = tmp.begin(); itr != tmp.end(); ++itr) {
            *itr -= rhs;
        }

        return tmp;
    }


    virtual root_type& operator-=(const_root_type& rhs)
    {
        *this = *this - rhs;
        return *this;
    }


    virtual root_type operator*(const_value_type rhs) const
    {
        root_type tmp(*this);
        return static_cast<root_type>(tmp * rhs);
    }


    virtual root_type operator/(const_value_type rhs) const
    {
        root_type tmp(*this);
        return static_cast<root_type>(tmp / rhs);
    }


public:
    virtual value_type snorm() const
    {
        // type tmp = static_cast<type>(0);
        // for(auto itr = this->begin(); itr != this->end(); ++itr) {
        //     tmp += (*itr) * (*itr);
        // }
        // return tmp;
        return norm() * norm();
    }


    virtual value_type norm() const
    {
        //return static_cast<type>(std::sqrt(snorm()));
        return norm_2(*this);
    }


    virtual void normalize()
    {
        value_type n = norm();

        assert(n != static_cast<value_type>(0));

        value_type tmp = static_cast<value_type>(1) / n;

        for(auto itr = this->begin(); itr != this->end(); ++itr) {
            *itr *= tmp;
        }
    }


    virtual self_type normalized() const
    {
        self_type tmp(*this);
        
        tmp.normalize();

        return tmp;
    }


public:
    virtual value_type* getArrayPtr()
    {
        for(size_t i = 0; i < (*this).size(); ++i) {
            vec.get()[i] = (*this)[i];
        }

        return vec.get();
    }
};






#define VECTOR_TYPES(dimention, precision) \
    public: typedef vector_base<dimention, precision>           super_type; \
    public: typedef const vector_base<dimention, precision>     const_super_type; \
    public: typedef typename super_type::root_type              root_type; \
    public: typedef const typename super_type::const_root_type  const_root_type; \
    public: typedef typename super_type::value_type             value_type; \
    public: typedef const typename super_type::const_value_type const_value_type; \
    public: typedef vector<dimention, precision>                self_type; \
    public: typedef const vector<dimention, precision>          const_self_type; 


#define VECTOR_VARIABLE_CTOR_PROCESS() { \
        (*this)[0] = first; \
        size_t i = 1; \
        for(value_type value : { static_cast<value_type>(chain)... }) { \
            (*this)[i++] = value; \
        } \
    }


#define VECTOR_OPERATOR_COPY() \
    self_type& operator=(const_self_type& rhs) { \
        return dynamic_cast<self_type&>(super_type::operator=(rhs)); \
    }



template<int Dimention = 2, typename Precision = double>
class vector
    : public vector_base<Dimention, Precision>
{
    // g++ 4.7 inheriting constructors no support
    // http://gcc.gnu.org/gcc-4.7/cxx0x_status.html
    // - using vector_base<Dimention, Precision>::vector_base;

public:
    VECTOR_TYPES(Dimention, Precision);
    VECTOR_OPERATOR_COPY();

public:
    vector() : super_type() {}
    vector(const_root_type& rhs) : super_type(rhs) {}

    template<typename ... Types> 
    vector(const_value_type first, Types ... chain) : super_type() 
    { VECTOR_VARIABLE_CTOR_PROCESS(); }
};



template<typename Precision>
class vector<2, Precision>
    : public vector_base<2, Precision>
{
public:
    VECTOR_TYPES(2, Precision);
    VECTOR_OPERATOR_COPY();

public:
    vector() : super_type(), x((*this)[0]), y((*this)[1]) {}
    vector(const_root_type& rhs) : super_type(rhs), x((*this)[0]), y((*this)[1]) {}

    template<typename ... Types>
    vector(const_value_type first, Types ... chain) : super_type(), x((*this)[0]), y((*this)[1])
    {  VECTOR_VARIABLE_CTOR_PROCESS(); }

public:
    value_type& x;
    value_type& y;
};



template<typename Precision>
class vector<3, Precision>
    : public vector_base<3, Precision>
{
public:
    VECTOR_TYPES(3, Precision);
    VECTOR_OPERATOR_COPY();

public:
    vector() : super_type(), x((*this)[0]), y((*this)[1]), z((*this)[2]) {}
    vector(const_root_type& rhs) : super_type(rhs), x((*this)[0]), y((*this)[1]), z((*this)[2]) {}

    template<typename ... Types>
    vector(const_value_type first, Types ... chain) : super_type(), x((*this)[0]), y((*this)[1]), z((*this)[2])
    { VECTOR_VARIABLE_CTOR_PROCESS(); }

public:
    value_type& x;
    value_type& y;
    value_type& z;

public:
    value_type dot(const_self_type& rhs) const
    {
        return ( x*rhs.x + y*rhs.y + z*rhs.z );
    }

    self_type cross(const_self_type& rhs) const
    {
        return self_type(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x);
    }

    value_type distance(const_self_type& rhs) const
    {
        value_type dx = x - rhs.x;
        value_type dy = y - rhs.y;
        value_type dz = z - rhs.z;

        return std::sqrt( dx*dx + dy*dy + dz*dz );
    }
};



template<typename Precision>
class vector<4, Precision>
    : public vector_base<4, Precision>
{
public:
    VECTOR_TYPES(4, Precision);
    VECTOR_OPERATOR_COPY();

public:
    vector() : super_type(), x((*this)[0]), y((*this)[1]), z((*this)[2]), w((*this)[3]) {}
    vector(const_root_type& rhs) : super_type(rhs), x((*this)[0]), y((*this)[1]), z((*this)[2]), w((*this)[3]) {}

    template<typename ... Types>
    vector(const_value_type first, Types ... chain) : super_type(), x((*this)[0]), y((*this)[1]), z((*this)[2]), w((*this)[3])
    { VECTOR_VARIABLE_CTOR_PROCESS(); }

public:
    value_type& x;
    value_type& y;
    value_type& z;
    value_type& w;
};



typedef vector<2, double> vector2d;
typedef vector<3, double> vector3d;
typedef vector<4, double> vector4d;

typedef vector<2, float> vector2f;
typedef vector<3, float> vector3f;
typedef vector<4, float> vector4f;



};



#endif

