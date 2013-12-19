#include <cstdio>
#include <string>
#include <iostream>
#include <type_traits>


using namespace std;


extern void* enabler;


int x = 1;


template<typename std::enable_if<std::is_integral<int>{}>::type*& = enabler>
void func()
{
    std::cout << "integral:" << x << std::endl;
}


template<typename std::enable_if<!std::is_integral<int>{}>::type*& = enabler>
void func()
{
    std::cout << "other:" << x << std::endl;
}

int main(int argc, char* argv[])
{
    func(3.1);
    func(3);

    return 0;
}


