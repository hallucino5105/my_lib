#pragma once

#include <cstdio>
#include <cstdarg>
#include <string>
#include <boost/lexical_cast.hpp>


namespace hoshi
{


using namespace std;
using namespace boost;


template<typename T> 
T str2num(const string& format, ...)
{
  va_list ap;
  char* allocatedBuffer;

  va_start(ap, format);
  vasprintf(&allocatedBuffer, format.c_str(), ap);
  va_end(ap);

  T ret = lexical_cast<T>(allocatedBuffer);

  free(allocatedBuffer);

  return ret;
}


}
