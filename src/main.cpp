#include <assert.h>
#include <stddef.h>
#include <iostream>
#include "vector.h"
#include "memcheck.h"

void vector_reservetest() {
  auto vect = gtl::vector<memcheck>::reserve(3);
  assert(memcheck::get_counter() == 0);
}

void vector_smoketest() {
  gtl::vector<memcheck> vect;
  assert(vect.capacity() == 0);
  assert(vect.size() == 0);
  size_t n = 20;
  for (size_t i = 1; i <= n; ++i) {
    memcheck el;
    vect.push_back(el);
  }
  assert(memcheck::get_counter() == vect.size());
  assert(vect.size() == n);
  std::cout << "OK" << std::endl;
}

int main(int argc, char* argv[]) {
  vector_smoketest();
  vector_reservetest();
  return 0;
}
