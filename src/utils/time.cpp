
#include "utils.h"

namespace sunstorm
{
  namespace time 
  {
    long long getTimeMicroseconds() {
      auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
      return std::chrono::duration_cast<std::chrono::microseconds>(now).count();
    }
  }
}