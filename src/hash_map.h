#include "vector.h"

namespace gtl {
  const size_t INITIAL_SIZE = 256;

  /*
   * Hash table with open addressing and linear probing
   */
  template <typename K, typename V> struct hash_map {
    hash_map();
    hash_map(hash_map const &other);
    hash_map(hash_map &&other);

    void swap(hash_map &other);
    hash_map &operator=(hash_map other);

    size_t size() const;

    bool add(K const &key, V value);
    bool remove(K const &key);

    V const* lookup(K const &key) const;
    V * lookup(K const &key);

    bool contains_key(K const &key) const;
  private:
    size_t hash(K const & key) const;
    size_t find(K const & key) const;

    struct Entry {
        K key;
        V value;
        bool is_empty;
    };

    vector<Entry> vector_;
  };


template <typename K, typename V>
hash_map<K, V>::hash_map()
  : vector_()
{
  for (size_t i = 0; i < INITIAL_SIZE; ++i) {
    Entry empty_entry = {K(), V(), true};
    vector_.push_back(empty_entry);
  }
}

template <typename K, typename V>
void hash_map<K, V>::swap(hash_map &other) {
  std::swap(vector_, other.vector_);
}

template <typename K, typename V>
hash_map<K, V>::hash_map(hash_map const &other)
  : vector_(other.vector_)
{}

template <typename K, typename V>
hash_map<K, V>::hash_map(hash_map && other)
  : vector_()
{
  swap(other);
}

template <typename K, typename V>
hash_map<K, V> & hash_map<K, V>::operator=(hash_map other) {
  swap(other);
  return *this;
}

template <typename K, typename V>
size_t hash_map<K, V>::size() const {
  return vector_.size();
}

template <typename K, typename V>
size_t hash_map<K, V>::hash(K const &key) const {
  return key % INITIAL_SIZE;
}

template <typename K, typename V>
size_t hash_map<K, V>::find(K const &key) const {
  size_t initial_hash = hash(key);
  for (size_t i = initial_hash; i < size(); ++i) {
    if (vector_[i].is_empty) return size();
    if (vector_[i].key == key) return i;
  }
  return size();
}

template <typename K, typename V>
bool hash_map<K, V>::add(K const &key, V value) {
  size_t initial_hash = hash(key);
  for (size_t i = initial_hash; i < size(); ++i) {
    if (vector_[i].is_empty) {
      Entry new_entry = {key, value, false};
      vector_[i] = new_entry;
      return true;
    }
    if (vector_[i].key == key) return false;
  }
  Entry new_entry = {key, value, false};
  vector_.push_back(new_entry);
  return true;
}

template <typename K, typename V>
bool hash_map<K, V>::remove(K const &key) {
  size_t index = find(key);
  if (index == size()) return false;
  Entry empty_entry = {K(), V(), true};
  vector_[index] = empty_entry;
  return true;
}

template <typename K, typename V>
bool hash_map<K, V>::contains_key(K const &key) const {
  return find(key) < size();
}

template <typename K, typename V>
V const * hash_map<K, V>::lookup(K const &key) const {
  int index = find(key);
  if (index == size()) return nullptr;
  return &vector_[index].value;
}

template <typename K, typename V>
V * hash_map<K, V>::lookup(K const &key) {
  return const_cast<V *>(static_cast<const vector_map<K, V> *>(this)->lookup(key));
}

}  // namespace gtl
