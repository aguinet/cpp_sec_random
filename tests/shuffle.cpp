#include <iostream>
#include <algorithm>
#include <random>

#include "sec_random.h"

int main()
{
  uint8_t S[] = {0,1,2,3,4,5,6};
  SecRandom RNG;
  std::shuffle(std::begin(S), std::end(S), RNG);
  for (uint8_t C: S) {
    std::cout << (unsigned int) C << ",";
  }
  std::cout << std::endl;
  return 0;
}

