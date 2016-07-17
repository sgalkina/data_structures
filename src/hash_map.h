#include "vector.h"

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
  private:
    struct Entry {
        K key;
        V value;
        bool is_empty;
    };

    vector<Entry> vector_;

    size_t hash(K const & key) const;
    size_t find(K const & key) const;
    bool is_overloaded() const;
    bool reallocate();
    bool add_to_vector(K const &key, V value, vector<Entry> & vect);

  };


template <typename K, typename V>
hash_map<K, V>::hash_map()
  : vector_()
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
  return vector_.size();
}

template <typename K, typename V>
size_t hash_map<K, V>::hash(K const &key) const {
  return key % capacity_;
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
bool hash_map<K, V>::add_to_vector(K const &key, V value, vector<Entry> & vect) {
  size_t initial_hash = hash(key);
  for (size_t i = initial_hash; i < vect.capacity() + initial_hash; ++i) {
    size_t ind = i % vect.capacity();
    if (vector_[ind].is_empty) {
      Entry new_entry = {key, value, false};
      vector_[ind] = new_entry;
      return true;
    }
    if (vector_[ind].key == key) return false;
  }
  return false;
}

template <typename K, typename V>
bool hash_map<K, V>::reallocate() {
  vector<Entry> new_vector;
  size_t capacity = vector_.capacity() == 0 ? 1 : vector_.capacity()*2;
  new_vector.reserve(capacity);
  for (size_t i = 0; i < vector_.capacity(); ++i) {
    if (!vector_[i].is_empty) {
      add_to_vector(vector_[i].key, vector_[i].value, new_vector);
    }
  }
  vector_ = new_vector;
}

template <typename K, typename V>
bool hash_map<K, V>::add(K const &key, V const &value) {
  if (is_overloaded()) {
    reallocate();
  }
  return add_to_vector(key, value, vector_);
}

template <typename K, typename V>
bool hash_map<K, V>::is_overloaded() const {
  return capacity_ == 0 || size_/capacity_ >= OVERLOAD_COEF;
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
