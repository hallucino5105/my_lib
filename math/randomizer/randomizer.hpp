#ifndef _RANDOMIZER_HPP_
#define _RANDOMIZER_HPP_



#include <functional>
#include <algorithm>
#include <random>


namespace hoshi
{


class Randomizer
{
public:
    typedef enum _RRANGE
    {
        R1 = 0,
        R2,
        R3,
    } RRANGE;
        


public:
    std::mt19937 engine;
    std::function<double (std::uniform_real_distribution<double>&)> generator;

    std::uniform_real_distribution<double> dist_range1;
    std::uniform_real_distribution<double> dist_range2;
    std::uniform_real_distribution<double> dist_range3;


public:
    Randomizer()
    {
        engine = std::mt19937(static_cast<unsigned long>(std::time(0)));

        dist_range1 = std::uniform_real_distribution<double>(0.0, 1.0);
        dist_range2 = std::uniform_real_distribution<double>(-1.0, 1.0);
        dist_range3 = std::uniform_real_distribution<double>(0.0, 90.0);
        
        generator = [&](std::uniform_real_distribution<double>& dist){ return dist(this->engine); };
    }


    double operator()(RRANGE range = RRANGE::R1)
    {
        double ret = 0.0;

        switch(range) {
            case RRANGE::R1:
                ret = generator(this->dist_range1);
                break;

            case RRANGE::R2:
                ret = generator(this->dist_range2);
                break;

            case RRANGE::R3:
                ret = generator(this->dist_range3);
                break;
        }

        return ret;
    }
};


};



#endif

