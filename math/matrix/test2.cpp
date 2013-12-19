#include <cstdio>
#include <iostream>
#include <boost/numeric/ublas/io.hpp>
#include <demangle/demangle.hpp>
#include <quaternion/quaternion.hpp>
#include "matrix.hpp"



using namespace boost::numeric::ublas;
using namespace hoshi;
using namespace std;



int main()
{
    //quaternion q(0.1, 0.2, 0.3, 0.4);
    //q.normalize();
    //q.print();
    
    quaternion(0.1, 0.2, 0.3, 0.4).normalized().print();

    quaternion p(1, 2, 3, 4);
    quaternion q(p.conjugated());
    q.print();
    cout << Demangle( typeid(q) ) << endl;
    
    return 0;
}

