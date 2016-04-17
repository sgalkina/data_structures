#include <stddef.h>
#include <algorithm>
#include <iostream>

namespace gtl {

/*
 * A "growable array" random access container.
 *
 * Allocates a contiguous buffer of `capacity` elements in memory. When capacity
 * is exceeded, a twice as large buffer is allocated, elements from the old
 * buffer are moved to the new buffer and the old buffer is deallocaed.
 */
template <typename T> struct vector {
  vector();
  vector(vector const &other);
  vector(vector &&other);
  static vector reserve(size_t n);

  void swap(vector &other);
  vector &operator=(vector const &other);
  vector &operator=(vector &&other);

  ~vector();

  size_t size() const;
  size_t capacity() const;

  void push_back(T const &value);
  void push_back(T &&value);
  /*
   * O(1) remove operation, which swaps the element to be removed with the last
   * one and then pop_backs it. Changes the order of elements in the container.
   */
  void swap_remove(size_t index);
  T pop_back();

  T const &operator[](size_t index) const;
  T &operator[](size_t index);

private:
  T &get_by_index(size_t index);

  size_t capacity_;
  T * array_;
  size_t size_;
};

template <typename T>
vector<T>::vector()
  : capacity_(0)
  , array_()
  , size_(0)
{
  array_ = new T[capacity_];
}

template <typename T>
vector<T>::~vector()
{
  delete[] array_;
}

template <typename T>
size_t vector<T>::capacity() const {
  return capacity_;
}

template <typename T>
size_t vector<T>::size() const {
  return size_;
}

template <typename T>
void vector<T>::push_back(T const &el) {
  if (size_ == capacity_) {
    capacity_ *= 2;
    if (capacity_ == 0) capacity_ = 1;
    T * new_array = new T[capacity_];
    std::copy(array_, array_ + size_ - 1, new_array);
    delete[] array_;
    array_ = new_array;
  }
  array_[size_] = el;
  ++size_;
}

template <typename T>
T &vector<T>::get_by_index(size_t index) {
  assert(index >= 0 && index < size_ && "Out of bound");
  return array_[index];
}

template <typename T>
T const & vector<T>::operator[](size_t index) const {
  return get_by_index(index);
}

template <typename T>
T &vector<T>::operator[](size_t index) {
  return get_by_index(index);
}

}  // namespace gtl
