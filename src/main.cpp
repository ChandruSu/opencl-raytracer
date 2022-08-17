
#include <iostream>

#include "compute/compute.h"

using namespace sunstorm;

int main(int argc, char const *argv[])
{
  try
  {
    cmp::ComputeHandler handler = cmp::ComputeHandler();
    handler.createQueue(NULL);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  

  std::cout << "Program has ended successfully!" << std::endl;
  return 0;
}
