#include <cstdio>
#include <string>
#include <iostream>
#include <type_traits>


using namespace std;


extern void* enabler;


template<typename T>
struct AAA
{
    template<
        typename U = T,
        typename std::enable_if<!std::is_same<U, int>::value>::type*& = enabler>
    static void BBB()
    {
        cout << "a" << endl;
    }

    template<
        typename U = T,
        typename std::enable_if<std::is_same<U, int>::value>::type*& = enabler>
    static void BBB()
    {
        cout << "b" << endl;
    }
};


template<typename T = double>
void test()
{
    if(std::is_same<T, int>::value) {
        cout << "a" << endl;
    } else {
        cout << "b" << endl;
    }
}


int main(int argc, char* argv[])
{
    AAA<double>::BBB();

    return 0;
}


