#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_



// If g++ is debug mode, will generate the code which performs various checks. 
// assertが動かなくなるので注意
#define NDEBUG

#include <cstdio>
#include <cmath>

#include <iostream>
#include <exception>
#include <memory>

#include <boost/numeric/ublas/exception.hpp>
#include <boost/numeric/ublas/expression_types.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <vector/vector.hpp>
#include <quaternion/quaternion.hpp>
//#include <demangle/demangle.hpp>



namespace hoshi
{



template<typename P> class quaternion_base;



struct internal_logic_error : public boost::numeric::ublas::internal_logic
{
    explicit internal_logic_error(const char* s = "internal_logic_error") : 
        boost::numeric::ublas::internal_logic(s) {}
};



struct external_logic_error : public boost::numeric::ublas::external_logic
{
    explicit external_logic_error(const char* s = "external_logic_error") : 
        boost::numeric::ublas::external_logic(s) {}
};



template<size_t DimentionRow = 2, size_t DimentionCol = DimentionRow, typename Precision = double>
class matrix_base
    : public boost::numeric::ublas::matrix<Precision>
{
public:
    template<typename P> friend class quaternion_base;


public:
    typedef boost::numeric::ublas::matrix<Precision>                    root_type;
    typedef const boost::numeric::ublas::matrix<Precision>              const_root_type;
    typedef Precision                                                   value_type;
    typedef const Precision                                             const_value_type;
    typedef matrix_base<DimentionRow, DimentionCol, value_type>         self_type;
    typedef const matrix_base<DimentionRow, DimentionCol, value_type>   const_self_type;
    typedef matrix_base<DimentionCol, DimentionRow, value_type>         tself_type;
    typedef const matrix_base<DimentionCol, DimentionRow, value_type>   const_tself_type;
    typedef vector<3, value_type>                                       vector_type;
    typedef const vector<3, value_type>                                 const_vector_type;
    typedef quaternion_base<value_type>                                 quat_type;
    typedef const quaternion_base<value_type>                           const_quat_type;


public:
    static const size_t dimention_row = DimentionRow;
    static const size_t dimention_col = DimentionCol;


public:
    // 配列のポインタ用
    std::shared_ptr<value_type> mat;


public:
    matrix_base() : 
        root_type(dimention_row, dimention_col), 
        mat(new value_type[dimention_row * dimention_col], std::default_delete<value_type[]>())
    {
        for(auto itr1 = (*this).begin1(); itr1 != (*this).end1(); ++itr1) {
            for(auto itr2 = itr1.begin(); itr2 != itr1.end(); ++itr2) {
                *itr2 = static_cast<value_type>(0);
            }
        }
    }


    template<typename ... Types>
    matrix_base(value_type first, Types ... chain) :
        root_type(dimention_row, dimention_col),
        mat(new value_type[dimention_row * dimention_col], std::default_delete<value_type[]>())
    {
        static_assert(sizeof...(Types) == dimention_row * dimention_col - 1, "damepo1");

        (*this)(0, 0) = first;

        size_t i = 1;
        for(value_type value : { static_cast<value_type>(chain)... }) {
            (*this)(std::floor( i/static_cast<double>(dimention_col) ), i%dimention_col) = value;
            ++i;
        }
    }


    template<typename ... Types>
    matrix_base(const vector<dimention_col, value_type>& first, const Types& ... chain) :
        root_type(dimention_row, dimention_col),
        mat(new value_type[dimention_row * dimention_col], std::default_delete<value_type[]>())
    {
        static_assert(sizeof...(Types) == dimention_row - 1, "damepo2");
        static_assert(vector<dimention_col, value_type>::dimention == first.dimention, "damepo3");

        for(size_t i = 0; i < first.size(); ++i) {
            (*this)(0, i) = first[i];
        }

        // // なぜ動かぬ
        // size_t row = 1;
        // for(const_vector_type& value : { chain... }) {
        //     for(size_t j = 0; j < value.size(); ++j) {
        //         (*this)(row, j) = value[j];
        //     }
        //     ++row;
        // }

        size_t row = 1;
        auto args = { chain... }; // initializer_list<Types>
        for(auto itr = args.begin(); itr != args.end(); ++itr) {
            static_assert(vector<dimention_col, value_type>::dimention == (*itr).dimention, "damepo3");
            for(size_t j = 0; j < (*itr).size(); ++j) {
                (*this)(row, j) = (*itr)[j];
            }
            ++row;
        }
    }


    matrix_base(const_quat_type& q) :
        root_type(dimention_row, dimention_col), 
        mat(new value_type[dimention_row * dimention_col], std::default_delete<value_type[]>())
    {
        if(dimention_row != 4 || dimention_col != 4) {
            throw std::runtime_error("dampoe8");
        }

        quat_type t = q.normalized();

        (*this)(0, 0) = 1.0 - 2*t.y*t.y - 2*t.z*t.z;
        (*this)(0, 1) = 2*t.x*t.y + 2*t.w*t.z;
        (*this)(0, 2) = 2*t.x*t.z - 2*t.w*t.y;
        (*this)(0, 3) = 0.0;
        (*this)(1, 0) = 2*t.x*t.y - 2*t.w*t.z;
        (*this)(1, 1) = 1.0 - 2*t.x*t.x - 2*t.z*t.z;
        (*this)(1, 2) = 2*t.y*t.z + 2*t.w*t.x;
        (*this)(1, 3) = 0.0;
        (*this)(2, 0) = 2*t.x*t.z + 2*t.w*t.y;
        (*this)(2, 1) = 2*t.y*t.z - 2*t.w*t.x;
        (*this)(2, 2) = 1.0 - 2*t.x*t.x - 2*t.y*t.y;
        (*this)(2, 3) = 0.0;
        (*this)(3, 0) = 0.0;
        (*this)(3, 1) = 0.0;
        (*this)(3, 2) = 0.0;
        (*this)(3, 3) = 1.0;
    }


    matrix_base(const_root_type& rhs) :
        root_type(dimention_row, dimention_col), 
        mat(new value_type[dimention_row * dimention_col], std::default_delete<value_type[]>())
    {
        for(auto itr1 = (*this).begin1(); itr1 != (*this).end1(); ++itr1) {
            for(auto itr2 = itr1.begin(); itr2 != itr1.end(); ++itr2) {
                *itr2 = static_cast<value_type>(0);
            }
        }

        copyCtor(*this, rhs);
    }


    virtual ~matrix_base()
    {
    }


    root_type& operator=(const_root_type& rhs)
    {
        copyCtor(*this, rhs);
        return *this;
    }


    static void copyCtor(root_type& lhs, const_root_type& rhs)
    {
        size_t row = ( lhs.size1() <= rhs.size1() ) ? lhs.size1() : rhs.size1();
        size_t col = ( lhs.size2() <= rhs.size2() ) ? lhs.size2() : rhs.size2();

        for(size_t i = 0; i < row; ++i) {
            for(size_t j = 0; j < col; ++j) {
                lhs(i, j) = rhs(i, j);
            }
        }
    }


public:
    virtual value_type* getArrayPtr1()
    {
        for(size_t i = 0; i < (*this).size1(); ++i) {
            for(size_t j = 0; j < (*this).size2(); ++j) {
                mat.get()[ i*dimention_row + j ] = (*this)(i, j);
            }
        }

        return mat.get();
    }


    virtual value_type* getArrayPtr2()
    {
        for(size_t i = 0; i < (*this).size1(); ++i) {
            for(size_t j = 0; j < (*this).size2(); ++j) {
                mat.get()[ j*dimention_row + i ] = (*this)(i, j);
            }
        }

        return mat.get();
    }


public:
    virtual void print() const
    {
        std::printf("matrix%ldx%ld:\n", dimention_row, dimention_col);

        for(auto itr_row = this->begin1(); itr_row != this->end1(); ++itr_row) {
            std::printf("\t");
            for(auto itr_col = itr_row.begin(); itr_col != itr_row.end(); ++itr_col) {
                std::printf("%0.6lf\t", *itr_col);
            }
            std::printf("\n");
        }
    }
    

public:
    virtual bool isSquare() const
    {
        if(dimention_row == dimention_col) return true;
        else return false;
    }


    // return false element contains 'nan' or '+-inf'
    virtual bool isFinite() const
    {
        for(auto itr1 = (*this).begin1(); itr1 != (*this).end1(); ++itr1) {
            for(auto itr2 = itr1.begin(); itr2 != itr1.end(); ++itr2) {
                if(!std::isfinite(*itr2)) return false;
            }
        }

        return true;
    }


public:
    // no resizeable
    void resize(const size_t row, const size_t col, bool preserve = true)
    {
        //dimention_row = row;
        //dimention_col = col;
        //root_type::resize(row, col, preserve);
    }


    virtual void identity() throw(std::runtime_error)
    {
        if(!isSquare()) throw std::runtime_error("damepo4");

        for(size_t i = 0; i < (*this).size1(); ++i) {
            for(size_t j = 0; j < (*this).size2(); ++j) {
                if(i == j) {
                    (*this)(i, j) = static_cast<value_type>(1);
                } else {
                    (*this)(i, j) = static_cast<value_type>(0);
                }
            }
        }
    }


    virtual tself_type transpose() const
    {
        tself_type tmp;

        for(size_t i = 0; i < dimention_row; ++i) {
            for(size_t j = 0; j < dimention_col; ++j) {
                tmp(j, i) = (*this)(i, j);
            }
        }

        return tmp;
    }


    virtual self_type inverse()
    {
        if(!isSquare()) throw std::runtime_error("damepo5");

        self_type tmp1 = *this;
        self_type tmp2;
        self_type i;

        tmp2.identity();
        i.identity();

        // 置換行列
        boost::numeric::ublas::permutation_matrix<> pm(tmp1.size1());

        lu_factorize(tmp1, pm);
        lu_substitute(tmp1, pm, tmp2);

        // 逆行列が存在しない場合
        if(!boost::numeric::ublas::detail::expression_type_check((*this) * tmp2, i)) {
            throw internal_logic_error("damepo6");
        }

        if(!tmp2.isFinite()) {
            throw internal_logic_error("damepo7");
        }

        return tmp2;
    }


public:
    virtual bool operator==(const_self_type& rhs) const
    {
        if( (*this).dimention_row != rhs.dimention_row ) return false;
        if( (*this).dimention_col != rhs.dimention_col ) return false;

        for(size_t i = 0; i < rhs.size1(); ++i) {
            for(size_t j = 0; j < rhs.size2(); ++j) {
                if( static_cast<value_type>( (*this)(i, j) ) != static_cast<value_type>( rhs(i, j) ) ) {
                    return false;
                }
            }
        }

        return true;
    }


    virtual bool operator!=(const_self_type& rhs) const
    {
        return !(*this == rhs);
    }


    virtual self_type operator-() const
    {
        self_type tmp(*this);

        for(auto itr1 = tmp.begin1(); itr1 != tmp.end1(); ++itr1) {
            for(auto itr2 = itr1.begin(); itr2 != itr1.end(); ++itr2) {
                *itr2 = -(*itr2);
            }
        }

        return tmp;
    }


    virtual root_type operator+(const_root_type& rhs) const
    {
        root_type tmp(*this);
        return static_cast<root_type>(tmp + rhs);
    }


    virtual root_type operator-(const_root_type& rhs) const
    {
        root_type tmp(*this);
        return static_cast<root_type>(tmp - rhs);
    }


public:
    virtual void translate(const_vector_type& v)
    {
        translate(v.x, v.y, v.z);
    }


    virtual void translate(const_value_type dx, const_value_type dy, const_value_type dz)
    {
        self_type tmp(
            1.0, 0.0, 0.0, dx, 
            0.0, 1.0, 0.0, dy, 
            0.0, 0.0, 1.0, dz, 
            0.0, 0.0, 0.0, 1.0);

        *this = *this * tmp;
    }


    virtual void scale(const_vector_type& v)
    {
        scale(v.x, v.y, v.z);
    }


    virtual void scale(const_value_type sx, const_value_type sy, const_value_type sz)
    {
        self_type tmp(
             sx, 0.0, 0.0, 0.0,
            0.0,  sy, 0.0, 0.0,
            0.0, 0.0,  sz, 0.0,
            0.0, 0.0, 0.0, 1.0);

        *this = *this * tmp;
    }


    virtual void rotate(const_vector_type& v)
    {
        rotate(v.x, v.y, v.z);
    }


    virtual void rotate(const_value_type ax, const_value_type ay, const_value_type az)
    {
        self_type tmp(quat_type(ax, ay, az));

        *this = *this * tmp;
    }
};



template<class Expression>
boost::numeric::ublas::matrix<typename Expression::value_type>
operator*(
    const boost::numeric::ublas::matrix_expression<Expression>& lhs, 
    const boost::numeric::ublas::matrix_expression<Expression>& rhs)
{
    auto tmp = prod(lhs, rhs);
    return boost::numeric::ublas::matrix<typename Expression::value_type>(tmp);
}


template<class Expression1, class Expression2>
boost::numeric::ublas::vector<typename Expression2::value_type>
operator*(
    const boost::numeric::ublas::matrix_expression<Expression1>& lhs, 
    const boost::numeric::ublas::vector_expression<Expression2>& rhs)
{
    auto tmp = prod(lhs, rhs);
    return boost::numeric::ublas::vector<typename Expression2::value_type>(tmp);
}


template<class Expression1, class Expression2>
boost::numeric::ublas::vector<typename Expression2::value_type>
operator*(
    const boost::numeric::ublas::vector_expression<Expression2>& lhs, 
    const boost::numeric::ublas::matrix_expression<Expression1>& rhs)
{
    auto tmp = prod(lhs, rhs);
    return boost::numeric::ublas::vector<typename Expression2::value_type>(tmp);
}


template<size_t R, size_t C, typename P>
matrix_base<R, C, P>
operator*(const matrix_base<R, C, P>& lhs, const double rhs)
{
    return static_cast<matrix_base<R, C, P>>(boost::numeric::ublas::operator*(lhs, rhs));
}


template<size_t R, size_t C, typename P>
matrix_base<R, C, P>
operator*(const matrix_base<R, C, P>& lhs, const int rhs)
{
    return static_cast<matrix_base<R, C, P>>(boost::numeric::ublas::operator*(lhs, rhs));
}


template<size_t R, size_t C, typename P>
matrix_base<R, C, P>
operator*(const double rhs, const matrix_base<R, C, P>& lhs)
{
    return static_cast<matrix_base<R, C, P>>(boost::numeric::ublas::operator*(lhs, rhs));
}


template<size_t R, size_t C, typename P>
matrix_base<R, C, P>
operator*(const int rhs, const matrix_base<R, C, P>& lhs)
{
    return static_cast<matrix_base<R, C, P>>(boost::numeric::ublas::operator*(lhs, rhs));
}



// // boostと被るからだめ
// template<typename Expression1, class Expression2>
// boost::numeric::ublas::matrix<typename Expression1::value_type>
// operator*(
//     const boost::numeric::ublas::matrix_expression<Expression1>& rhs,
//     const Expression2& lhs)
// {
//     auto tmp = lhs * rhs;
//     return boost::numeric::ublas::matrix<typename Expression1::value_type>(tmp);
// }



typedef matrix_base<3, 3, double> matrixd;
typedef matrix_base<2, 2, double> matrix2d;
typedef matrix_base<3, 3, double> matrix3d;
typedef matrix_base<4, 4, double> matrix4d;
typedef matrix_base<4, 3, double> matrix4x3d;
typedef matrix_base<3, 4, double> matrix3x4d;

typedef matrix_base<3, 3, float> matrixf;
typedef matrix_base<2, 2, float> matrix2f;
typedef matrix_base<3, 3, float> matrix3f;
typedef matrix_base<4, 4, float> matrix4f;
typedef matrix_base<4, 3, float> matrix4x3f;
typedef matrix_base<3, 4, float> matrix3x4f;




template<typename Precision = double>
static inline 
matrix_base<4, 4, Precision>
createMatrixGl(Precision m[16])
{
    return matrix_base<4, 4, Precision>(
        m[0], m[4], m[ 8], m[12], 
        m[1], m[5], m[ 9], m[13], 
        m[2], m[6], m[10], m[14], 
        m[3], m[7], m[11], m[15]);
}



};



#endif

