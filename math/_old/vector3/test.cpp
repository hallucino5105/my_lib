#include <cstdio>
#include <iostream>
#include "vector3.hpp"


using namespace std;
using namespace hoshi;


int main()
{
    Vector3 v1(1.0, 2.0, 3.0);
    Vector3 v2 = 3 * v1;
    Vector3 v3 = v1 * v2;
    Vector3 v4 = v1;
    Vector3 v5 = v1, v6 = v1;

    v1.print();
    v2.print();
    v3.print();
    
    std::printf("%f\n", v4.normalize().norm());

    std::printf("%f\n", v5.dotProduct(v6));

    std::printf("%f\n", v5.crossProduct(v6).norm());

    std::printf("%f\n", v1.distance(v2));

    return 0;
}

