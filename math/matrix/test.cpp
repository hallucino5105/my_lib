#include <cstdio>
#include <iostream>
#include <boost/numeric/ublas/io.hpp>
#include <demangle/demangle.hpp>
#include <quaternion/quaternion.hpp>
#include "matrix.hpp"


template<typename ... Ts>
void test(Ts ... as)
{
    for(const int& a : { as... }) {
        std::cout << a << std::endl;
    }
}


int main()
{
    using namespace boost::numeric::ublas;
    using namespace hoshi;
    using namespace std;

    //boost::numeric::ublas::matrix<double> m0(5, 3);
    //std::cout << m0 << std::endl;

    //hoshi::matrix_base<3, 3, double> m1(0, 1, 2, 3, 4, 5, 6, 7, 8);
    //hoshi::matrix_base<3, 3, double> m2(
    //    hoshi::vector3d(10, 20, 30),
    //    hoshi::vector3d(10, 20, 30),
    //    hoshi::vector3d(10, 20, 30));
    //hoshi::matrix_base<3, 3, double> m3(m1);

    //std::cout << prod(m1, m2) << std::endl;
    //std::cout << prod(m2, m1) << std::endl;
    //std::cout << (m1 * m1) << std::endl;
    //std::cout << (m1 + m2) << std::endl;

    //hoshi::matrix_base<3, 3, double> m = m1 * m1;
    //std::cout << hoshi::Demangle(typeid( v1*3 )) << std::endl;

    //matrix<double> m0(3,3), m1(2,4);
    //std::cout << prod(m1, m0) << std::endl;

    //hoshi::vector<10, double> vv1(1, 1, 2, 2, 3, 3, 4, 4, 5, 5);
    //boost::numeric::ublas::vector<double> vv2(2);
    //matrix_base<3, 3, double> mm0(1, 1, 1, 2, 2, 2, 3, 3, 3);
    //matrix_base<2, 4, double> mm1(3, 3, 3, 3, 2, 2, 2, 2);
    //boost::numeric::ublas::vector<double> vv3;
    //matrix_base<> mm2 = 30 * mm0;

    //std::cout << mm0 << std::endl;
    //std::cout << mm1 << std::endl;
    //std::cout << vv4 << std::endl;
    //std::cout << mm2 << std::endl;
    //std::cout << (3 * mm1) << std::endl;

    //cout << Demangle(typeid(mm1 * 1)) << endl;

    //matrix2x2d m0(1, 2, 3, 4);
    //matrix2x2d m1(4, 3, 2, 1);
    //matrix2x2d m2(9, 9, 9, 9);

    //matrixd m3 = m0 * m1;
    //matrixd m4 = m0 * m1 * m2;
    //matrixd m5 = (m0 * m1) * m2;
    //matrixd m6 = m0 * (m1 * m2);

    //m0.print();
    //m1.print();
    //m2.print();
    //m3.print();
    //m4.print();
    //m5.print();
    //m6.print();

    // try {
    //     matrix3x3d m0(1,-1,3,5,1,6,0,8,9);
    //     matrix3x3d m1 = m0.inverse();
    //     matrix3x3d m2 = m0 * m1;
    //     matrix3x3d m3 = m1 * m0;

    //     cout << m2 << endl;
    //     m3.print();
    // } catch(internal_logic_error& e) {
    //     cout << e.what() << endl;
    // }

    //matrix2x2d m0(1,2,3,4);
    //matrix3x3d m1;

    //m0.resize(3, 3, true);
    //m0.print();

    //m1 = m0.transpose();
    //m1.print();

    //quaternion q(1.0, 0.9, 0.8, 0.7);
    //matrix3x3d m(q);

    //quaternion q(1, 0.1, 0, 0);
    //matrix4x4d m(q);

    //q.print();
    //m.print();

    //quaternion q1(1, 1, 1, 1);
    //quaternion q2(quaternion());

    //q1.print();
    //tmp.print();
    //q2.print();

    matrix4d m;
    m.identity();
    m.print();

    return 0;
}

