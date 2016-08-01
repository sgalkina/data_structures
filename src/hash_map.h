#include "vector.h"
#include <utility>

namespace gtl {
  static const float OVERLOAD_COEF = 0.75;

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
    size_t capacity() const;

    bool add(K const &key, V const &value);
    bool remove(K const &key);

    V const* lookup(K const &key) const;
    V * lookup(K const &key);

    bool contains_key(K const &key) const;

    void print() const;
  private:
    struct Entry {
        K key;
        V value;
        bool is_empty;
        bool continue_searching;
    };

    vector<Entry> vector_;

    size_t hash(K const & key, size_t capacity) const;
    size_t find(K const & key) const;
    bool is_overloaded() const;
    void reallocate();
    bool add_to_vector(K const &key, V value, vector<Entry> & vect);

    size_t size_;
  };


template <typename K, typename V>
hash_map<K, V>::hash_map()
  : vector_()
  , size_()
{
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
  return size_;
}

template <typename K, typename V>
size_t hash_map<K, V>::capacity() const {
  return vector_.capacity();
}

template <typename K, typename V>
size_t hash_map<K, V>::hash(K const &key, size_t capacity) const {
  if (capacity == 0) return 0;
  return key % capacity;
}

template <typename K, typename V>
size_t hash_map<K, V>::find(K const &key) const {
  size_t initial_hash = hash(key, vector_.capacity());
  for (size_t i = initial_hash; i < capacity(); ++i) {
    if (vector_[i].is_empty && !vector_[i].continue_searching) return capacity();
    if (!vector_[i].is_empty && vector_[i].key == key) return i;
  }
  return capacity();
}

template <typename K, typename V>
bool hash_map<K, V>::add_to_vector(K const &key, V value, vector<Entry> & vect) {
  size_t initial_hash = hash(key, vect.capacity());
  for (size_t i = initial_hash; i < vect.capacity() + initial_hash; ++i) {
    size_t ind = i % vect.capacity();
    if (vect[ind].is_empty && !vect[ind].continue_searching) {
      vect[ind].key = key;
      vect[ind].value = value;
      vect[ind].is_empty = false;
      vect[ind].continue_searching = false;
      return true;
    }
    if (vect[ind].key == key) {
      vect[ind].value = value;
      vect[ind].continue_searching = false;
      return false;
    }
  }
  std::cout << "EXCEPTION" << std::endl;
  return false;
}

template <typename K, typename V>
void hash_map<K, V>::reallocate() {
  vector<Entry> new_vector;
  size_t capacity = vector_.capacity() == 0 ? 1 : vector_.capacity()*2;
  new_vector.reserve(capacity);
  for (size_t i = 0; i < capacity; ++i) {
    Entry empty_entry = {K(), V(), true, false};
    new_vector.push_back(empty_entry);
  }
  for (size_t i = 0; i < vector_.capacity(); ++i) {
    if (!vector_[i].is_empty) {
      add_to_vector(vector_[i].key, vector_[i].value, new_vector);
    }
  }
  vector_ = std::move(new_vector);
}

template <typename K, typename V>
bool hash_map<K, V>::add(K const &key, V const &value) {
  if (is_overloaded()) {
    reallocate();
  }
  bool result = add_to_vector(key, value, vector_);
  if (result) size_++;
  return result;
}

template <typename K, typename V>
bool hash_map<K, V>::is_overloaded() const {
  if (capacity() == 0) return true;
  return size()/capacity() >= OVERLOAD_COEF;
}

template <typename K, typename V>
bool hash_map<K, V>::remove(K const &key) {
  size_t index = find(key);
  if (index == capacity()) return false;
  Entry empty_entry = {K(), V(), true, true};
  vector_[index] = empty_entry;
  size_--;
  return true;
}

template <typename K, typename V>
bool hash_map<K, V>::contains_key(K const &key) const {
  return find(key) < capacity();
}

template <typename K, typename V>
V const * hash_map<K, V>::lookup(K const &key) const {
  size_t index = find(key);
  if (index == capacity()) return nullptr;
  return &vector_[index].value;
}

template <typename K, typename V>
V * hash_map<K, V>::lookup(K const &key) {
  return const_cast<V *>(static_cast<const hash_map<K, V> *>(this)->lookup(key));
}


template <typename K, typename V>
void hash_map<K, V>::print() const {
  std::cout << "Capacity is " << vector_.capacity() << ", size is " << size() << std::endl;
  std::cout << "Elements:" << std::endl;
  for (size_t i = 0; i < vector_.capacity(); ++i) {
    if (vector_[i].is_empty) {
      std::cout << "<empty> " << vector_[i].continue_searching << std::endl;
    } else {
      std::cout << vector_[i].key << ": " << vector_[i].value << std::endl;
    }
  }
}

}  // namespace gtl
