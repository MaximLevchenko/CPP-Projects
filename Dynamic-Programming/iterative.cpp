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
 * @param vec_trees Vector of unordered sets, where each set contains connected trees for each tree
 */
void getGraphRepresentation(const std::vector<std::pair<ChristmasTree, ChristmasTree>> &connections,
                            std::vector<std::unordered_set<ChristmasTree>> &vec_trees) {
    for (const auto &con: connections) {
        vec_trees[con.first].emplace(con.second);
        vec_trees[con.second].emplace(con.first);
    }
}

/**
 * Function to identify leaf nodes (trees with only one connection) in the graph.
 * @param vec_trees Vector of unordered sets representing the graph
 * @param lists Unordered set to store the identified leaf nodes
 */
void getLists(const std::vector<std::unordered_set<ChristmasTree>> &vec_trees, std::unordered_set<ChristmasTree> &lists) {
    size_t cnt = 0;
    for (const auto &neigh: vec_trees) {
        if (neigh.size() == 1)
            lists.emplace(cnt);
        cnt++;
    }
}

/**
 * Function to solve the tree problem iteratively by calculating the maximum presents that can be collected.
 * @param node_presents Vector of tuples tracking the present count for each tree
 * @param vec_trees Vector of unordered sets representing the graph
 * @param gifts Vector of gifts on each tree
 * @param max_group_size The maximum size of the group allowed
 * @return The maximum presents that can be collected
 */
uint64_t solve_iter(
        std::vector<std::tuple<size_t, size_t, size_t>> &node_presents,
        std::vector<std::unordered_set<ChristmasTree>> &vec_trees,
        const std::vector<uint64_t> &gifts, int max_group_size) {

    std::unordered_set<ChristmasTree> lists;
    getLists(vec_trees, lists); // Identify the leaf nodes in the graph
    std::queue<ChristmasTree> q;
    uint64_t max_presents = 0;
    std::unordered_set<ChristmasTree> already_checked;

    // Start BFS from leaf nodes
    for (const auto &list: lists) {
        q.emplace(list);
        already_checked.emplace(list);
    }

    while (!q.empty()) {
        ChristmasTree q_front = q.front();
        q.pop();

        if (vec_trees[q_front].empty()) {
            // If we have reached the end of the tree, return the max presents
            return max_group_size == 2 ?
                   std::max(
                           std::max(std::get<0>(node_presents[q_front]), std::get<1>(node_presents[q_front])),
                           std::get<2>(node_presents[q_front])) :
                   std::max(std::get<0>(node_presents[q_front]), std::get<1>(node_presents[q_front]));
        }

        if (!std::get<0>(node_presents[q_front]))
            std::get<0>(node_presents[q_front]) = gifts[q_front];
        if (!std::get<0>(node_presents[*vec_trees[q_front].begin()]))
            std::get<0>(node_presents[*vec_trees[q_front].begin()]) = gifts[*vec_trees[q_front].begin()];

        auto prs_f_s = std::get<0>(node_presents[*vec_trees[q_front].begin()]) + std::get<0>(node_presents[q_front]);

        if ((prs_f_s > std::get<2>(node_presents[*vec_trees[q_front].begin()])) &&
            prs_f_s > std::get<2>(node_presents[*vec_trees[q_front].begin()]) + std::get<1>(node_presents[q_front])) {
            std::get<2>(node_presents[*vec_trees[q_front].begin()]) =
                    std::get<0>(node_presents[*vec_trees[q_front].begin()]) + std::get<0>(node_presents[q_front]);
        } else
            std::get<2>(node_presents[*vec_trees[q_front].begin()]) += std::get<1>(node_presents[q_front]);

        std::get<0>(node_presents[*vec_trees[q_front].begin()]) += std::get<1>(node_presents[q_front]);

        if (max_group_size == 2) {
            std::get<1>(node_presents[*vec_trees[q_front].begin()]) += std::max(
                    std::max(std::get<0>(node_presents[q_front]),
                             std::get<1>(node_presents[q_front])),
                    std::get<2>(node_presents[q_front]));
            max_presents = std::max(
                    std::max(std::get<0>(node_presents[*vec_trees[q_front].begin()]),
                             std::get<1>(node_presents[*vec_trees[q_front].begin()])),
                    std::get<2>(node_presents[*vec_trees[q_front].begin()]));
        } else {
            std::get<1>(node_presents[*vec_trees[q_front].begin()]) += std::max(std::get<0>(node_presents[q_front]),
                                                                                std::get<1>(node_presents[q_front]));
            max_presents = std::max(
                    std::get<0>(node_presents[*vec_trees[q_front].begin()]),
                    std::get<1>(node_presents[*vec_trees[q_front].begin()]));
        }

        vec_trees[*vec_trees[q_front].begin()].erase(q_front);
        if (vec_trees[*vec_trees[q_front].begin()].size() == 1 && !already_checked.count(*vec_trees[q_front].begin())) {
            q.emplace(*vec_trees[q_front].begin());
            already_checked.emplace(*vec_trees[q_front].begin());
        }
    }
    return max_presents;
}

