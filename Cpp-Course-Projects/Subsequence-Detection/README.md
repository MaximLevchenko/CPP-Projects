# Project README: CSelfMatch

## Overview

The `CSelfMatch` project is designed to handle the detection of repeated subsequences within a given sequence of elements. This can be useful for various applications such as data compression or plagiarism detection. The class template `CSelfMatch` is implemented in a generic way, allowing it to work with sequences of any type of elements, including strings, arrays of integers, or lists of pairs.

The main functionality of this project includes:

- **Finding the length of the longest subsequence** that appears at least `N` times in a sequence.
- **Finding all combinations of positions** where the longest subsequence, which appears at least `N` times, occurs in the sequence.

## Key Features

### 1. Subsequence Length Detection

The function `sequenceLen(n)` computes the length of the longest contiguous subsequence that appears at least `n` times within the stored sequence. If no such subsequence exists, the function returns `0`.

### 2. Subsequence Position Combinations

The function `findSequences<N>()` identifies all combinations of positions where the longest subsequence appears at least `N` times. The results are returned as a vector of arrays, where each array contains positions of the subsequence within the original sequence.

## Class Template: `CSelfMatch`

### Constructors

1. **Initializer List Constructor**: Initializes the sequence matcher with elements provided in an `std::initializer_list`.

   ```cpp
   CSelfMatch<char> matcher = {'a', 'b', 'c'};
   ```
2. **Container Constructor**: Initializes the sequence matcher using elements from any container that provides forward iterators (e.g., **std::vector, std::list**).
    ```cpp
   std::vector<int> vec = {1, 2, 3, 4};
    CSelfMatch<int> matcher(vec);
   ```
3. **Iterator Constructor**: Initializes the sequence matcher using a range defined by two iterators.
    ```cpp
   std::list<char> lst = {'a', 'b', 'c', 'd'};
    CSelfMatch<char> matcher(lst.begin(), lst.end());
    ```
### Methods

- **`void push_back(...)`**: Adds elements to the end of the existing sequence. Accepts multiple elements to be added in a single call. *(Optional Method)*

    ```cpp
    matcher.push_back('d', 'e', 'f');
    ```

- **`size_t sequenceLen(size_t n)`**: Returns the length of the longest contiguous subsequence that appears at least `n` times in the sequence. Throws `std::invalid_argument` if `n` is zero.

    ```cpp
    size_t length = matcher.sequenceLen(2);
    ```

- **`std::vector<std::array<size_t, N>> findSequences<N>()`**: Finds all combinations of positions where the longest subsequence appears at least `N` times. Returns a vector of arrays of size `N`.

    ```cpp
    auto sequences = matcher.findSequences<3>();
    ```

## Compilation and Running

### Compilation

To compile the project, use a C++ compiler with C++11 (or later) support:

```bash
g++ -std=c++11 -o self_match main.cpp
```
### Running
```bash
./self_match
```

## Conclusin
The **CSelfMatch** class template provides an efficient way to find repeated subsequences in any generic sequence of elements. It supports various methods to initialize sequences, find the length of the longest repeated subsequences, and locate all combinations of these subsequences' positions. This makes it a versatile tool for applications involving pattern recognition and data analysis.