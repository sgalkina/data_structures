#include "vector.h"
#include <utility>

namespace gtl {
  template <typename K> struct default_hash {
    size_t operator() (K const & key) const { return key; }
  };

  static const float OVERLOAD_COEF = 0.75;

  /*
   * Hash table with open addressing and linear probing
   */
  template <typename K, typename V, typename H = default_hash<K>> struct hash_map {
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
        bool is_deleted;
    };
    H hasher_;
    vector<Entry> vector_;

    size_t hash(K const & key, size_t capacity) const;
    size_t find(K const & key) const;
    bool is_overloaded() const;
    void reallocate();
    size_t insertion_point(vector<Entry> const & vect, K const &key) const;
    bool add_to_vector(K const &key, V value, vector<Entry> & vect);

    size_t size_;
  };


template <typename K, typename V, typename H>
hash_map<K, V, H>::hash_map()
  : hasher_()
  , vector_()
  , size_()
{
}

template <typename K, typename V, typename H>
void hash_map<K, V, H>::swap(hash_map &other) {
  std::swap(vector_, other.vector_);
}

template <typename K, typename V, typename H>
hash_map<K, V, H>::hash_map(hash_map const &other)
  : vector_(other.vector_)
{}

template <typename K, typename V, typename H>
hash_map<K, V, H>::hash_map(hash_map && other)
  : vector_()
{
  swap(other);
}

template <typename K, typename V, typename H>
hash_map<K, V, H> & hash_map<K, V, H>::operator=(hash_map other) {
  swap(other);
  return *this;
}

template <typename K, typename V, typename H>
size_t hash_map<K, V, H>::size() const {
  return size_;
}

template <typename K, typename V, typename H>
size_t hash_map<K, V, H>::capacity() const {
  return vector_.capacity();
}

template <typename K, typename V, typename H>
size_t hash_map<K, V, H>::hash(K const &key, size_t capacity) const {
  if (capacity == 0) return 0;
  return hasher_(key) % capacity;
}

template <typename K, typename V, typename H>
size_t hash_map<K, V, H>::insertion_point(vector<Entry> const & vect, K const &key) const {
  size_t initial_hash = hash(key, vect.capacity());
  for (size_t i = initial_hash; i < vect.capacity() + initial_hash; ++i) {
    size_t ind = i % vect.capacity();
    bool is_empty = vect[ind].is_empty && !vect[ind].is_deleted;
    bool is_found = !vect[ind].is_empty && vect[ind].key == key;
    if (is_empty || is_found) return ind;
  }
  assert(false && "Insertion point not found");
}

template <typename K, typename V, typename H>
size_t hash_map<K, V, H>::find(K const &key) const {
  if (vector_.capacity() == 0) return 0;
  size_t insertion_index = insertion_point(vector_, key);
  if (vector_[insertion_index].is_empty) return capacity();
  return insertion_index;
}

template <typename K, typename V, typename H>
bool hash_map<K, V, H>::add_to_vector(K const &key, V value, vector<Entry> & vect) {
  size_t ind = insertion_point(vect, key);
  bool result = vect[ind].is_empty;
  vect[ind] = {key, value, false, false};
  return result;
}

template <typename K, typename V, typename H>
void hash_map<K, V, H>::reallocate() {
  vector<Entry> new_vector;
  size_t capacity = vector_.capacity() == 0 ? 1 : vector_.capacity()*2;
  new_vector.reserve(capacity);
  for (size_t i = 0; i < capacity; ++i) {
    new_vector.push_back({K(), V(), true, false});
  }
  for (size_t i = 0; i < vector_.capacity(); ++i) {
    if (!vector_[i].is_empty) {
      add_to_vector(vector_[i].key, vector_[i].value, new_vector);
    }
  }
  vector_ = std::move(new_vector);
}

template <typename K, typename V, typename H>
bool hash_map<K, V, H>::add(K const &key, V const &value) {
  if (is_overloaded()) {
    reallocate();
  }
  bool result = add_to_vector(key, value, vector_);
  if (result) size_++;
  return result;
}

template <typename K, typename V, typename H>
bool hash_map<K, V, H>::is_overloaded() const {
  if (capacity() == 0) return true;
  return size()/capacity() >= OVERLOAD_COEF;
}

template <typename K, typename V, typename H>
bool hash_map<K, V, H>::remove(K const &key) {
  size_t index = find(key);
  if (index == capacity()) return false;
  vector_[index] = {K(), V(), true, true};
  size_--;
  return true;
}

template <typename K, typename V, typename H>
bool hash_map<K, V, H>::contains_key(K const &key) const {
  return find(key) < capacity();
}

template <typename K, typename V, typename H>
V const * hash_map<K, V, H>::lookup(K const &key) const {
  size_t index = find(key);
  if (index == capacity()) return nullptr;
  return &vector_[index].value;
}

template <typename K, typename V, typename H>
V * hash_map<K, V, H>::lookup(K const &key) {
  return const_cast<V *>(static_cast<const hash_map<K, V, H> *>(this)->lookup(key));
}


template <typename K, typename V, typename H>
void hash_map<K, V, H>::print() const {
  std::cout << "Capacity is " << vector_.capacity() << ", size is " << size() << std::endl;
  std::cout << "Elements:" << std::endl;
  for (size_t i = 0; i < vector_.capacity(); ++i) {
    if (vector_[i].is_empty) {
      std::cout << "<empty> " << vector_[i].is_deleted << std::endl;
    } else {
      std::cout << vector_[i].key << ": " << vector_[i].value << std::endl;
    }
  }
}

}  // namespace gtl
