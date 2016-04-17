#include <assert.h>
#include <stddef.h>
#include <iostream>
#include "vector.h"
#include "memcheck.h"

void vector_smoketest() {
  gtl::vector<memcheck> vect;
  assert(vect.capacity() == 0);
  assert(vect.size() == 0);
  size_t n = 5;
  for (size_t i = 1; i <= n; ++i) {
    memcheck el;
    vect.push_back(el);
  }
  assert(vect[0].get_counter() == vect.capacity());
  assert(vect.capacity() == 8);
  assert(vect.size() == n);
  std::cout << "OK" << std::endl;
}

int main(int argc, char* argv[]) {
  vector_smoketest();
  return 0;
}
