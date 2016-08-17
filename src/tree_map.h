#include <string>
#include <sstream>
#include <fstream>

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
    void trace();

    void delete_min();
    void remove(K const & key);

    // A debug representation, suitable for displaying with http://www.graphviz.org
    std::string dot_graph(std::string name) const;

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
    Node * fix_up(Node * node);
    Node * delete_min(Node * node);
    Node * remove(Node * node, K const & key);
    Node * move_red_left(Node * node);
    Node * move_red_right(Node * node);
    Node * get(Node * node, K const & key);
    Node * get(K const & key);
    Node * min(Node * node) const;
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
      // std::cout << "here " << key << std::endl;
      std::string color = child->is_red ? "red" : "black";
      result << key << " -> " << child->key << "[color=\"" << color << "\"];\n";
      result << child->dot_graph();
      // std::cout << "here2" << std::endl;
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
  is_red = !is_red;
  left->is_red = !left->is_red;
  right->is_red = !right->is_red;
}

template <typename K, typename V>
bool tree_map<K, V>::is_red(Node * node) {
  return node && node->is_red;
}

template <typename K, typename V>
auto tree_map<K, V>::fix_up(Node * node) -> Node * {
  if (is_red(node->right) && !is_red(node->left)) node = node->rotate_left();
  if (is_red(node->left) && is_red(node->left->left)) node = node->rotate_right();
  if (is_red(node->left) && is_red(node->right)) node->flip_colors();
  return node;
}

template <typename K, typename V>
typename tree_map<K, V>::Node * tree_map<K, V>::add(Node * node, K key, V value) {
  if (!node) return new Node(key, value);

  if (key == node->key) node->value = value;
  else if (key < node->key) node->left = add(node->left, key, value);
  else node->right = add(node->right, key, value);

  return fix_up(node);
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
std::string tree_map<K, V>::dot_graph(std::string name) const {
  std::string result = "digraph " + name + " {";
  if (root_) result += root_->dot_graph();
  return result + "labelloc=\"t\";\nlabel=\"" + name + "\";}";
}

template <typename K, typename V>
bool tree_map<K, V>::add(K key, V value) {
  root_ = add(root_, key, value);
  root_->is_red = false;
  return true;
}

template <typename K, typename V>
void tree_map<K, V>::delete_min() {
  root_ = delete_min(root_);
  if (root_) root_->is_red = false;
}

template <typename K, typename V>
void tree_map<K, V>::remove(K const & key) {
  root_ = remove(root_, key);
  if (root_) root_->is_red = false;
}

template <typename K, typename V>
auto tree_map<K, V>::remove(Node * node, K const & key) -> Node * {
  if (!node) return nullptr;
  if (key < node->key) {
    if (!is_red(node->left) && !is_red(node->left->left)) node = move_red_left(node);
    node->left = remove(node->left, key);
  } else {
    if (is_red(node->left)) node = node->rotate_right();
    if (key == node->key && !node->right) {
      Node * temp = node->left;
      node->left = nullptr;
      delete node;
      return temp;
    }
    if (!is_red(node->right) && !is_red(node->right->left)) node = move_red_right(node);
    if (key == node->key) {
      node->value = min(node->right)->value;
      node->right = delete_min(node->right);
    } else {
      node->right = remove(node->right, key);
    }
  }
  return fix_up(node);
}

template <typename K, typename V>
auto tree_map<K, V>::get(K const & key) -> Node * {
  return get(root_, key);
}

template <typename K, typename V>
auto tree_map<K, V>::get(Node * node, K const & key) -> Node * {
  if (!node) return nullptr;
  if (node->key == key) return node;
  if (key < node->key) return get(node->left, key);
  return get(node->right, key);
}

template <typename K, typename V>
auto tree_map<K, V>::min(Node * node) const -> Node * {
  if (!node) return nullptr;
  if (!node->left) return node;
  return min(node->left);
}

template <typename K, typename V>
void tree_map<K, V>::trace() {
  static size_t i = 0;
  std::string name = "trace" + std::to_string(i++);
  std::ofstream f("bin/" + name + ".dot");
  f << dot_graph(name);
}

template <typename K, typename V>
auto tree_map<K, V>::delete_min(Node * node) -> Node * {
  if (!node) return nullptr;
  if (!node->left) {
    Node * temp = node->right;
    node->right = nullptr;
    delete node;
    return temp;
  }
  if (!is_red(node->left) && !is_red(node->left->left)) node = move_red_left(node);
  node->left = delete_min(node->left);
  return fix_up(node);
}

template <typename K, typename V>
auto tree_map<K, V>::move_red_left(Node * node) -> Node * {
  node->flip_colors();
  if (is_red(node->right->left)) {
    node->right = node->right->rotate_right();
    node = node->rotate_left();
    node->flip_colors();
  }
  return node;
}

template <typename K, typename V>
auto tree_map<K, V>::move_red_right(Node * node) -> Node * {
  node->flip_colors();
  if (is_red(node->left->left)) {
    node = node->rotate_right();
    node->flip_colors();
  }
  return node;
}

}
