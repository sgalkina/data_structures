#include <assert.h>
#include <iostream>
#include "vector.h"

void vector_smoketest() {
  gtl::vector<int> vect;
  assert(vect.capacity() == 0);
  assert(vect.size() == 0);
  vect.push_back(0);
  vect.push_back(1);
  vect.push_back(2);
  assert(vect.capacity() == 4);
  assert(vect.size() == 3);
}

int main(int argc, char* argv[]) {
  vector_smoketest();
  return 0;
}
