# Christmas Tree Gift Collection Problem

This project implements two different solutions—recursive and iterative—to solve the Christmas Tree Gift Collection Problem. The problem involves maximizing the number of gifts collected from a group of connected trees, considering specific constraints on group size and tree connections.


## Introduction

The Christmas Tree Gift Collection Problem involves finding the maximum number of gifts that can be collected from a set of connected trees. The trees are connected in such a way that forms a graph, and each tree has a specific number of gifts. The challenge is to determine the optimal strategy for collecting the gifts while adhering to constraints like group size.

## Problem Description

Given a number of Christmas trees, each tree has a certain number of gifts, and the trees are connected by edges, forming a graph. The goal is to collect the maximum number of gifts, starting from any tree and considering group size limitations.

## Solution Approaches

### Recursive Solution

The recursive solution employs a depth-first search (DFS) strategy to explore all possible paths from the root tree to the leaf nodes. It recursively calculates the maximum number of gifts that can be collected by either including or excluding a node (tree) in the collection.

#### Key Features:
- **Recursive Depth-First Search:** The solution explores all branches of the tree structure recursively.
- **Memoization:** It tracks the maximum gifts that can be collected for each subtree to avoid redundant calculations.

### Iterative Solution

The iterative solution uses a breadth-first search (BFS) strategy, processing nodes level by level, starting from the leaf nodes and moving towards the root. This approach uses a queue to handle the nodes and updates the maximum gift count as it processes each node.

#### Key Features:
- **Breadth-First Search:** The solution processes the tree in layers, ensuring all nodes at the current level are processed before moving to the next.
- **Efficient Memory Usage:** The iterative approach typically requires less memory since it avoids the deep call stack associated with recursion.

## Comparison of Solutions

### Efficiency

- **Iterative Solution:** The iterative solution is generally more efficient than the recursive solution for this problem. It avoids the overhead associated with recursive function calls, which can be significant when dealing with deep or complex tree structures.
- **Recursive Solution:** While the recursive solution is intuitive and easier to implement for tree problems, it can lead to excessive memory usage due to deep call stacks, especially with large inputs. This makes it less suitable for large-scale problems.

### Performance

- **Iterative Solution:** This approach scales better with larger datasets, handling deep or wide tree structures more effectively due to its controlled memory usage and iterative nature.
- **Recursive Solution:** Although it can be faster in certain small-scale scenarios, the recursive solution risks stack overflow and increased computational overhead in large or complex tree structures.

### Summary

In summary, while both solutions are correct and solve the problem, the iterative approach is more effective in terms of memory usage and overall performance, particularly for larger and more complex tree structures.

## Usage Example

Here’s a sample usage of the `solve` function:

```cpp
#include <iostream>
#include "TreeProblem.h" // Include your header file

int main() {
    TreeProblem problem = {
        1, // Maximum group size
        {1, 1, 1, 4}, // Gifts on each tree
        {{0, 3}, {1, 3}, {2, 3}} // Connections between trees
    };

    uint64_t result = solve(problem); // Use the iterative or recursive version

    std::cout << "Maximum gifts collected: " << result << std::endl;

    return 0;
}
```
## Compilation and Execution
To compile and run the code, use a C++ compiler such as g++. Here is a basic compilation command:
```bash
g++ -o treeproblem main.cpp
./treeproblem

```
