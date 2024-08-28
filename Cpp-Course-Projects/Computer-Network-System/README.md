# Network and Computer Simulation System

This project implements a C++ simulation for networks, computers, and their components. It models various hardware components (like CPU, Memory, and Disk) and organizes them into computers and networks.

## Overview

The system simulates a network of computers, each containing various hardware components such as CPUs, memory modules, and disks. This project allows for creating, cloning, and displaying these components within a computer and the network itself. It provides a flexible way to add, modify, and visualize hardware configurations in a simulated environment.

## Classes and Their Purpose

### CComponent

- **Abstract Base Class** for all hardware components (e.g., CPU, Memory, Disk).
- Defines a common interface for cloning and printing component details.
- Contains pure virtual methods:
    - `clone()`: Creates a copy of the component.
    - `print()`: Outputs component details in a formatted manner.

### CCPU

- **Derived Class** representing a CPU component.
- Stores details such as the number of cores and frequency.
- Implements `clone()` and `print()` to handle CPU-specific data.

### CMemory

- **Derived Class** representing a Memory component.
- Holds the size of the memory in MiB.
- Implements `clone()` and `print()` to manage memory-specific information.

### CDisk

- **Derived Class** representing a Disk component.
- Supports two types of disks: `SSD` and `MAGNETIC`.
- Manages disk partitions, each with a size and label.
- Implements `clone()` and `print()` for disk-specific details and partition information.

### CComputer

- Represents a computer in the network.
- Contains a name, a list of network addresses, and a collection of components (`CComponent` derivatives).
- Supports adding components and network addresses.
- Overloaded `operator<<` to output computer details formatted as per network and standalone contexts.

### CNetwork

- Represents a network consisting of multiple computers (`CComputer`).
- Can add computers to the network and search for a computer by name.
- Overloaded `operator<<` to display the entire network with all its computers and their components.

## Usage

1. **Define Components**: Instantiate `CCPU`, `CMemory`, and `CDisk` objects with desired specifications.
2. **Create Computers**: Use `CComputer` to create computers and add components and addresses.
3. **Build Networks**: Use `CNetwork` to organize computers into a network.
4. **Display Information**: Use `operator<<` to print details of computers and networks.

## Examples

Below is a basic example demonstrating how to use this system:

```cpp
CNetwork network("Example Network");

// Create computers and add components
CComputer comp1("computer1");
comp1.addAddress("192.168.1.1")
     .addComponent(CCPU(4, 2400))
     .addComponent(CMemory(8192))
     .addComponent(CDisk(CDisk::SSD, 512));

CComputer comp2("computer2");
comp2.addAddress("192.168.1.2")
     .addComponent(CCPU(8, 3200))
     .addComponent(CMemory(16384))
     .addComponent(CDisk(CDisk::MAGNETIC, 1024));

// Add computers to network
network.addComputer(comp1);
network.addComputer(comp2);

// Print network details
std::cout << network;
```

## Conclusion

This project provides a versatile framework for simulating networks and computers with customizable hardware components. It allows for detailed configurations and offers a comprehensive view of network structures. The modular design makes it easy to extend and adapt for different simulation needs, making it a powerful tool for both learning and practical applications in network and computer architecture.