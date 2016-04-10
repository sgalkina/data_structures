#include "vector.h"
#include <assert.h>
#include <iostream>

void vector_smoketest() {
  gtl::vector<int> vect;
  assert(vect.capacity() == 0);
  assert(vect.size() == 0);
}

int main(int argc, char* argv[]) {
  vector_smoketest();
  return 0;
}
