
#ifndef __PROGTEST__

#include <cassert>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>

#endif

// Structure representing a node in the AVL tree
template<typename Product>
struct CNode {
public:
    std::shared_ptr<CNode> left; // Pointer to the left child
    std::shared_ptr<CNode> right; // Pointer to the right child
    std::vector<Product> products; // List of products in this node
    size_t height; // Height of the node in the AVL tree
    size_t amount{}; // Amount of the product (used for ranking)
    size_t children_cnt; // Count of child nodes
    size_t children_product_cnt; // Count of products in child nodes

    // Constructor for CNode
    CNode() : left(nullptr), right(nullptr), height(1), amount(0), children_cnt(0), children_product_cnt(0) {}
};

// Structure representing the Bestsellers management system using an AVL tree
template<typename Product>
struct Bestsellers {
    Bestsellers() : root(nullptr) {}

    std::unordered_map<Product, size_t> product_mapping; // Mapping from product to its amount
    std::shared_ptr<CNode<Product>> root; // Root of the AVL tree

    /**
     * Function to get the maximum of two size_t values.
     * @param a First value
     * @param b Second value
     * @return The maximum value
     */
    size_t getMax(const size_t a, const size_t b) const {
        return (a > b) ? a : b;
    }

    /**
     * Function to get the size of the products vector in a node.
     * @param node Pointer to the node
     * @return Size of the products vector
     */
    size_t getProductsSize(const std::shared_ptr<CNode<Product>> &node) const {
        if (node == nullptr)
            return 0;
        return node->products.size();
    }

