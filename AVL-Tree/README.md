# Bestsellers Tracker using AVL Tree

This project implements a Bestsellers tracker that maintains a list of products and the number of times each has been sold. The products are organized in an AVL tree, ensuring efficient insertion, deletion, and rank queries. The tracker supports querying the rank of a product, the number of times products have been sold, and other statistical operations.

## Introduction

This project is a C++ implementation of a Bestsellers tracker, which manages products and tracks their sales using an AVL tree. The AVL tree is a self-balancing binary search tree that ensures O(log n) time complexity for insertions, deletions, and lookups, making it an efficient structure for maintaining and querying ranked data.

## Key Structures and Concepts

### `CNode<Product>` Structure
Represents a node in the AVL tree:
- `std::shared_ptr<CNode> left, right`: Pointers to the left and right children.
- `std::vector<Product> products`: List of products with the same sale amount.
- `size_t height`: Height of the node.
- `size_t amount`: The total number of times the products in this node have been sold.
- `size_t children_cnt, children_product_cnt`: Auxiliary counters to manage the number of child nodes and their respective product counts.

### `Bestsellers<Product>` Structure
Handles operations on the AVL tree, such as insertion, deletion, and querying:
- `std::unordered_map<Product, size_t> product_mapping`: Maps products to their sale counts.
- `std::shared_ptr<CNode<Product>> root`: Root node of the AVL tree.

## Key Functions

### `void sell(const Product &p, size_t amount)`
Inserts a product into the AVL tree or updates its sale count if it already exists.

### `size_t rank(const Product &p) const`
Returns the rank of a product, where the most sold product has rank 1.

### `const Product &product(size_t rank) const`
Retrieves the product with the given rank.

### `size_t sold(size_t rank) const`
Returns the number of times the product with the given rank has been sold.

### `size_t sold(size_t from, size_t to) const`
Returns the total number of times products within a given rank range (inclusive) have been sold.

### `size_t first_same(size_t r) const` and `size_t last_same(size_t r) const`
Returns the first and last rank, respectively, that has the same number of sales as the product with rank `r`.

## Usage Example

Here’s a sample usage of the `Bestsellers` structure:

```cpp
#include <iostream>
#include "Bestsellers.h" // Include your header file

int main() {
    Bestsellers<std::string> T;
    T.sell("coke", 32);
    T.sell("bread", 1);
    T.sell("ham", 2);
    T.sell("mushrooms", 12);
    T.sell("bla", 12);
    T.sell("apple", 12);
    T.sell("oi", 1);
    T.sell("nanana", 1);
    T.sell("kukuku", 1);

    std::cout << "Rank of coke: " << T.rank("coke") << std::endl;
    std::cout << "Product with rank 2: " << T.product(2) << std::endl;
    std::cout << "Total sold between rank 1 and 3: " << T.sold(1, 3) << std::endl;

    return 0;
}
```
## Compilation and Execution
To compile and run the code, use a C++ compiler such as g++. Here is a basic compilation command:
```bash
g++ -o bestsellers main.cpp
./bestsellers
```
