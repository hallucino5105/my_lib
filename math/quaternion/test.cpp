#include <cstdio>
#include <iostream>
#include <quaternion/quaternion.hpp>



using namespace std;
using namespace hoshi;


int main()
{
    //hoshi::matrix4x4d m(
    //    1.00,     0,     0,    0,
    //       0,  0.95, +0.01,    0,
    //       0, -0.01,  0.95,    0,
    //       0,     0,     0, 1.00);

    //hoshi::quaternion q(m);
    //hoshi::matrix4x4d n(q);

    //m.print();
    //q.print();
    //n.print();


    quaternion q1(1, 2, 3, 4);
    quaternion q2(2, 2, 2, 2);

    (q1 * q2).print();

    q1.normalize();
    q2.normalize();

    (q1 * q2).print();

    return 0;
}

