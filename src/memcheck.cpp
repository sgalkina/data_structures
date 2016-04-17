#include "memcheck.h"
#include <assert.h>
#include <stddef.h>
#include <iostream>

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
    --counter;
  }

size_t memcheck::get_counter() const {
  return counter;
}
