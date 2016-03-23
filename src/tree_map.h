#include <string>

namespace gtl {
  /*
   * A leaft leaning red-black tree
   * https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf
   */
  template <typename K, typename V> struct tree_map {
    // TODO:

    // A debug representation, suitable for displaying with http://www.graphviz.org
    std::string dot_graph() const;
  };
}
