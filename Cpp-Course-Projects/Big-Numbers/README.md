# **CBigInt Project** 

## **Overview**

The **CBigInt** project is a C++ implementation of a class designed to handle arbitrarily large integers. Unlike standard integer types (`int`, `long long int`, etc.), which have fixed sizes and limited ranges, `CBigInt` allows for mathematical operations on integers of virtually unlimited size. This class is ideal for applications requiring high-precision calculations.

## **Features**

- **Arbitrary Precision**: Supports integers far beyond the range of built-in types.
- **Basic Arithmetic Operations**: Includes addition (`+`), multiplication (`*`), and their compound assignment counterparts (`+=`, `*=`).
- **Comparison Operators**: Supports all standard comparison operators (`<`, `<=`, `>`, `>=`, `==`, `!=`).
- **Stream Input/Output**: Overloads the `<<` and `>>` operators to allow for easy input and output via streams.
- **Error Handling**: Properly handles invalid input by throwing exceptions, ensuring robust error management.

## **Implemented Constructors and Operators**

- **Constructors**:
    - **Default Constructor**: Initializes the integer to `0`.
    - **Integer Constructor**: Initializes the integer from a standard `int` value.
    - **String Constructor**: Initializes the integer from a decimal string representation, throwing `std::invalid_argument` if the string is invalid.
    - **Copy Constructor**: If needed, for copying instances.

- **Operators**:
    - **Assignment (`=`)**: Assigns values from other `CBigInt` instances, integers, or strings.
    - **Arithmetic (`+`, `*`, `+=`, `*=`)**: Supports addition and multiplication with other `CBigInt` instances, integers, and strings.
    - **Comparison (`<`, `<=`, `>`, `>=`, `==`, `!=`)**: Allows comparison between `CBigInt` instances, integers, and strings.
    - **Stream Operators (`<<`, `>>`)**: Allows `CBigInt` instances to be read from and written to streams in decimal format.

## **Usage Examples**

### **Creating Instances**

```cpp
CBigInt a;                // Default constructor, a = 0
CBigInt b(123);           // Integer constructor, b = 123
CBigInt c("-456");        // String constructor, c = -456
CBigInt d = b;            // Copy constructor, d = 123
```
### **Performing Operations**
```cpp
CBigInt a("1000");
CBigInt b("2000");

CBigInt sum = a + b;         // Addition
CBigInt product = a * b;     // Multiplication

a += b;                      // Compound addition
a *= CBigInt("3");           // Compound multiplication
```
## **Error Handling**

- The string constructor and arithmetic operations that involve strings will throw `std::invalid_argument` if the input string is not a valid representation of a decimal integer.
- The input operator (`>>`) will set the `failbit` if it encounters an invalid format or unexpected characters.

### **Example**

```cpp
try {
    CBigInt invalid("abc123"); // Throws std::invalid_argument
} catch (const std::invalid_argument &e) {
    std::cerr << "Invalid input: " << e.what() << std::endl;
}
```

## **Compilation and Running**

### **Compilation**

To compile the project, use a C++ compiler with C++11 (or later) support:

```bash
g++ -std=c++11 -o bigint main.cpp
```

### Running
After compiling, you can run the program with:
```bash
./bigint
```

## **Conclusion**

The `CBigInt` class provides a robust solution for handling large integers in C++, complete with arithmetic, comparison, and stream input/output capabilities. It is suitable for applications requiring high-precision calculations that exceed the range of standard data types.


