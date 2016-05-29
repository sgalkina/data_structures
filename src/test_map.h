#pragma once

namespace gtl {

/*
 * Test class for map implementations
 */
template <typename T> struct test_map {
  void add_remove();
  void value_semantics();
};

template <typename T>
void test_map<T>::add_remove() {
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
void test_map<T>::value_semantics() {
  T vect_map;
  size_t n = 100;
  for (size_t i = 1; i <= n; ++i) {
    memcheck el;
    vect_map.add(i, el);
  }
  value_semantics_memory_test(vect_map);
}

}
