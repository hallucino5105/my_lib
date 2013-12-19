#ifndef _ROUND_HPP_
#define _ROUND_HPP_



namespace hoshi
{


inline static double round(double x)
{
    return static_cast<double>(static_cast<int>(x + 0.5));
}


};



#endif

