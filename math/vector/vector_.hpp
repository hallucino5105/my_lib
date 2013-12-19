#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_



#include <cstdio>
#include <cstdarg>
#include <exception>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>


namespace hoshi
{


template<size_t Dimention, typename Precision = float>
class vector
{
public:
    typedef boost::numeric::ublas::vector<Precision> super;

    typedef vector this_type;
    typedef const vector const_this_type;
    typedef Precision type;
    typedef const Precision const_type;
    

    typedef typename super::value_type             value_type;
    typedef typename super::reference              reference;
    typedef typename super::const_reference        const_reference;
    typedef typename super::size_type              size_type;
    typedef typename super::difference_type        difference_type;
    typedef typename super::const_iterator         const_iterator;
    typedef typename super::iterator               iterator;
    typedef typename super::const_reverse_iterator const_reverse_iterator;
    typedef typename super::reverse_iterator       reverse_iterator;
    typedef typename super::array_type             array_type;


public:
    size_t dimention;
    super v;


public:
    vector()
        : dimention(Dimention), v(Dimention)
    {
    }


    vector(const_type _1, ...)
        : dimention(Dimention), v(Dimention)
    {
        va_list argptr;
        va_start(argptr, _1);

        for(auto itr = v.begin(); itr != v.end(); ++itr) {
            if(itr == v.begin()) {
                *itr = _1;
            } else {
                *itr = va_arg(argptr, Precision);
            }
        }

        va_end(argptr);
    }


    vector(const super& rhs)
        : dimention(Dimention), v(rhs)
    {
        if(dimention != rhs.size()) throw std::exception();
    }


    vector(const_this_type& rhs)
        : dimention(Dimention), v(Dimention)
    {
        if(dimention != rhs.dimention) throw std::exception();

        for(unsigned int i = 0; i < dimention; ++i) {
            v[i] = rhs[i];
        }
    }


    ~vector()
    {
    }


public:
    void print()
    {
        std::printf("vector:\n");
        
        for(auto itr = v.begin(); itr != v.end(); ++itr) {
            std::printf("\t%f\n", *itr);
        }
    }


private:
    bool alignmentCheck(const_this_type& rhs) const
    {
        return v.size() == rhs.size();
    }


public:
    bool operator==(const_this_type& rhs) const
    {
        if(!alignmentCheck(rhs)) return false;

        for(size_t i = 0; i < v.size(); ++i) {
            if(v[i] != rhs[i]) return false;
        }

        return true;
    }


    bool operator!=(const_this_type& rhs) const
    {
        return !(operator==(rhs));
    }


    this_type operator-() const
    {
        this_type tmp(v);

        for(auto itr = tmp.begin(); itr != tmp.end(); ++itr) {
            *itr = -(*itr);
        }

        return tmp;
    }


    this_type operator+(const_this_type& rhs) const
    {
        return this_type(v + rhs.v);
    }


    this_type operator+(const_type rhs) const
    {
        this_type tmp(v);

        for(auto itr = tmp.begin(); itr != tmp.end(); ++itr) {
            *itr += rhs;
        }

        return tmp;
    }


    this_type operator-(const_this_type& rhs) const
    {
        return this_type(v - rhs.v);
    }


    this_type operator-(const_type rhs) const
    {
        this_type tmp(v);

        for(auto itr = tmp.begin(); itr != tmp.end(); ++itr) {
            *itr -= rhs;
        }

        return tmp;
    }


    this_type operator*(const_type rhs) const
    {
        this_type tmp(v);

        for(auto itr = tmp.begin(); itr != tmp.end(); ++itr) {
            *itr *= rhs;
        }

        return tmp;
    }


    this_type operator/(const_type rhs) const
    {
        assert(rhs != static_cast<type>(0));
        
        this_type tmp = v;

        for(auto itr = tmp.begin(); itr != tmp.end(); ++itr) {
            *itr /= rhs;
        }

        return tmp;
    }


public:
    void normalize()
    {
        type n = norm();

        assert(n != static_cast<type>(0));

        type tmp = static_cast<type>(1) / n;

        for(auto itr = v.begin(); itr != v.end(); ++itr) {
            *itr *= tmp;
        }
    }


    this_type normalized() const
    {
        this_type tmp(v);
        
        tmp.normalize();

        return this_type(tmp);
    }


    type norm() const
    {
        return static_cast<type>(std::sqrt(snorm()));
    }


    type snorm() const
    {
        type tmp = static_cast<type>(0);

        for(auto itr = v.begin(); itr != v.end(); ++itr) {
            tmp += (*itr) * (*itr);
        }

        return tmp;
    }

    
    // 内積
    type dot(const_this_type& rhs) const
    {
        return ;
    }


    // 外積
    this_type cross(const_this_type& rhs) const
    {
        return this_type;
    }


public:
    bool empty() const              { return v.empty(); }
    const array_type& data() const  { return v.data(); }
    array_type& data()              { return v.data(); }
    void swap(this_type& rhs)       { v.swap(rhs); }
    void clear()                    { v.clear(); }
    size_type size() const          { return v.size(); }
    size_type max_size() const      { return v.max_size(); }
    void resize(size_type size, bool preserve = true) { v.resize(size, preserve); }

    const_iterator begin() const            { return v.begin(); }
    const_iterator end() const              { return v.end(); }
    iterator begin()                        { return v.begin(); }
    iterator end()                          { return v.end(); }
    const_reverse_iterator rbegin() const   { return v.begin(); }
    const_reverse_iterator rend() const     { return v.end(); }
    reverse_iterator rbegin()               { return v.begin(); }
    reverse_iterator rend()                 { return v.end(); }

    const_reference operator()(size_type i) const   { return v.operator()(i); }
    reference operator()(size_type i)               { return v.operator()(i); }
    const_reference operator[](size_type i) const   { return v.operator[](i); }
    reference operator[](size_type i)               { return v.operator[](i); }

    this_type& operator=(const_this_type& rhs) { return v.operator=(rhs); }
    template<class AE> this_type& operator=(const boost::numeric::ublas::vector_expression<AE>& ae)    { return v.operator=(ae); }
    template<class AE> this_type& assign(const boost::numeric::ublas::vector_expression<AE>& ae)       { return v.assign(ae); }
    template<class AE> this_type& operator+=(const boost::numeric::ublas::vector_expression<AE>& ae)   { return v.operator+=(ae); }
    template<class AE> this_type& plus_assign(const boost::numeric::ublas::vector_expression<AE>& ae)  { return v.plus_assign(ae); }
    template<class AE> this_type& operator-=(const boost::numeric::ublas::vector_expression<AE>& ae)   { return v.operator-=(ae); }
    template<class AE> this_type& minus_assign(const boost::numeric::ublas::vector_expression<AE>& ae) { return v.minus_assign(ae); }
    template<class AT> this_type& operator*=(const AT& at) { return v.operator*=(at); }
    template<class AT> this_type& operator/=(const AT& at) { return v.operator/=(at); }
};


typedef vector<2, double> vector2d;
typedef vector<3, double> vector3d;
typedef vector<4, double> vector4d;


};



#endif

