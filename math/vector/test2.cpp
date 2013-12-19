#include <cstdio>
#include <iostream>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/expression_types.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <demangle/demangle.hpp>
#include "vector.hpp"



using namespace std;
using namespace hoshi;



void test(vector3d v)
{
}


int main()
{
    //vector3d v1(1, 2, 3);
    //vector3d v2 = v1.normalized();

    //cout << v2.x << endl;
    //cout << v2.y << endl;
    //cout << v2.z << endl;

    //v2.print();


    //test(std::move( vector3d(0.0, 0.0, 0.0) ));


    vector3d v1(0.1, 0.2, 0.3);
    31.normalize();
    v1.print();

    return 0;
}

