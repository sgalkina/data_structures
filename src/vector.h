#include <stddef.h>

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
  // TODO:
};
}
