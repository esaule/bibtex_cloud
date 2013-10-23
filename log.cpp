#include "log.hpp"

Log Log::log;
Log Log::logerr;
Log::c Log::endl;

ostream& operator<< (ostream& o, Log::c & )
{
  Log& l = (Log&) o;
  l.commit();
  return o;
}
