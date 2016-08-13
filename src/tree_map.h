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

      Node * rotate_right();
      Node * rotate_left();
      void flip_colors();

      std::string dot_graph() const;

      K key;
      V value;
      bool is_red;
      Node * left;
      Node * right;
    };

    Node * add(Node * node, K key, V value);
    bool is_red(Node * node);
    Node * root_;
  };

template <typename K, typename V>
tree_map<K, V>::Node::Node(K const & key, V const & value)
  : key(key)
  , value(value)
  , is_red(true)
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
  result << key << ";\n";
  auto children = {left, right};
  for (auto child : children) {
    if (child) {
      std::string color = child->is_red ? "red" : "black";
      result << key << " -> " << child->key << "[color=\"" << color << "\"];\n";
      result << child->dot_graph();
    }
  }
  return result.str();
}

template <typename K, typename V>
auto tree_map<K, V>::Node::rotate_left() -> Node * {
  Node * x = right;
  assert(x->is_red);
  x->is_red = is_red;
  is_red = true;
  right = x->left;
  x->left = this;
  return x;
}

template <typename K, typename V>
auto tree_map<K, V>::Node::rotate_right() -> Node * {
  Node * x = left;
  assert(x->is_red);
  x->is_red = is_red;
  is_red = true;
  left = x->right;
  x->right = this;
  return x;
}

template <typename K, typename V>
void tree_map<K, V>::Node::flip_colors() {
  assert(!is_red);
  assert(left->is_red);
  assert(right->is_red);
  is_red = true;
  left->is_red = false;
  right->is_red = false;
}

template <typename K, typename V>
bool tree_map<K, V>::is_red(Node * node) {
  return node && node->is_red;
}

template <typename K, typename V>
typename tree_map<K, V>::Node * tree_map<K, V>::add(Node * node, K key, V value) {
  if (!node) return new Node(key, value);

  if (is_red(node->left) && is_red(node->right)) node->flip_colors();

  if (key == node->key) node->value = value;
  else if (key < node->key) node->left = add(node->left, key, value);
  else node->right = add(node->right, key, value);

  if (is_red(node->right) && !is_red(node->left)) node = node->rotate_left();
  if (is_red(node->left) && is_red(node->left->left)) node = node->rotate_right();

  return node;
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
  root_ = add(root_, key, value);
  root_->is_red = false;
  return true;
}

}
