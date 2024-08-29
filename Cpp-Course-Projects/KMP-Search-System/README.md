# C++ Generic Indexing and Search System

This project implements a generic indexing and search system in C++ using templates. It is designed to efficiently search for sequences within collections using the **Knuth-Morris-Pratt (KMP) algorithm** for pattern matching. The solution is highly customizable, supporting various data types and custom comparison functions.

## Overview

The `CIndex` class template allows for the indexing and searching of sequences within a collection of elements. The class can handle different types of collections, including:

- `string`: A sequence of characters.
- `vector<T>`: A vector of elements of type `T`.
- `list<T>`: A list of elements of type `T`.

The indexing class also supports customizable comparison operations, allowing searches to be performed with user-defined criteria (e.g., case-insensitive searches).

## Key Features

- **Generic Template Design**: The `CIndex` class is a template, making it highly flexible and capable of handling various types of collections and elements.
- **Efficient Searching with KMP Algorithm**: The class uses the Knuth-Morris-Pratt (KMP) algorithm for efficient pattern matching, reducing the time complexity of searches.
- **Customizable Comparisons**: Users can provide their own comparator to define custom search criteria, such as case-insensitive searches or other custom sorting logic.

## Usage

1. **Instantiate `CIndex` with a collection**:
    - The collection can be a `string`, `vector<T>`, or `list<T>`.
2. **Perform Searches**:
    - Call the `search` method with a sequence to find its occurrences in the indexed collection.

### Example

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <cassert>

int main() {
    // Example with a string
    CIndex<std::string> index("abcabcabc");
    std::set<size_t> result = index.search("abc");
    assert(result == (std::set<size_t>{0, 3, 6}));

    // Example with a vector of integers
    std::vector<int> vec = {1, 2, 3, 4, 2, 3, 4, 5};
    CIndex<std::vector<int>> vecIndex(vec);
    std::set<size_t> vecResult = vecIndex.search({2, 3});
    assert(vecResult == (std::set<size_t>{1, 4}));

    // Example with a list of strings using a custom comparator
    std::list<std::string> lst = {"apple", "banana", "Apple", "Banana"};
    CStrComparator comparator(false);
    CIndex<std::list<std::string>, CStrComparator> listIndex(lst, comparator);
    std::set<size_t> listResult = listIndex.search({"apple", "banana"});
    assert(listResult == (std::set<size_t>{0, 2}));

    return 0;
}
```
## Algorithm Details


The Knuth-Morris-Pratt (**KMP**) algorithm is used to perform efficient pattern matching within the collection. This algorithm preprocesses the pattern to create a partial match table (also known as a "prefix" table), which allows the search to skip parts of the text, resulting in a linear time complexity for the search operation, **O(N + M)**, where N is the length of the text and M is the length of the pattern.


## Conclusion

This C++ template-based solution provides a versatile and efficient way to index and search sequences within collections. By leveraging the KMP algorithm and allowing for custom comparators, it caters to various use cases, from simple substring searches to complex custom comparisons.
