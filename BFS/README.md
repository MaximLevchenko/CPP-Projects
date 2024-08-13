# Pathfinding with Items Collection in a Graph

This project implements a pathfinding algorithm that not only finds the shortest path in a graph but also ensures that specific items scattered across different locations are collected. The project uses a breadth-first search (BFS) approach and bitmasking to efficiently handle the collection of items.

## Introduction

This project is a C++ implementation that solves a pathfinding problem where the goal is to navigate from a start location to an end location within a map, while collecting all necessary items along the way. The graph is represented using an adjacency list, and the item collection is managed using bitmasking techniques.

## Key Structures and Concepts

### `Map` Structure
Represents the map on which the pathfinding takes place:
- `size_t places`: The number of places (nodes) in the map.
- `Place start, end`: The starting and ending locations.
- `std::vector<std::pair<Place, Place>> connections`: The connections (edges) between the places.
- `std::vector<std::vector<Place>> items`: The items located at various places.

### `Graph` Class
Handles the BFS-based pathfinding and item collection:
- `std::unordered_map<std::pair<Place, bitmask>, std::pair<Place, bitmask>> predators`: Tracks the predecessor of each state (node and bitmask combination) during BFS.
- `std::unordered_map<Place, std::vector<Place>> map_neighbours`: Stores the adjacency list representation of the graph.
- `std::unordered_map<Place, std::vector<size_t>> item_location`: Maps each place to the items it contains.
- `std::unordered_map<size_t, bitmask> item_bitmask`: Assigns a unique bitmask to each item.

## Key Functions

### `bool bfs(size_t item_cnt, Place start, Place end, std::list<Place> &found_path)`
Performs BFS to find the shortest path from `start` to `end` while collecting all items. Returns `true` if such a path is found, otherwise `false`.

### `std::list<Place> find_path(const Map &map)`
Main function that initializes the graph and uses `bfs` to find the required path. Returns the list of places constituting the path.

## Usage Example

Here’s a sample usage of the `find_path` function:

```cpp
#include <iostream>
#include "Pathfinding.h" // Include your header file

int main() {
    Map map{
        4,  // Number of places
        0,  // Start place
        1,  // End place
        {{0, 2}, {2, 3}, {0, 3}, {3, 1}}, // Connections
        {{2}} // Items locations
    };

    std::list<Place> path = find_path(map);

    if (!path.empty()) {
        std::cout << "Found path: ";
        for (Place p : path) {
            std::cout << p << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No valid path found." << std::endl;
    }

    return 0;
}
```

## Compilation and Execution
To compile and run the code, use a C++ compiler such as g++. Here is a basic compilation command:
```bash
g++ -o pathfinding main.cpp
./pathfinding

```
