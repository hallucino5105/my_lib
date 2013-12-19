#include <cstdio>
#include <string>
#include "rgb_hsv.hpp"



using namespace std;
using namespace hoshi;



int main(int argc, char* argv[])
{
    RGB rgb = { 0xFF, 0xFF, 0xFF };
    HSV hsv = rgb2hsv(rgb);

    printf("%d %d %d\n", hsv.h, hsv.s, hsv.v);

    return 0;
}


