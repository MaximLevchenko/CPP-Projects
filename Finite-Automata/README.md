# Finite Automata Operations in C++

This project implements operations on Non-deterministic Finite Automata (NFA) and Deterministic Finite Automata (DFA). Specifically, it includes functionalities for creating, determinizing, minimizing, and performing intersection/unification of automata.

## Key Structures

### `NFA` Structure
Represents a non-deterministic finite automaton with the following members:
- `std::set<State> m_States`: Set of states.
- `std::set<Symbol> m_Alphabet`: Set of symbols (alphabet).
- `std::map<std::pair<State, Symbol>, std::set<State>> m_Transitions`: Transition mapping.
- `State m_InitialState`: Initial state.
- `std::set<State> m_FinalStates`: Set of final states.

### `DFA` Structure
Represents a deterministic finite automaton with the following members:
- `std::set<State> m_States`: Set of states.
- `std::set<Symbol> m_Alphabet`: Set of symbols (alphabet).
- `std::map<std::pair<State, Symbol>, State> m_Transitions`: Transition mapping.
- `State m_InitialState`: Initial state.
- `std::set<State> m_FinalStates`: Set of final states.

## Key Functions

### `DFA encryptAndConstructDfa`
Converts a map of transitions for an NFA into a DFA using state encryption (renaming).

### `DFA determinize`
Converts an NFA to a DFA using a breadth-first search to explore state combinations.

### `DFA parallelRun`
Combines two DFAs into one using a specified operation (e.g., intersection or union).

### `std::set<State> getUsefulStates`
Identifies states in a DFA that are reachable from the final states, used for removing redundant states.

### `DFA removeRedundantStatesAndRename`
Removes non-useful states from a DFA and renames the remaining states.

### `void minimalise`
Minimizes a DFA by merging states with identical transition behavior.

### `DFA intersect`
Performs the intersection of two NFAs and returns the resulting minimized DFA.

### `DFA unify`
Performs the union of two NFAs and returns the resulting minimized DFA.


## Compilation and Execution
To compile and run the code, use a C++ compiler such as g++. Here is a basic compilation command:
```bash
g++ -o automata main.cpp
./automata
```

## Usage Example

Here's an example of how to use the `unify` function to combine two NFAs:

```cpp
#include <iostream>
#include "Automata.h" // Include your header file

int main() {
    NFA a{
        {0, 1},
        {'a', 'b'},
        {
            {{1, 'a'}, {0}},
            {{1, 'b'}, {1}},
        },
        0,
        {0, 1},
    };

    NFA b{
        {0, 1, 2, 3, 4},
        {'a', 'b'},
        {
            {{0, 'a'}, {1}},
            {{1, 'b'}, {2}},
            {{2, 'a'}, {3}},
            {{3, 'b'}, {4}},
        },
        0,
        {4},
    };

    DFA result = unify(a, b);
    
    // Display or use the result DFA as needed
    return 0;
}
```

## Testing
### Important Note on Local Tests
Please note that the local tests provided in the **main** function are based on the standard tests and may not work properly with the naming convention implemented in this project. The standard tests do not account for the specific naming convention used here, which might cause discrepancies when running locally.

However, when tested in the university's official testing environment, this implementation scored 100%. The discrepancy lies only in the naming convention, which does not affect the correctness of the logic or the final output as per the standard tests.