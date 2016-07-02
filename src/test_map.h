#pragma once
#include <iostream>
#include <assert.h>
#include <stdlib.h>

namespace gtl {

/*
 * Smoketest class for map implementations
 */
template <typename T> struct smoketest_map {
  void smoketest();
  void value_semantics();
};

template <typename T>
void smoketest_map<T>::smoketest() {
  T map;
  assert(map.size() == 0);
  assert(!map.contains_key(1));
  assert(map.add(1, 100));
  assert(map.size() == 1);
  assert(map.contains_key(1));
  int * value = map.lookup(1);
  assert(*value == 100);
  assert(!map.add(1, 200));
  int * new_value = map.lookup(1);
  assert(*new_value == 200);
  assert(map.remove(1));
  assert(!map.remove(1));
  assert(map.size() == 0);
  int * empty_value = map.lookup(1);
  assert(empty_value == nullptr);
}

template <typename T>
void value_semantics_memory_test(T & obj) {
  /*
   * Memcheck type has to be used in order to run this test
   */
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

template <typename T>
void smoketest_map<T>::value_semantics() {
  T vect_map;
  size_t n = 100;
  for (size_t i = 1; i <= n; ++i) {
    memcheck el;
    vect_map.add(i, el);
  }
  value_semantics_memory_test(vect_map);
}

/*
 * Comparison test class for map implementations
 */
template <typename T, typename U> struct map_comparison_test {
  void compare_add();
  void compare_remove();
};

template <typename T, typename U>
void map_comparison_test<T, U>::compare_add() {
  T map1;
  U map2;
  int MAX = 10;
  for (size_t i = 0; i < MAX*MAX; ++i) {
    int element = rand() % MAX;
    assert(map1.add(element, element) == map2.add(element, element));
    assert(map1.add(element, element*2) == map2.add(element, element*2));
  }
}

template <typename T, typename U>
void map_comparison_test<T, U>::compare_remove() {
  T map1;
  U map2;
  int MAX = 10;
  for (size_t i = 0; i < MAX*MAX; ++i) {
    int element = rand() % MAX;
    assert(map1.remove(element) == map2.remove(element));
  }
}

}
