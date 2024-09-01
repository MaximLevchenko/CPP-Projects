# University C++ Projects Repository

This repository contains various C++ projects developed during my university coursework. Each project demonstrates different aspects of C++ programming, including data structures, algorithms, multithreading, automata theory, cryptography, and more.

## Table of Contents

1. [AVL Tree: Bestsellers Tracker](#avl-tree-bestsellers-tracker)
2. [BFS Pathfinding with Items Collection](#bfs-pathfinding-with-items-collection)
3. [Cpp Course Projects](#cpp-course-projects)
4. [Cryptography Projects](#cryptography-projects)
5. [Dynamic Programming: Christmas Tree Gift Collection Problem](#dynamic-programming-christmas-tree-gift-collection-problem)
6. [Finite Automata Operations](#finite-automata-operations)
7. [Multithreaded Problem Solver for Rental Optimization](#multithreaded-problem-solver-for-rental-optimization)
8. [Robot Control Server](#robot-control-server)

## AVL Tree: Bestsellers Tracker

This project implements a Bestsellers tracker using an AVL tree to maintain a list of products and their sales counts. The AVL tree ensures efficient insertion, deletion, and rank queries.

### Features
- Efficient insertion, deletion, and lookup with O(log n) complexity.
- Functions to query product ranks and sales statistics.

## BFS Pathfinding with Items Collection

This project uses a breadth-first search (BFS) algorithm to find the shortest path in a graph while collecting specific items.

### Features
- Implements BFS with bitmasking for efficient item collection.
- Finds the shortest path while ensuring all required items are collected.

## Cpp Course Projects

A collection of smaller C++ projects, each demonstrating different concepts such as data structures, algorithms, and system simulations.

### Projects Included

1. **Big Numbers**:
    - Implements a `CBigInt` class to handle arbitrarily large integers.
    - Supports standard arithmetic operations, comparisons, and input/output.

2. **Computer Network System**:
    - Simulates networks, computers, and their components in C++.
    - Allows users to create, modify, and display various hardware components within a simulated environment.

3. **Data Types Simulation**:
    - Provides a set of C++ classes to simulate common data types (`int`, `double`, `enum`, and `struct`).
    - Mimics how a compiler might handle these types internally, offering functionalities like size retrieval and equality checking.

4. **KMP Search System**:
    - Implements the Knuth-Morris-Pratt (KMP) algorithm for efficient substring search.
    - Designed to find patterns within a text using the KMP string-matching algorithm.

5. **Student Database Simulator**:
    - Implements a student management system for a university's study department.
    - Allows for the addition, deletion, and searching of student records, with capabilities for filtering and sorting based on various criteria.

6. **Subsequence Detection**:
    - Detects repeated subsequences within a sequence.
    - Useful for applications such as data compression or plagiarism detection.

7. **Supermarket Management Simulation**:
    - Provides a C++ class to manage inventory in a supermarket setting.
    - Includes functionalities to store products with expiration dates, process shopping lists, and find expired products.

## Cryptography Projects

This repository contains projects focused on cryptographic operations, such as hashing and encryption/decryption using the AES algorithm.

### Projects Included
- **Encryption-Modes**: Demonstrates file encryption using various AES modes.
- **Hashing**: Generates and verifies cryptographic hashes using OpenSSL.

## Dynamic Programming: Christmas Tree Gift Collection Problem

Solves the Christmas Tree Gift Collection problem using recursive and iterative approaches to maximize gifts collected from a set of connected trees.

### Features
- Recursive and iterative solutions for the gift collection problem.
- Compares the efficiency and memory usage of both approaches.

## Finite Automata Operations

Implements operations on Non-deterministic Finite Automata (NFA) and Deterministic Finite Automata (DFA), including determinization, minimization, and intersection/unification.

### Features
- Functions for creating, determinizing, and minimizing automata.
- Supports intersection and union operations.

## Multithreaded Problem Solver for Rental Optimization

A multithreaded application that optimizes the handling of rental requests for a company, ensuring maximum profit and efficient resource use.

### Features
- Uses multithreading to process rental requests concurrently.
- Synchronizes thread operations to ensure correct processing order.

## Robot Control Server

An autonomous robot control server that manages a fleet of robots remotely. The server guides robots to a target location, handling communication and error situations.

### Features
- Multithreaded server using a custom communication protocol.
- Handles authentication, robot movement, and error conditions.