/**
 * Function to solve the TreeProblem using the given maximum group size and tree connections.
 * @param treeProblem The problem instance containing the group size, gifts, and connections
 * @return The maximum presents that can be collected
 */
uint64_t solve(const TreeProblem &treeProblem) {

    std::vector<std::unordered_set<ChristmasTree>> vec_trees(
            treeProblem.gifts.size()); // Vector for graph representation of the trees
    getGraphRepresentation(treeProblem.connections, vec_trees); // Build the graph
    std::unordered_set<ChristmasTree> checked_nodes;
    std::vector<std::tuple<size_t, size_t, size_t>> node_presents(treeProblem.gifts.size());
    uint64_t max = solve_iter(node_presents, vec_trees, treeProblem.gifts, treeProblem.max_group_size); // Solve iteratively

    return max; // Return the maximum presents that can be collected
}

#ifndef __PROGTEST__

using TestCase = std::pair<uint64_t, TreeProblem>; // Alias for a test case

// Basic test cases for the TreeProblem
const std::vector<TestCase> BASIC_TESTS = {
        {37,  {2, {2,  3,  4,  5,  6,  7,  8,  9}, {{0,  1},  {1,  2},  {2,  3},  {3,  4},  {3, 5}, {3,  6},  {6,  7}}}},


        {3,   {1, {1,  1,  1,  2},                 {{0,  3},  {1,  3},  {2,  3}}}},
        {4,   {1, {1,  1,  1,  4},                 {{0,  3},  {1,  3},  {2,  3}}}},
        {57,  {1, {
                   17, 11, 5,  13, 8,  12, 7,  4,  2,  8,
                  },                               {
                                                    {1,  4},  {6,  1},  {2,  1},  {3,  8},  {8, 0}, {6,  0},  {5,  6}, {7,  2},  {0,  9},
                                                   }}},
        {85,  {1, {
                   10, 16, 13, 4,  19, 8,  18, 17, 18, 19, 10,
                  },                               {
                                                    {9,  7},  {9,  6},  {10, 4},  {4,  9},  {7, 1}, {0,  2},  {9,  2}, {3,  8},  {2,  3}, {5, 4},
                                                   }}},
        {79,  {1, {
                   8,  14, 11, 8,  1,  13, 9,  14, 15, 12, 1,  11,
                  },                               {
                                                    {9,  1},  {1,  2},  {1,  4},  {5,  10}, {7, 8}, {3,  7},  {11, 3}, {11, 10}, {6,  8}, {0, 1}, {0,  3},
                                                   }}},
        {102, {1, {
                   15, 10, 18, 18, 3,  4,  18, 12, 6,  19, 9,  19, 10,
                  },                               {
                                                    {10, 2},  {11, 10}, {6,  3},  {10, 8},  {5, 3}, {11, 1},  {9,  5}, {0,  4},  {12, 3}, {9, 7}, {11, 9}, {4, 12},
                                                   }}},
        {93,  {1, {
                   1,  7,  6,  18, 15, 2,  14, 15, 18, 8,  15, 1,  5, 6,
                  },                               {
                                                    {0,  13}, {6,  12}, {0,  12}, {7,  8},  {8, 3}, {12, 11}, {12, 1}, {10, 12}, {2,  6}, {6, 9}, {12, 7},
                                                                                                                                                           {0, 4}, {0, 5},
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
    test(BONUS_TESTS);
}

#endif
