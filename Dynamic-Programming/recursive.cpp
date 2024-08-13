#ifndef __PROGTEST__

#include <cassert>
#include <cstdint>
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
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>

using ChristmasTree = size_t; // Alias for tree identifier

// Structure representing the problem details for the tree problem
struct TreeProblem {
    int max_group_size; // Maximum group size allowed
    std::vector<uint64_t> gifts; // Vector of gifts on each tree
    std::vector<std::pair<ChristmasTree, ChristmasTree>> connections; // Pairs of connections between trees
};

#endif

/**
 * Function to build a graph representation of the trees based on their connections.
 * @param connections Vector of connections between trees
 * @param vec_trees Vector of sets, where each set contains connected trees for each tree
 */
void getGraphRepresentation(const std::vector<std::pair<ChristmasTree, ChristmasTree>> &connections,
                            std::vector<std::set<ChristmasTree>> &vec_trees) {
    for (const auto &con: connections) {
        vec_trees[con.first].emplace(con.second);
        vec_trees[con.second].emplace(con.first);
    }
}

/**
 * Recursive function to solve the tree problem by calculating the maximum presents that can be collected.
 * @param root The root of the tree being processed
 * @param node The current node (tree) being processed
 * @param pred The predecessor node (parent in the tree)
 * @param checked_nodes Set of nodes that have already been processed
 * @param node_presents Vector tracking the present count for each node
 * @param vec_trees Vector of sets representing the graph of trees
 * @param gifts Vector of gifts on each tree
 * @return The maximum presents that can be collected from the current node and its subtrees
 */
uint64_t solve_rec(const ChristmasTree &root, ChristmasTree &node, size_t pred, std::set<ChristmasTree> &checked_nodes,
                   std::vector<std::pair<size_t, size_t>> &node_presents,
                   const std::vector<std::set<ChristmasTree>> &vec_trees,
                   const std::vector<uint64_t> &gifts) {

    // Recursively process each neighboring node (child nodes in the tree)
    for (auto neigh: vec_trees[node]) {
        if (!checked_nodes.count(neigh)) {
            checked_nodes.emplace(node); // Mark the current node as checked
            solve_rec(root, neigh, node, checked_nodes, node_presents, vec_trees, gifts);
        }
    }

    // If the current node is a leaf node and not the root, initialize its presents
    if (vec_trees[node].size() == 1 && node != root) {
        node_presents[node].first = gifts[node];
        node_presents[node].second = 0;
    } else {
        // Calculate the total presents if the node is included or excluded in the collection
        size_t accumulate_with = gifts[node];
        size_t accumulate_without = 0;
        for (const auto &neigh: vec_trees[node]) {
            accumulate_with += node_presents[neigh].second;
            accumulate_without += std::max(node_presents[neigh].first, node_presents[neigh].second);
        }
        node_presents[node].first = accumulate_with;
        node_presents[node].second = accumulate_without;
    }
    return std::max(node_presents[node].first, node_presents[node].second); // Return the maximum presents for the node
}

/**
 * Function to solve the TreeProblem using a recursive approach.
 * @param treeProblem The problem instance containing the group size, gifts, and connections
 * @return The maximum presents that can be collected
 */
uint64_t solve(const TreeProblem &treeProblem) {
    std::vector<std::set<ChristmasTree>> vec_trees(
            treeProblem.gifts.size()); // Vector for graph representation of the trees
    getGraphRepresentation(treeProblem.connections, vec_trees); // Build the graph
    ChristmasTree root = 0; // Start with the root node (tree)
    std::set<ChristmasTree> checked_nodes; // Set to keep track of checked nodes
    std::vector<std::pair<size_t, size_t>> node_presents(treeProblem.gifts.size()); // Vector to store presents information for each node
    uint64_t max = solve_rec(root, root, 999999, checked_nodes, node_presents, vec_trees, treeProblem.gifts); // Solve recursively
    return max; // Return the maximum presents that can be collected
}

#ifndef __PROGTEST__

using TestCase = std::pair<uint64_t, TreeProblem>; // Alias for a test case

// Basic test cases for the TreeProblem
const std::vector<TestCase> BASIC_TESTS = {
        {3,   {1, {1,  1,  1,  2}, {{0,  3},  {1,  3},  {2,  3}}}},
        {4,   {1, {1,  1,  1,  4}, {{0,  3},  {1,  3},  {2,  3}}}},
        {57,  {1, {
                   17, 11, 5,  13, 8,  12, 7,  4,  2,  8,
                  }, {
                                    {1,  4},  {6,  1},  {2,  1},  {3,  8},  {8, 0}, {6,  0},  {5,  6}, {7,  2},  {0,  9},
                                   }}},
        {85,  {1, {
                   10, 16, 13, 4,  19, 8,  18, 17, 18, 19, 10,
                  }, {
                                    {9,  7},  {9,  6},  {10, 4},  {4,  9},  {7, 1}, {0,  2},  {9,  2}, {3,  8},  {2,  3}, {5, 4},
                                   }}},
        {79,  {1, {
                   8,  14, 11, 8,  1,  13, 9,  14, 15, 12, 1,  11,
                  }, {
                                    {9,  1},  {1,  2},  {1,  4},  {5,  10}, {7, 8}, {3,  7},  {11, 3}, {11, 10}, {6,  8}, {0, 1}, {0,  3},
                                   }}},
        {102, {1, {
                   15, 10, 18, 18, 3,  4,  18, 12, 6,  19, 9,  19, 10,
                  }, {
                                    {10, 2},  {11, 10}, {6,  3},  {10, 8},  {5, 3}, {11, 1},  {9,  5}, {0,  4},  {12, 3}, {9, 7}, {11, 9}, {4, 12},
                                   }}},
        {93,  {1, {
                   1,  7,  6,  18, 15, 2,  14, 15, 18, 8,  15, 1,  5, 6,
                  }, {
                                    {0,  13}, {6,  12}, {0,  12}, {7,  8},  {8, 3}, {12, 11}, {12, 1}, {10, 12}, {2,  6}, {6, 9}, {12, 7}, {0, 4}, {0, 5},
                                   }}},
};

// Bonus test cases for additional verification
const std::vector<TestCase> BONUS_TESTS = {
        {3, {2, {1, 1, 1, 2}, {{0, 3}, {1, 3}, {2, 3}}}},
        {5, {2, {1, 1, 1, 4}, {{0, 3}, {1, 3}, {2, 3}}}},
};

/**
 * Function to run the test cases and verify the correctness of the solve function.
 * @param T Vector of test cases
 */
void test(const std::vector<TestCase> &T) {
    int i = 0;
    for (auto &[s, t]: T) {
        if (s != solve(t))
            std::cout << "Error in " << i << " (returned " << std::endl << solve(t) << ")" << std::endl;
        i++;
    }
    std::cout << "Finished" << std::endl;
}

/**
 * Main function to execute the test cases.
 * @return 0 if all tests pass
 */
int main() {
    test(BASIC_TESTS);
    // test(BONUS_TESTS);
}

#endif
