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
  const gtl::vector<memcheck> vect2(vect);
  memcheck by_index = vect2[0];
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

void vector_removal_operations() {
  gtl::vector<memcheck> vect;
  size_t n = 3;
  for (size_t i = 1; i <= n; ++i) {
    memcheck el;
    vect.push_back(el);
  }
  assert(memcheck::get_counter() == n);
  for (size_t i = 1; i <= n; ++i) {
    vect.pop_back();
  }
  assert(memcheck::get_counter() == 0);

  gtl::vector<int> vect_int;
  int el = 100;
  vect_int.push_back(el);
  int int_from_vect = vect_int.pop_back();
  assert(int_from_vect == el);

  vect_int.push_back(1);
  vect_int.push_back(2);
  vect_int.swap_remove(0);
  assert(vect_int.size() == 1);
  assert(vect_int[0] == 2);
}

int main(int argc, char* argv[]) {
  vector_smoketest();
  vector_reserve();
  vector_value_semantics();
  vector_removal_operations();
  std::cout << "OK" << std::endl;
  return 0;
}
