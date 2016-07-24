#pragma once
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
  vector &operator=(vector other);

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

  void print() const;

private:
  vector(size_t n);
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
vector<T> & vector<T>::operator=(vector other) {
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
  vector<T> temp(*this);
  *this = std::move(temp);
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
T const & vector<T>::operator[](size_t index) const {
  assert(index < size_ && "Out of bound");
  return array_[index];
}

template <typename T>
T & vector<T>::operator[](size_t index) {
  assert(index < size_ && "Out of bound");
  return array_[index];
}

template <typename T>
T vector<T>::pop_back() {
  assert(size_ != 0 && "Empty vector");
  T last_element = std::move(array_[--size_]);
  array_[size_].~T();
  return last_element;
}

template <typename T>
void vector<T>::swap_remove(size_t index) {
  assert(size_ != 0 && "Empty vector");
  assert(index < size_ && "Out of bound");
  std::swap(array_[size_ - 1], array_[index]);
  pop_back();
}

template <typename K>
void vector<K>::print() const {
  std::cout << "Vector capacity is " << capacity() << ", size is " << size() << std::endl;
  std::cout << "Vector elements:" << std::endl;
  for (size_t i = 0; i < capacity(); ++i) {
    if (array_[i].is_empty) {
      std::cout << "<empty>" << std::endl;
    } else {
      std::cout << array_[i].key << ": " << array_[i].value << std::endl;
    }
  }
}

}  // namespace gtl
