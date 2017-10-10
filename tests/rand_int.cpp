#include <iostream>
#include <random>

#include "sec_random.h"

int main()
{
  SecRandom RNG;
  std::uniform_int_distribution<uint32_t> RU;
  std::cout << RU(RNG) << std::endl;
  return 0;
}