    /**
     * Function to get the height of a node in the AVL tree.
     * @param node Pointer to the node
     * @return Height of the node
     */
    size_t getHeight(const std::shared_ptr<CNode<Product>> &node) const {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    /**
     * Function to get the total product count in the child nodes of a node.
     * @param node Pointer to the node
     * @return Total product count in the child nodes
     */
    size_t getChildrenProductCnt(const std::shared_ptr<CNode<Product>> &node) const {
        if (node == nullptr)
            return 0;
        return node->children_product_cnt + (node->products.size() * node->amount);
    }

    /**
     * Function to get the balance factor (alpha) of a node in the AVL tree.
     * @param node Pointer to the node
     * @return Balance factor (height difference between left and right children)
     */
    int getAlpha(const std::shared_ptr<CNode<Product>> &node) const {
        if (node == nullptr)
            return 0;
        return getHeight(node->right) - getHeight(node->left);
    }

    /**
     * Function to get the total number of child nodes for a given node.
     * @param node Pointer to the node
     * @return Total number of child nodes including the node itself
     */
    size_t getChildren(const std::shared_ptr<CNode<Product>> &node) const {
        if (node == nullptr)
            return 0;
        return node->children_cnt + 1; // +1 because we need to count the node itself
    }

    /**
     * Function to get the total number of child nodes in both the left and right subtrees.
     * @param node Pointer to the node
     * @return Total number of child nodes in both subtrees
     */
    size_t getBothChildren(const std::shared_ptr<CNode<Product>> &node) const {
        if (node == nullptr)
            return 0;
        return getChildren(node->left) + getChildren(node->right);
    }

    /**
     * Function to find the inorder successor of a node in the AVL tree.
     * @param node Pointer to the node
     * @return Pointer to the inorder successor node
     */
    std::shared_ptr<CNode<Product>> findInorderSuccessor(const std::shared_ptr<CNode<Product>> &node) {
        std::shared_ptr<CNode<Product>> tmp = node;
        while (tmp->left != nullptr)
            tmp = tmp->left;
        return tmp;
    }

    /**
     * Function to create a new node in the AVL tree.
     * @param product The product to store in the node
     * @param amount The amount of the product
     * @return Pointer to the newly created node
     */
    std::shared_ptr<CNode<Product>> newNode(Product product, size_t amount) {
        std::shared_ptr<CNode<Product>> node = std::make_shared<CNode<Product>>();
        node->height = 1;
        node->products.push_back(product);
        node->amount = amount;
        node->children_cnt = 0;
        node->children_product_cnt = 0;
        node->right = nullptr;
        node->left = nullptr;
        return node;
    }

    /**
     * Function to perform a right rotation in the AVL tree.
     * @param node Pointer to the node to rotate
     * @return Pointer to the new root after rotation
     */
    std::shared_ptr<CNode<Product>> rightRotate(const std::shared_ptr<CNode<Product>> &node) {
        std::shared_ptr<CNode<Product>> node_left = node->left;
        std::shared_ptr<CNode<Product>> node_left_right = node_left->right;

        node_left->right = node;
        node->left = node_left_right;

        node->height = getMax(getHeight(node->left), getHeight(node->right)) + 1;
        node_left->height = getMax(getHeight(node_left->left), getHeight(node_left->right)) + 1;

        node->children_cnt = getBothChildren(node) + node->products.size() - 1;
        node_left->children_cnt = getBothChildren(node_left) + node_left->products.size() - 1;

        node->children_product_cnt = getChildrenProductCnt(node->left) + getChildrenProductCnt(node->right);
        node_left->children_product_cnt = getChildrenProductCnt(node_left->left) + getChildrenProductCnt(node_left->right);

        return node_left; // Return the new root after rotation
    }

    /**
     * Function to perform a left rotation in the AVL tree.
     * @param node Pointer to the node to rotate
     * @return Pointer to the new root after rotation
     */
    std::shared_ptr<CNode<Product>> leftRotate(const std::shared_ptr<CNode<Product>> &node) {
        std::shared_ptr<CNode<Product>> node_right = node->right;
        std::shared_ptr<CNode<Product>> node_right_left = node_right->left;

        node_right->left = node;
        node->right = node_right_left;

        node->height = getMax(getHeight(node->left), getHeight(node->right)) + 1;
        node_right->height = getMax(getHeight(node_right->left), getHeight(node_right->right)) + 1;

        node->children_cnt = getBothChildren(node) + node->products.size() - 1;
        node_right->children_cnt = getBothChildren(node_right) + node_right->products.size() - 1;

        node->children_product_cnt = getChildrenProductCnt(node->left) + getChildrenProductCnt(node->right);
        node_right->children_product_cnt = getChildrenProductCnt(node_right->left) + getChildrenProductCnt(node_right->right);

        return node_right; // Return the new root after rotation
    }

    /**
     * Function to insert a new node into the AVL tree.
     * @param node Pointer to the current node in the AVL tree
     * @param product The product to insert
     * @param amount The amount of the product
     * @return Pointer to the updated node after insertion
     */
    std::shared_ptr<CNode<Product>> insertNode(std::shared_ptr<CNode<Product>> &node, const Product &product, size_t amount) {
        // Base case: if the node is null, create a new node
        if (node == nullptr) {
            product_mapping.emplace(product, amount);
            return newNode(product, amount);
        }

        // Insert the product based on the amount
        if (amount > node->amount)
            node->right = insertNode(node->right, product, amount);
        else if (amount < node->amount)
            node->left = insertNode(node->left, product, amount);
        else { // If the amount is equal, handle the product in the current node
            auto it = std::find(node->products.begin(), node->products.end(), product);
            if (it == node->products.end()) {
                product_mapping.emplace(product, amount);
                node->products.push_back(product);
                node->children_cnt++;
            } else {
                node->products.erase(it);
            }
        }

        // Update the height and child counts
        node->height = getMax(getHeight(node->left), getHeight(node->right)) + 1;
        node->children_cnt = getBothChildren(node) + node->products.size() - 1;
        node->children_product_cnt = getChildrenProductCnt(node->left) + getChildrenProductCnt(node->right);

        // Check and correct AVL tree balance
        int avl_balance = getAlpha(node);

        if (avl_balance < -1 && node->left->amount > amount)
            return rightRotate(node);
        else if (avl_balance < -1 && node->left->amount < amount) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        else if (avl_balance > 1 && node->right->amount < amount)
            return leftRotate(node);
        else if (avl_balance > 1 && node->right->amount > amount) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    /**
     * Function to delete a node from the AVL tree.
     * @param node Pointer to the current node in the AVL tree
     * @param product The product to delete
     * @param amount The amount of the product
     * @param delete_full Flag to indicate whether to delete the full node
     * @param delete_containers Flag to indicate whether to delete the container of products
     * @return Pointer to the updated node after deletion
     */
    std::shared_ptr<CNode<Product>> deleteNode(std::shared_ptr<CNode<Product>> &node, Product product, size_t amount, bool delete_full,
                                               bool delete_containers) {
        // If the tree is empty or the product is not found
        if (node == nullptr)
            return node;

        // Recursive call to find the node to delete
        if (amount > node->amount)
            node->right = deleteNode(node->right, product, amount, delete_full, delete_containers);
        else if (amount < node->amount)
            node->left = deleteNode(node->left, product, amount, delete_full, delete_containers);
        else { // If the node is found
            if (node->products.size() == 1 || delete_full) {
                if (node->right == nullptr && node->left == nullptr) { // No children
                    for (const auto &item: node->products)
                        if (delete_containers)
                            product_mapping.erase(item);
                    node = nullptr;
                    return node;
                }
                else if (node->right == nullptr) { // One left child
                    for (const auto &item: node->products)
                        if (delete_containers)
                            product_mapping.erase(item);
                    std::shared_ptr<CNode<Product>> tmp = node->left;
                    *node = *tmp;
                } else if (node->left == nullptr) { // One right child
                    for (const auto &item: node->products)
                        if (delete_containers)
                            product_mapping.erase(item);
                    std::shared_ptr<CNode<Product>> tmp = node->right;
                    *node = *tmp;
                } else { // Two children, find inorder successor
                    for (const auto &item: node->products)
                        if (delete_containers)
                            product_mapping.erase(item);
                    std::shared_ptr<CNode<Product>> inorder_successor = findInorderSuccessor(node->right);
                    node->amount = inorder_successor->amount;
                    node->products = inorder_successor->products;
                    node->right = deleteNode(node->right, *inorder_successor->products.begin(),
                                             node->amount, true, false);
                }
            } else { // Handle partial deletion in the node
                node->products.erase(std::find(node->products.begin(), node->products.end(), product));
                if (delete_containers)
                    product_mapping.erase(product);
            }
        }

        // Update height and child counts
        node->height = getMax(getHeight(node->left), getHeight(node->right)) + 1;
        node->children_cnt = getBothChildren(node) + node->products.size() - 1;
        node->children_product_cnt = getChildrenProductCnt(node->left) + getChildrenProductCnt(node->right);

        // Check and correct AVL tree balance
        int avl_balance = getAlpha(node);

        if (avl_balance < -1 && (getAlpha(node->left) <= 0))
            return rightRotate(node);
        else if (avl_balance < -1 && (getAlpha(node->left) > 0)) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        else if (avl_balance > 1 && (getAlpha(node->right) >= 0))
            return leftRotate(node);
        else if (avl_balance > 1 && (getAlpha(node->right) < 0)) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    /**
     * Function to get the total number of tracked products.
     * @return The total number of products
     */
    size_t products() const {
        return product_mapping.size();
    }

    /**
     * Function to record a sale of a product, updating its amount in the AVL tree.
     * @param p The product being sold
     * @param amount The amount sold
     */
    void sell(const Product &p, size_t amount) {
        if (product_mapping.count(p)) {
            size_t tmp_amount = product_mapping.at(p);
            root = deleteNode(root, p, product_mapping.at(p), false, true);
            root = insertNode(root, p, amount + tmp_amount);
        } else
            root = insertNode(root, p, amount);
    }

    /**
     * Function to get the rank of a product based on its sales amount.
     * @param node Pointer to the current node in the AVL tree
     * @param p The product to find the rank for
     * @param amount The amount of the product
     * @param max_rank Reference to store the maximum rank
     * @param find_same String indicating whether to find the first or last rank with the same sales
     * @param found_same Reference to store the rank if the product is found
     * @return The rank of the product
     */
    size_t getRank(const std::shared_ptr<CNode<Product>> &node, const Product &p, const size_t &amount,
                   size_t &max_rank, std::string find_same, size_t &found_same) const {
        if (node == nullptr)
            throw std::out_of_range("blabla");
        if (amount > node->amount) {
            getRank(node->right, p, amount, max_rank, find_same, found_same);
        } else if (amount < node->amount) {
            max_rank += getChildren(node->right) + getProductsSize(node);
            getRank(node->left, p, amount, max_rank, find_same, found_same);
        } else if (amount == node->amount) {
            if (find_same == "first_same") {
                found_same = max_rank + 1;
                return 1;
            } else if (find_same == "last_same") {
                found_same = max_rank + getProductsSize(node);
                return 1;
            }
            int right_child_count = node->right ? getChildren(node->right) : 0;
            for (size_t i = 0; i < getProductsSize(node); i++) {
                if (node->products[i] == p) {
                    max_rank += i + 1 + right_child_count;
                    return max_rank;
                }
            }
        }
        return max_rank;
    }

    /**
     * Function to get the rank of a product.
     * The most sold product has rank 1.
     * @param p The product to find the rank for
     * @return The rank of the product
     */
    size_t rank(const Product &p) const {
        if (!product_mapping.count(p))
            throw std::out_of_range("there is no product with such a name");
        size_t product_amount = 0;
        if (product_mapping.count(p))
            product_amount = product_mapping.at(p);
        size_t max_rank = 0;
        size_t rubbish_amount;
        getRank(root, p, product_amount, max_rank, "nothing", rubbish_amount);
        return max_rank;
    }

    /**
     * Helper function to retrieve the product at a given rank.
     * @param node Pointer to the current node in the AVL tree
     * @param rank The rank to retrieve
     * @param product_return Reference to store the product found
     * @param aggregate_children Reference to store the aggregate number of children
     * @param aggregate_product The product to aggregate
     * @return The product at the given rank
     */
    Product & getProduct(const std::shared_ptr<CNode<Product>> &node, size_t &rank, Product &product_return,
                         size_t &aggregate_children, const Product &aggregate_product) const {
        size_t right_children = getChildren(node->right);
        if (rank <= right_children)
            product_return = getProduct(node->right, rank, product_return, aggregate_children, aggregate_product);
        else if (rank > right_children) {
            if (rank <= right_children + getProductsSize(node)) {
                int iterate_to = rank - right_children;
                aggregate_children += getChildrenProductCnt(node->right);
                for (size_t i = 0; i < getProductsSize(node); i++) {
                    if (node->products[i] == aggregate_product)
                        aggregate_children += node->amount * (i + 1);
                }
                size_t cnt = 0;
                for (int i = 0; i < iterate_to; i++) {
                    product_return = node->products[i];
                    cnt = i;
                }
                return node->products[cnt];
            } else {
                size_t new_rank = rank - right_children - getProductsSize(node);
                aggregate_children += getChildrenProductCnt(node->right) + getProductsSize(node) * node->amount;
                product_return = getProduct(node->left, new_rank, product_return, aggregate_children,
                                            aggregate_product);
            }
        }
        return product_return;
    }

    /**
     * Function to get the product at a specific rank.
     * @param rank The rank to retrieve
     * @return The product at the given rank
     */
    const Product &product(size_t rank) const {
        if (rank > product_mapping.size() || rank < 1)
            throw std::out_of_range("rank is incorrect");
        static Product product1;
        size_t aggregate_children = 0;
        Product rubbish;
        product1 = getProduct(root, rank, product1, aggregate_children, rubbish);
        return product1;
    }

    /**
     * Function to get the number of copies sold for a product with a specific rank.
     * @param rank The rank to check
     * @return The number of copies sold
     */
    size_t sold(size_t rank) const {
        if (rank > product_mapping.size() || rank < 1)
            throw std::out_of_range("rank is incorrect");
        Product tmp = product(rank);
        return product_mapping.at(tmp);
    }

    /**
     * Function to get the total number of copies sold for products within a rank range.
     * @param from The starting rank
     * @param to The ending rank
     * @return The total number of copies sold within the range
     */
    size_t sold(size_t from, size_t to) const {
        if (from > to || from < 1 || to < 1 || from > product_mapping.size() || to > product_mapping.size())
            throw std::out_of_range("from is bigger than to");
        if (from == to)
            return sold(from);

        Product p_from = product(from);
        Product p_to = product(to);
        size_t one_to_from = 0;
        size_t one_to_to = 0;
        Product rubbish;
        getProduct(root, from, rubbish, one_to_from, p_from);
        getProduct(root, to, rubbish, one_to_to, p_to);
        return one_to_to - one_to_from + product_mapping.at(p_from);
    }

    /**
     * Function to get the first rank with the same number of copies sold as a given rank.
     * @param r The rank to check
     * @return The first rank with the same number of copies sold
     */
    size_t firstSame(size_t r) const {
        if (r > product_mapping.size() || r < 1)
            throw std::out_of_range("you cant do this");
        Product product1 = product(r);
        size_t max_rank = 0;
        size_t product1_amount = product_mapping.at(product1);
        size_t found_rank = 0;
        getRank(root, product1, product1_amount, max_rank, "first_same", found_rank);
        return found_rank;
    }

    /**
     * Function to get the last rank with the same number of copies sold as a given rank.
     * @param r The rank to check
     * @return The last rank with the same number of copies sold
     */
    size_t lastSame(size_t r) const {
        if (r > product_mapping.size() || r < 1)
            throw std::out_of_range("you cant do this");
        Product product1 = product(r);
        size_t max_rank = 0;
        size_t product1_amount = product_mapping.at(product1);
        size_t found_rank = 0;
        getRank(root, product1, product1_amount, max_rank, "last_same", found_rank);
        return found_rank;
    }

};

#ifndef __PROGTEST__

/**
 * Test case 1: Testing various functionalities of the Bestsellers class.
 */
void test1() {
    Bestsellers<std::string> T;
    T.sell("coke", 32);
    T.sell("bread", 1);
    assert(T.products() == 2);
    T.sell("ham", 2);
    T.sell("mushrooms", 12);
    T.sell("bla", 12);
    T.sell("apple", 12);
    T.sell("oi", 1);
    T.sell("nanana", 1);
    T.sell("kukuku", 1);

    assert(T.firstSame(2) == 2);
    assert(T.lastSame(2) == 4);
    assert(T.sold(2) == T.sold(4));

    assert(T.firstSame(7) == 6);
    assert(T.lastSame(7) == 9);
    assert(T.sold(7) == T.sold(6) == T.sold(8) == T.sold(9));
}

/**
 * Test case 2: Testing exceptions and error handling.
 */
void test2() {
# define CATCH(expr) \
  try { expr; assert(0); } catch (const std::out_of_range&) { assert(1); };

    Bestsellers<std::string> T;
    T.sell("coke", 32);
    T.sell("bread", 1);

    CATCH(T.rank("ham"));
    CATCH(T.product(3));
    CATCH(T.sold(0));
    CATCH(T.sold(9));
    CATCH(T.sold(0, 1));
    CATCH(T.sold(3, 2));
    CATCH(T.sold(1, 9));

#undef CATCH
}

/**
 * Main function to run the test cases.
 * @return 0 if all tests pass
 */
int main() {
    test1();
    test2();
}

#endif
