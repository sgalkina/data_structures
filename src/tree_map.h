#include <string>
#include <sstream>

namespace gtl {
  /*
   * A leaft leaning red-black tree
   * https://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf
   */
  template <typename K, typename V> struct tree_map {
    tree_map();
    ~tree_map();

    tree_map(tree_map &&other) = delete;
    tree_map &operator=(tree_map other) = delete;

    bool add(K key, V value);

    // A debug representation, suitable for displaying with http://www.graphviz.org
    std::string dot_graph() const;

  private:
    struct Node {
      Node(K const & key, V const & value);
      ~Node();
      Node(Node &&other) = delete;

      Node &operator=(Node other) = delete;

      bool add(K new_key, V new_value);

      std::string dot_graph() const;

      K key;
      V value;
      Node * left;
      Node * right;
    };

    Node * root_;
  };

template <typename K, typename V>
tree_map<K, V>::Node::Node(K const & key, V const & value)
  : key(key)
  , value(value)
  , left(nullptr)
  , right(nullptr)
{}

template <typename K, typename V>
tree_map<K, V>::Node::~Node()
{
  delete left;
  delete right;
}

template <typename K, typename V>
std::string tree_map<K, V>::Node::dot_graph() const {
  std::stringstream result;
  bool no_children = !left && !right;
  if (no_children) {
    result << key << ";";
    return result.str();
  }
  if (left) {
    result << key << " -> " << left->key << ";" << std::endl;
    result << left->dot_graph();
  }
  if (right) {
    result << key << " -> " << right->key << ";" << std::endl;
    result << right->dot_graph();
  }
  return result.str();
}

template <typename K, typename V>
bool tree_map<K, V>::Node::add(K new_key, V new_value) {
  if (new_key == key) {
    key = new_key;
    value = new_value;
    return false;
  }
  Node * next = new_key < key ? left : right;
  if (next) return next->add(new_key, new_value);
  next = new Node(new_key, new_value);
  if (new_key < key) left = next;
  else right = next;
  return true;
}

template <typename K, typename V>
tree_map<K, V>::tree_map()
  : root_(nullptr)
{
}

template <typename K, typename V>
tree_map<K, V>::~tree_map()
{
  delete root_;
}

template <typename K, typename V>
std::string tree_map<K, V>::dot_graph() const {
  std::string result = "digraph tree_map {";
  if (root_) result += root_->dot_graph();
  return result + "}";
}

template <typename K, typename V>
bool tree_map<K, V>::add(K key, V value) {
  if (root_ == nullptr) {
    root_ = new Node(key, value);
    return true;
  }
  return root_->add(key, value);
}

}
