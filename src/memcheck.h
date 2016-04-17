#include <stddef.h>

struct memcheck {
  memcheck();
  ~memcheck();

  size_t get_counter() const;

private:
  static size_t counter;
  int value_;

  static const int CONSTRUCTOR_VALUE = 0xCAFEBABE;
  static const int DESTRUCTOR_VALUE = 0xDEADBEEF;
};
