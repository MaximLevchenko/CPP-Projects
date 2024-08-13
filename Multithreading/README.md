# C++ Multithreaded Problem Solver for Rental Optimization

This project implements a multithreaded solution to optimize the profit of rental companies through the use of a problem-solving infrastructure. The main task is to efficiently solve customer rental requests using a set of constraints and deliver the results in a synchronized and ordered manner.



## Introduction

The goal of this project is to manage and optimize rental requests from multiple customers for different types of tools. Each request specifies a rental duration and the price the customer is willing to pay. The system must ensure that:
- The rental intervals for each tool do not overlap.
- The total profit is maximized.

The project makes extensive use of multithreading to handle requests concurrently, ensuring efficient processing and synchronization.

## Key Classes

### `ProblemPackConfig`
Encapsulates the configuration of a problem pack, associating it with a company and tracking whether it has been solved.

### `COptimizer`
The main class responsible for managing the lifecycle of problem-solving operations. It handles:
- Adding companies.
- Managing threads for producing, consuming, and solving problems.
- Synchronizing the order of solved problem packs.

## Implementation Details

- **Multithreading:** The project uses multiple threads to handle different tasks, such as fetching problem packs from companies, processing them, and returning the results in the correct order.
- **Synchronization:** Mutexes and condition variables are used to ensure that threads operate on shared resources safely and that results are returned in the correct order.
- **Progtest Solver:** The `CProgtestSolver` class is used to solve the individual problems within a problem pack. The implementation ensures that the solver's capacity is fully utilized before triggering the computation.

## Usage Example

```cpp
#include "solution.h"

int main(void) {
    COptimizer optimizer;
    ACompanyTest company = std::make_shared<CCompanyTest>();
    ACompanyTest company1 = std::make_shared<CCompanyTest>();

    optimizer.addCompany(company);
    optimizer.addCompany(company1);

    optimizer.start(5);
    optimizer.stop();

    if (!company->allProcessed())
        throw std::logic_error("(some) problems were not correctly processed");
    return 0;
}
```
## **Build Instructions** 
1. **Install Dependencies**: Ensure you have GCC and make installed.
2. **Compile the Project**: Run `make` in the project directory to compile the source files and build the executable.
3. **Generate Dependencies**: Run `make deps` to generate dependencies for the project.
4. **Clean the Build Directory**: Run `make clean` to remove generated files.

### **Makefile Targets** 
- **`all`**: Default target to build the project.
- **`deps`**: Generates dependencies for the source files.
- **`test`**: Compiles the object files and links them into the final executable.
- **`lib`**: Creates a static library from the object file.
- **`clean`**: Removes all generated files from the build directory.
- **`pack`**: Cleans the build directory and creates a tarball package of the project.

## **Run Instructions** 
1. **Start the Optimizer**:
   ```sh
   ./test
   ```
   This will start the optimizer, which will process the rental problems using multiple threads.

2. **Test the Implementation**:
   The provided `sample_tester.cpp` includes sample test cases to validate the implementation.
