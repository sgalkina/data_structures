#include <stddef.h>

namespace gtl {
  template <typename K, typename V> struct vector_map {
    // TODO: special function prototypes

    size_t size() const;

    bool add(K const &key, V const& value);
    bool remove(K const &key);

    V const* lookup(K const &key) const;
    V * lookup(K const &key);

    bool contains_key(K const &key) const;

  private:
    // TODO:
  };
}
