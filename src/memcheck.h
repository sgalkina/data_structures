#include <stddef.h>

struct memcheck {
  memcheck();
  ~memcheck();
  // memcheck(memcheck const & obj);
  memcheck(memcheck && other);
  memcheck(memcheck const & other);

  memcheck& operator=(memcheck other);

  static size_t get_counter();

private:
  void swap(memcheck & other);
  static size_t counter;
  int value_;

  static const int CONSTRUCTOR_VALUE = 0xCAFEBABE;
  static const int DESTRUCTOR_VALUE = 0xDEADBEEF;
};
