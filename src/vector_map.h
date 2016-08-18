#pragma once
#include <stddef.h>
#include "vector.h"

namespace gtl {

template <typename K, typename V> struct vector_map {
  vector_map();
  vector_map(vector_map const &other);
  vector_map(vector_map &&other);

  void swap(vector_map &other);
  vector_map &operator=(vector_map other);

  size_t size() const;
  size_t capacity() const;

  bool add(K const &key, V value);
  bool remove(K const &key);

  V const* lookup(K const &key) const;
  V * lookup(K const &key);

  bool contains_key(K const &key) const;

  void trace() const;
private:
  size_t find(K const & key) const;

  struct Entry {
      K key;
      V value;
  };

  vector<Entry> vector_;
};

template <typename K, typename V>
vector_map<K, V>::vector_map()
  : vector_()
{}

template <typename K, typename V>
void vector_map<K, V>::swap(vector_map &other) {
  std::swap(vector_, other.vector_);
}

template <typename K, typename V>
vector_map<K, V>::vector_map(vector_map const &other)
  : vector_(other.vector_)
{}

template <typename K, typename V>
vector_map<K, V>::vector_map(vector_map && other)
  : vector_()
{
  swap(other);
}

template <typename K, typename V>
vector_map<K, V> & vector_map<K, V>::operator=(vector_map other) {
  swap(other);
  return *this;
}

template <typename K, typename V>
size_t vector_map<K, V>::size() const {
  return vector_.size();
}

template <typename K, typename V>
size_t vector_map<K, V>::capacity() const {
  return vector_.capacity();
}

template <typename K, typename V>
size_t vector_map<K, V>::find(K const &key) const {
  for (size_t i = 0; i < size(); ++i) {
    if (vector_[i].key == key) return i;
  }
  return size();
}

template <typename K, typename V>
bool vector_map<K, V>::contains_key(K const &key) const {
  return find(key) < size();
}

template <typename K, typename V>
V const * vector_map<K, V>::lookup(K const &key) const {
  size_t index = find(key);
  if (index == size()) return nullptr;
  return &vector_[index].value;
}

template <typename K, typename V>
V * vector_map<K, V>::lookup(K const &key) {
  return const_cast<V *>(static_cast<const vector_map<K, V> *>(this)->lookup(key));
}

template <typename K, typename V>
bool vector_map<K, V>::add(K const &key, V value) {
  V * old_value = lookup(key);
  if (!bool(old_value)) {
    Entry new_entry = {key, value};
    vector_.push_back(new_entry);
    return true;
  }
  *old_value = value;
  return false;
}

template <typename K, typename V>
bool vector_map<K, V>::remove(K const &key) {
  size_t index = find(key);
  if (index == size()) return false;
  vector_.swap_remove(index);
  return true;
}

template <typename K, typename V>
void vector_map<K, V>::trace() const {
  std::cout << "Capacity is " << vector_.capacity() << ", size is " << vector_.size() << std::endl;
  std::cout << "Elements:" << std::endl;
  for (size_t i = 0; i < vector_.size(); ++i) {
    std::cout << vector_[i].key << ": " << vector_[i].value << std::endl;
  }
}

}
