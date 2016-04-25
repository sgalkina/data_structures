#include <assert.h>
#include <stddef.h>
#include <iostream>
#include "vector.h"
#include "memcheck.h"

void vector_reserve() {
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
    vect.push_back(std::move(el));
  }
  assert(memcheck::get_counter() == vect.size());
  assert(vect.size() == n);
  memcheck el;
  vect[10] = el;
  assert(vect[vect.size() - 1].get_counter() == vect.size() + 1);
}

void vector_value_semantics() {
  gtl::vector<memcheck> vect;
  size_t n = 100;
  for (size_t i = 1; i <= n; ++i) {
    memcheck el;
    vect.push_back(el);
  }
  size_t old_counter = memcheck::get_counter();
  gtl::vector<memcheck> vect2(vect);
  assert(memcheck::get_counter() == old_counter*2);
  gtl::vector<memcheck> vect3(std::move(vect));
  assert(memcheck::get_counter() == old_counter*2);
  gtl::vector<memcheck> vect4;
  vect4 = vect3;
  assert(memcheck::get_counter() == old_counter*3);
  vect4 = std::move(vect3);
  assert(memcheck::get_counter() == old_counter*3);
}

int main(int argc, char* argv[]) {
  vector_smoketest();
  vector_reserve();
  vector_value_semantics();
  std::cout << "OK" << std::endl;
  return 0;
}
