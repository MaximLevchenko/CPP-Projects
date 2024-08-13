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
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

using Place = size_t; // Type alias for place identifier

// Structure representing the map, including places, connections, and items
struct Map
{
    size_t places; // Total number of places
    Place start, end; // Starting and ending places
    std::vector<std::pair<Place, Place>> connections; // Vector of connections between places
    std::vector<std::vector<Place>> items; // Vector of items located in different places
};

// Hash specialization for std::pair to use it as a key in unordered_map/unordered_set
template <typename F, typename S>
struct std::hash<std::pair<F, S>>
{
    /**
     * Hash function for std::pair
     * @param p Pair to be hashed
     * @return Hash value of the pair
     */
    std::size_t operator()(const std::pair<F, S> &p) const noexcept
    {
        // Combine the hash values of the two elements
        return std::hash<F>()(p.first) ^ (std::hash<S>()(p.second) << 1);
    }
};

#endif

using bitmask = uint16_t; // Type alias for bitmask representation of collected items

// Class representing the graph and performing the BFS to find the path
class Graph
{
public:
    std::unordered_map<std::pair<Place, bitmask>, std::pair<Place, bitmask>> predators; // Maps current state to predecessor state in BFS
    std::unordered_map<Place, std::vector<Place>> map_neighbours; // Maps each place to its neighbouring places
    std::unordered_map<Place, std::vector<size_t>> item_location; // Maps each place to the items located there
    std::unordered_map<size_t, bitmask> item_bitmask; // Maps each item to its corresponding bitmask

    /**
     * Function to perform a BFS search to find the shortest path that collects all items.
     * @param item_cnt The total number of items
     * @param start The starting place in the map
     * @param end The target place in the map
     * @param found_path Reference to a list to store the found path
     * @return True if a path is found, false otherwise
     */
    bool bfs(size_t item_cnt, Place start, Place end, std::list<Place> &found_path)
    {
        uint16_t strt_mask = 0; // Bitmask representing items collected at the start
        uint16_t trgt_mask = 0; // Bitmask representing all items collected

        // Setting the target bitmask to represent all items
        for (size_t i = 0; i < item_cnt; i++)
            trgt_mask = (trgt_mask << 1) + 1;

        std::queue<std::pair<Place, bitmask>> q;

        // Check if the start place contains any items
        if (item_location.count(start) != 0)
            for (const auto &item : item_location[start])
            {
                strt_mask |= item_bitmask[item]; // Update the starting mask with the items found at the start
                if (strt_mask == trgt_mask && start == end)
                {
                    found_path.push_back(start); // If all items are collected and we're already at the end, return
                    return true;
                }
            }

        q.emplace(start, strt_mask); // Initialize the queue with the starting place and mask
        predators.insert({{start, strt_mask}, {-1, -1}}); // Mark the start as visited with a dummy predecessor

        while (!q.empty())
        {
            uint16_t curr_mask = q.front().second; // Current bitmask of collected items
            std::pair<Place, bitmask> node = q.front(); // Current node
            q.pop();

            // Explore neighbours of the current node
            for (const auto &nbr : map_neighbours[node.first])
            {
                uint16_t new_mask = curr_mask;
                if (item_location.count(nbr))
                    for (const auto &item : item_location[nbr])
                        new_mask |= item_bitmask[item]; // Update the bitmask with items found at the neighbour

                if (!predators.count({nbr, new_mask})) // If the neighbour hasn't been visited with this bitmask
                {
                    predators.insert({{nbr, new_mask}, node}); // Mark it as visited
                    if (new_mask == trgt_mask && nbr == end) // If all items are collected and we're at the end
                    {
                        Place dest = nbr;
                        while ((int)dest != -1) // Backtrack to find the path
                        {
                            found_path.push_back(dest);
                            Place tmp_dest = dest;
                            tmp_dest = predators[{dest, new_mask}].first;
                            new_mask = predators[{dest, new_mask}].second;
                            dest = tmp_dest;
                        }
                        return true;
                    }
                    else
                        q.emplace(nbr, new_mask); // Continue exploring
                }
            }
        }
        return false; // Return false if no path found
    }
};

/**
 * Function to find the path from start to end in the map while collecting all items.
 * @param map The map containing places, connections, and items
 * @return A list of places representing the found path, or an empty list if no path is found
 */
std::list<Place> find_path(const Map &map)
{
    Graph g;
    size_t item_cnt = 0;

    // Map each item to its location
    for (const auto &row : map.items)
    {
        for (const auto &col : row)
        {
            g.item_location[col].push_back(item_cnt);
        }
        item_cnt++;
    }

    // Map each item to a unique bitmask
    uint16_t mask = 1;
    for (size_t i = 0; i < map.items.size(); i++)
    {
        g.item_bitmask[i] = mask;
        mask <<= 1;
    }

    // Map each place to its neighbours
    for (const auto &connection : map.connections)
    {
        g.map_neighbours[connection.first].push_back(connection.second);
        g.map_neighbours[connection.second].push_back(connection.first);
    }

    std::list<Place> found_path;
    if (g.bfs(map.items.size(), map.start, map.end, found_path))
    {
        found_path.reverse(); // Reverse the path to get the correct order
        return found_path;
    }

    return {}; // Return an empty list if no path is found
}

#ifndef __PROGTEST__

using TestCase = std::pair<size_t, Map>; // Type alias for a test case

// Class template argument deduction exists since C++17 :-)
const std::array examples = {
        TestCase{1, Map{2, 0, 0, {{0, 1}}, {{0}}}}, // Test case 0
        TestCase{3, Map{2, 0, 0, {{0, 1}}, {{1}}}}, // Test case 1
        TestCase{3, Map{4, 0, 1, {{0, 2}, {2, 3}, {0, 3}, {3, 1}}, {}}}, // Test case 2
        TestCase{4, Map{4, 0, 1, {{0, 2}, {2, 3}, {0, 3}, {3, 1}}, {{2}}}}, // Test case 3
        TestCase{0, Map{4, 0, 1, {{0, 2}, {2, 3}, {0, 3}, {3, 1}}, {{2}, {}}}}, // Test case 4
};

/**
 * Main function to run the test cases and verify the correctness of the find_path function.
 * @return 0 if all tests pass, otherwise returns the number of failed tests
 */
int main()
{
    int fail = 0;

    // Iterate through all test cases
    for (size_t i = 0; i < examples.size(); i++)
    {
        auto sol = find_path(examples[i].second); // Get the path for the current test case
        if (sol.size() != examples[i].first) // Check if the path length matches the expected result
        {
            std::cout << "Wrong answer for map " << i << std::endl; // Output error message if it doesn't match
            fail++;
        }
    }

    if (fail)
        std::cout << "Failed " << fail << " tests" << std::endl; // Output number of failed tests
    else
        std::cout << "All tests completed" << std::endl; // Output success message

    return 0; // Return success
}

#endif
