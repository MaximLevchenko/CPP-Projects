# CSupermarket - A Supermarket Inventory Management System

## Overview

`CSupermarket` is a C++ class designed to manage inventory in a supermarket setting. It provides functionality to store products with expiration dates, sell products from a shopping list, and find products that have expired before a certain date. The class utilizes a combination of unordered maps and sets to efficiently store and manage products and their expiry dates, allowing for fast lookups, insertions, and deletions.

## Features

- **Store Products**: Add products to the inventory with an expiration date and a specified quantity.
- **Sell Products**: Process a shopping list and remove sold products from the inventory. Supports correcting simple spelling errors in product names.
- **Find Expired Products**: Identify all products that have expired before a given date.
- **Automatic Memory Management**: Uses C++ standard library containers and RAII principles to manage memory automatically.

## Class Design

### CDate Class

A helper class for representing dates. This class supports comparison operations, which are necessary for sorting and managing product expiration.

- **Constructor**: Initializes a date with a given year, month, and day.
- **Operators**: Overloads the `<` and `==` operators to facilitate date comparisons.

### CSupermarket Class

The main class responsible for managing the supermarket inventory. It uses an unordered map to store products, where each product is associated with a set of expiry dates and quantities.

- **Methods**:
    - `store`: Adds a product to the inventory with a specified expiration date and quantity.
    - `sell`: Processes a shopping list, removes sold items from the inventory, and returns a list of items that could not be sold due to insufficient stock or incorrect product names.
    - `expired`: Returns a list of all products that have expired before a given date, sorted in descending order of quantity.

### CProduct Struct

A helper struct used within the `CSupermarket` class to store information about a product's expiration date and quantity.

- **Fields**:
    - `expire_date`: A `CDate` object representing the product's expiration date.
    - `product_count`: An integer representing the quantity of the product.

### set_cmp Struct

A comparator struct used for sorting products within the sets stored in the unordered map of the `CSupermarket` class. It compares products based on their expiration dates.

## Usage

The `CSupermarket` class can be used to simulate inventory management in a supermarket. Below is a brief example of how the class might be used:

```cpp
CSupermarket s;
s.store("bread", CDate(2016, 4, 30), 100)
  .store("butter", CDate(2016, 5, 10), 10)
  .store("beer", CDate(2016, 8, 10), 50);

// Check expired products
list<pair<string, int>> expiredList = s.expired(CDate(2018, 4, 30));

// Sell products
list<pair<string, int>> shoppingList = {{"bread", 2}, {"butter", 5}};
s.sell(shoppingList);
```

## Compilation and Running Tests
To compile the code, use a C++ compiler that supports C++11 or later. Here’s a basic command for compiling with **g++**:

```bash
g++ -std=c++11 -o supermarket main.cpp
```
The **main** function contains test cases that demonstrate the functionality of the CSupermarket class. To run the tests, simply compile and execute the program:
```bash
./supermarket
```