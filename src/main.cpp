#include <assert.h>
#include <stddef.h>
#include <iostream>
#include "vector.h"
#include "vector_map.h"
#include "hash_map.h"
#include "memcheck.h"
#include "test_map.h"

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
  gtl::value_semantics_memory_test(vect);
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

void test_vector() {
  vector_smoketest();
  vector_reserve();
  vector_value_semantics();
  vector_removal_operations();
  std::cout << "Vector OK" << std::endl;
}

void test_vector_map() {
  gtl::smoketest_map< gtl::vector_map<int, int> > test;
  test.smoketest();
  gtl::smoketest_map< gtl::vector_map<int, memcheck> > test_value;
  test_value.value_semantics();
  std::cout << "Vector Map OK" << std::endl;
}

void map_comparison() {
  gtl::map_comparison_test< gtl::vector_map<int, int>, gtl::vector_map<int, int> > test;
  test.compare_add();
  test.compare_remove();
  std::cout << "vector_map vs vector_map OK" << std::endl;
  gtl::map_comparison_test< gtl::vector_map<int, int>, gtl::hash_map<int, int> > test_diff;
  test_diff.compare_add();
  std::cout << "vector_map vs hash_map OK" << std::endl;
}

int main(int argc, char* argv[]) {
  test_vector();
  test_vector_map();
  map_comparison();
  return 0;
}
