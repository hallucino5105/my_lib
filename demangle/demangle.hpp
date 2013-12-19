#ifndef _DEMANGLE_HPP_
#define _DEMANGLE_HPP_



#include <cxxabi.h>


namespace hoshi
{


class Demangle
{
private:
    char* realname;


public :
    Demangle(std::type_info const& ti)
    {
        int status = 0;
        realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
    }


    Demangle(Demangle const&) = delete;
    Demangle& operator=(Demangle const &) = delete;


    ~Demangle()
    {
        std::free(realname);
    }


    operator char const*() const
    {
        return realname;
    }
};


};



#endif

