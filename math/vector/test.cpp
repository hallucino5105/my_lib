#include <cstdio>
#include <iostream>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/expression_types.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <demangle/demangle.hpp>
#include "vector.hpp"


int main()
{
    boost::numeric::ublas::vector<double> vv1(3);
    boost::numeric::ublas::vector<double> vv2(3);

    for(int i = 0; i < 3; i++) {
        vv1[i] = M_PI;
        vv2[i] = 1.0;
    }


    hoshi::vector2d v1(0.9, 0.9);
    hoshi::vector2d v2(1.1, 1.1);
    hoshi::vector2d v3;

    v2 += v1;

    std::cout << v2 << std::endl;
    //std::printf("%f\n", v1.snorm());

    return 0;
}

