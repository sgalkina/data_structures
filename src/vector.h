#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <memory>

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
  vector(size_t n);
  T &get_by_index(size_t index);
  void reallocate();

  size_t capacity_;
  T * array_;
  size_t size_;
};

template <typename T>
vector<T>::vector()
  : capacity_(0)
  , array_(nullptr)
  , size_(0)
{}

template <typename T>
vector<T>::vector(size_t n)
  : capacity_(n)
  , array_()
  , size_(0)
{
  array_ = reinterpret_cast<T*>(new char[capacity_*sizeof(T)]);
}

template <typename T>
vector<T>::vector(vector const & other)
  : capacity_(other.capacity_)
  , array_()
  , size_(other.size_)
{
  array_ = reinterpret_cast<T *>(new char[capacity_*sizeof(T)]);
  for (size_t i = 0; i < size_; ++i) {
    new(reinterpret_cast<void *>(array_ + i)) T(other.array_[i]);
  }
}

template <typename T>
void vector<T>::swap(vector &other) {
  std::swap(capacity_, other.capacity_);
  std::swap(array_, other.array_);
  std::swap(size_, other.size_);
}

template <typename T>
vector<T>::vector(vector && other)
  : capacity_()
  , array_()
  , size_()
{
  swap(other);
}

template <typename T>
vector<T> & vector<T>::operator=(vector const & other) {
  capacity_ = other.capacity_;
  for (size_t i = 0; i < size_; ++i) {
    array_[i].~T();
  }
  array_ = reinterpret_cast<T *>(new char[capacity_*sizeof(T)]);
  size_ = other.size_;
  for (size_t i = 0; i < size_; ++i) {
    new(reinterpret_cast<void *>(array_ + i)) T(other.array_[i]);
  }
  return *this;
}

template <typename T>
vector<T> & vector<T>::operator=(vector && other) {
  swap(other);
  return *this;
}

template <typename T>
vector<T> vector<T>::reserve(size_t n) {
  vector<T> vect(n);
  return vect;
}

template <typename T>
vector<T>::~vector()
{
  for (size_t i = 0; i < size_; ++i) {
    array_[i].~T();
  }
  delete[] reinterpret_cast<char *>(array_);
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
void vector<T>::reallocate() {
  capacity_ = capacity_ > 0 ? capacity_ * 2 : 1;
  T * new_array = reinterpret_cast<T *>(new char[capacity_*sizeof(T)]);
  for (size_t i = 0; i < size_; ++i) {
    new(reinterpret_cast<void *>(new_array + i)) T(std::move(array_[i]));
  }
  for (size_t i = 0; i < size_; ++i) {
    array_[i].~T();
  }
  delete[] reinterpret_cast<char *>(array_);
  array_ = new_array;
}

template <typename T>
void vector<T>::push_back(T const & el) {
  if (size_ == capacity_) {
    reallocate();
  }
  new(reinterpret_cast<void *>(array_ + size_++)) T(el);
}

template <typename T>
void vector<T>::push_back(T && el) {
  if (size_ == capacity_) {
    reallocate();
  }
  new(reinterpret_cast<void *>(array_ + size_++)) T(std::move(el));
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
T & vector<T>::operator[](size_t index) {
  return get_by_index(index);
}

template <typename T>
T vector<T>::pop_back() {
  T last_element = std::move(array_[--size_]);
  array_[size_].~T();
  return last_element;
}

}  // namespace gtl
