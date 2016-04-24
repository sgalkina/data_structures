#include "memcheck.h"
#include <assert.h>
#include <stddef.h>
#include <iostream>
#include <algorithm>

size_t memcheck::counter = 0;

memcheck::memcheck()
  : value_(memcheck::CONSTRUCTOR_VALUE)
  {
    ++counter;
  }

memcheck::~memcheck()
  {
    assert(value_ != memcheck::DESTRUCTOR_VALUE && "Double free");
    assert(value_ == memcheck::CONSTRUCTOR_VALUE && "Not initialized memory");
    value_ = memcheck::DESTRUCTOR_VALUE;
    --counter;
  }

memcheck::memcheck(memcheck && other)
  : value_(memcheck::CONSTRUCTOR_VALUE)
  {
    swap(other);
    ++counter;
  }

memcheck::memcheck(memcheck const & other)
  : value_(other.value_)
  {
    ++counter;
  }

memcheck& memcheck::operator=(memcheck other) {
  swap(other);
  return *this;
}

void memcheck::swap(memcheck & other) {
  std::swap(value_, other.value_);
}

size_t memcheck::get_counter() {
  return counter;
}
