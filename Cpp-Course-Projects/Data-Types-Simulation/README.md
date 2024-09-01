# C++ Data Type Simulation

This project implements a set of C++ classes to simulate the representation and operations of some common data types, such as `int`, `double`, `enum`, and `struct`. These classes can be used to emulate how a compiler might manage and manipulate these data types internally.

## Overview

Compilers need to remember the properties of data types during compilation. Some data types are built-in (like `int` and `double`), while others can be defined by programmers (such as `enum` and `struct`). This project provides classes to capture the descriptions and operations of these types, offering functionality such as size retrieval, equality checking, and type printing.

## Classes and Their Purpose

### AbstractCLass

- **Abstract Base Class** for all data types.
- Defines a common interface that all data type classes must implement.
- Pure virtual methods:
    - `getSize()`: Returns the size of the data type.
    - `getType()`: Returns the type as a string.
    - `print_type(std::ostream &os)`: Outputs the type's name to the given output stream.
    - `check_equal(const AbstractCLass &obj)`: Checks if the current type is equal to another type.
    - `clone()`: Creates a copy of the type instance.
- Provides overloaded operators `==` and `!=` to compare types.

### CDataTypeInt

- Represents the primitive type `int`.
- Always returns a size of 4 bytes.
- Overloaded operators:
    - `==`: Compares if two types are both `int`.
    - `!=`: Compares if two types are not both `int`.
    - `<<`: Outputs the name "int" to the provided output stream.

### CDataTypeDouble

- Represents the primitive type `double`.
- Always returns a size of 8 bytes.
- Overloaded operators:
    - `==`: Compares if two types are both `double`.
    - `!=`: Compares if two types are not both `double`.
    - `<<`: Outputs the name "double" to the provided output stream.

### CDataTypeEnum

- Represents an enumerated type (`enum`).
- Always returns a size of 4 bytes.
- Supports adding values to the enumeration with the `add()` method.
- Throws an `invalid_argument` exception if a duplicate value is added.
- Overloaded operators:
    - `==`: Compares if two enum types have the same values in the same order.
    - `!=`: Compares if two enum types do not have the same values in the same order.
    - `<<`: Outputs the enum type and its values in the order they were added.

### CDataTypeStruct

- Represents a structured type (`struct`).
- Size is determined by the sum of its fields' sizes.
- Supports adding fields with `addField(name, type)`.
- Throws an `invalid_argument` exception if a field with the same name already exists.
- Provides `field(name)` method to access a field by name, throwing an exception if the field does not exist.
- Overloaded operators:
    - `==`: Compares if two struct types have the same number of fields and types (field names may differ).
    - `!=`: Compares if two struct types do not match in the number of fields or types.
    - `<<`: Outputs the struct type and its fields in the order they were added.

## Usage

1. **Create Instances**: Instantiate `CDataTypeInt`, `CDataTypeDouble`, `CDataTypeEnum`, and `CDataTypeStruct`.
2. **Manipulate Data Types**: Use methods like `add()` for `CDataTypeEnum` or `addField()` for `CDataTypeStruct`.
3. **Compare Types**: Use `==` and `!=` to compare different data types.
4. **Print Type Information**: Use the overloaded `<<` operator to output type information to the console or other streams.

## Examples

Below is an example demonstrating how to use the system:

```cpp
CDataTypeStruct a = CDataTypeStruct()
    .addField("m_Length", CDataTypeInt())
    .addField("m_Status", CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("DEAD"))
    .addField("m_Ratio", CDataTypeDouble());

CDataTypeStruct b = CDataTypeStruct()
    .addField("m_Length", CDataTypeInt())
    .addField("m_Status", CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("READY"))
    .addField("m_Ratio", CDataTypeDouble());

// Print details
std::cout << a;
std::cout << b;

// Compare structs
assert(a != b);
```
## Compilation and Running Tests
To compile the code, use a C++ compiler that supports C++11 or later. Here’s a basic command for compiling with **g++**:

```bash
g++ -std=c++11 -o simulation main.cpp
```
The **main** function contains test cases that demonstrate the functionality of the project. To run the tests, simply compile and execute the program:
```bash
./simulation
```

## Conclusion
This project provides a detailed simulation of C++ data types, leveraging polymorphism to handle different data types uniformly. The classes and methods allow for comprehensive type management and comparison, simulating how a compiler might interact with these data types during compilation.