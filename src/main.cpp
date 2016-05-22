#include <assert.h>
#include <stddef.h>
#include <iostream>
#include "vector.h"
#include "vector_map.h"
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

template <typename T>
void value_semantics(T & obj) {
  size_t old_counter = memcheck::get_counter();
  T obj2(obj);
  assert(memcheck::get_counter() == old_counter*2);
  T obj3(std::move(obj));
  assert(memcheck::get_counter() == old_counter*2);
  T obj4;
  obj4 = obj3;
  assert(memcheck::get_counter() == old_counter*3);
  T obj5;
  obj5 = std::move(obj3);
  assert(memcheck::get_counter() == old_counter*3);
}

void vector_value_semantics() {
  gtl::vector<memcheck> vect;
  size_t n = 100;
  for (size_t i = 1; i <= n; ++i) {
    memcheck el;
    vect.push_back(el);
  }
  value_semantics(vect);
}

void vector_map_value_semantics() {
  gtl::vector_map<int, memcheck> vect_map;
  size_t n = 100;
  for (size_t i = 1; i <= n; ++i) {
    memcheck el;
    vect_map.add(i, el);
  }
  value_semantics(vect_map);
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

void vector_map_smoketest() {
  gtl::vector_map<int, int> vect;
  assert(vect.size() == 0);
  assert(!vect.contains_key(1));
  assert(vect.add(1, 100));
  assert(vect.size() == 1);
  assert(vect.contains_key(1));
  int * value = vect.lookup(1);
  assert(*value == 100);
  assert(!vect.add(1, 200));
  int * new_value = vect.lookup(1);
  assert(*new_value == 200);
  assert(vect.remove(1));
  assert(!vect.remove(1));
  assert(vect.size() == 0);
  int * empty_value = vect.lookup(1);
  assert(!bool(empty_value));
}

void test_vector() {
  vector_smoketest();
  vector_reserve();
  vector_value_semantics();
  vector_removal_operations();
  std::cout << "Vector OK" << std::endl;
}

void test_vector_map() {
  vector_map_smoketest();
  vector_map_value_semantics();
  std::cout << "Vector Map OK" << std::endl;
}

int main(int argc, char* argv[]) {
  test_vector();
  test_vector_map();
  return 0;
}
