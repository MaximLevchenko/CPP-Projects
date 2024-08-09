#ifndef __PROGTEST__

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <variant>
#include <vector>

using namespace std;

// Defining State as unsigned int and Symbol as uint8_t
using State = unsigned int;
using Symbol = uint8_t;

// Nondeterministic Finite Automaton (NFA) structure
struct NFA {
    std::set<State> m_States; // Set of states in the NFA
    std::set<Symbol> m_Alphabet; // Set of symbols in the alphabet
    std::map<std::pair<State, Symbol>, std::set<State>> m_Transitions; // Transition function
    State m_InitialState; // Initial state of the NFA
    std::set<State> m_FinalStates; // Set of final (accepting) states
};

// Deterministic Finite Automaton (DFA) structure
struct DFA {
    std::set<State> m_States; // Set of states in the DFA
    std::set<Symbol> m_Alphabet; // Set of symbols in the alphabet
    std::map<std::pair<State, Symbol>, State> m_Transitions; // Transition function
    State m_InitialState; // Initial state of the DFA
    std::set<State> m_FinalStates; // Set of final (accepting) states
};

#endif

/**
 * Function for encrypting states and constructing a DFA from an NFA's determinization transitions.
 * @param new_transitions New transitions generated during determinization
 * @param nfa Original NFA to be converted to a DFA
 * @return A DFA constructed from the NFA's transitions
 */
 DFA encryptAndConstructDfa(const std::map<std::pair<std::set<State>, Symbol>, std::set<State>> &new_transitions,
                           const NFA &nfa) {
    DFA dfa;
    dfa.m_Alphabet = nfa.m_Alphabet; // Copying the alphabet from NFA to DFA
    std::map<std::set<State>, State> encrypt_table; // Table to map NFA state sets to DFA states
    State state_cnt = 0; // State counter for new DFA states

    // Creating encrypted states for DFA and adding final states
    for (const auto &transition: new_transitions) {
        if (!encrypt_table.count(transition.first.first)) {
            encrypt_table.emplace(transition.first.first, state_cnt);
            state_cnt++;
        }
        dfa.m_States.emplace(encrypt_table.at(transition.first.first)); // Adding encrypted states to DFA
        for (const auto &f_state: nfa.m_FinalStates) {
            if (transition.first.first.count(
                    f_state)) // Checking if any NFA final states are part of the current DFA state
                dfa.m_FinalStates.emplace(encrypt_table.at(transition.first.first)); // Adding final states to DFA
        }
    }
    dfa.m_InitialState = encrypt_table.at({nfa.m_InitialState}); // Setting the initial state

    // Filling the transitions for the DFA
    for (const auto &transition: new_transitions) {
        dfa.m_Transitions.insert(
                std::make_pair(std::make_pair(encrypt_table.at(transition.first.first), transition.first.second),
                               encrypt_table.at(transition.second)));
    }
    return dfa;
}

/**
 * Function to determinize an NFA into a DFA.
 * @param nfa The NFA to be determinized
 * @param common_alphabet The alphabet used for determinization, common to all input NFAs
 * @return The determinized DFA
 */
DFA determinize(const NFA &nfa, const std::set<Symbol> &common_alphabet) {
    State max_state = *nfa.m_States.begin();
    for (const auto &state: nfa.m_States) {
        if (state > max_state)
            max_state = state; // Finding the maximum state number
    }

    std::set<std::set<State>> all_sets; // Set of all state sets
    std::map<std::pair<std::set<State>, Symbol>, std::set<State>> new_transitions; // New transitions for DFA
    std::queue<std::set<State>> q; // Queue to process state sets
    q.emplace(std::set<State>{nfa.m_InitialState}); // Starting with the initial state
    all_sets.emplace(std::set<State>{nfa.m_InitialState}); // Adding the initial state set

    while (!q.empty()) {
        std::set<State> q_states = q.front();
        q.pop();
        for (const auto &sym: common_alphabet) {
            std::set<State> aggregate_states_set; // To aggregate states for a transition
            for (const auto &q_state: q_states) {
                if (nfa.m_Transitions.count({q_state, sym})) {
                    for (const auto &m_state: nfa.m_Transitions.at({q_state, sym}))
                        aggregate_states_set.emplace(m_state); // Aggregate states on transition
                }
            }
            if (aggregate_states_set.empty()) {
                aggregate_states_set.emplace(max_state + 1); // If no transition, add a dummy state
            }
            if (!all_sets.count(aggregate_states_set))
                q.emplace(aggregate_states_set); // Add new state set to the queue
            new_transitions.emplace(std::pair<std::set<State>, Symbol>{q_states, sym}, aggregate_states_set);
            all_sets.emplace(aggregate_states_set);
        }
    }
    DFA return_dfa = encryptAndConstructDfa(new_transitions, nfa);
    return return_dfa;
}

/**
 * Function for creating an alphabet for parallel run.
 * @param a Alphabet from first DFA
 * @param b Alphabet from second DFA
 * @return New alphabet for parallel run
 */
std::set<Symbol> parallelRunCreateAlphabet(const std::set<Symbol> &a, const std::set<Symbol> &b) {
    std::set<Symbol> syms{a}; // Initialize with symbols from DFA a
    syms.insert(b.begin(), b.end()); // Insert symbols from DFA b
    return syms;
}

/**
 * Function to encrypt states and construct DFA for parallel run.
 * @param new_pr_transitions New transitions generated during parallel run
 * @param result Reference to the resulting DFA
 * @param new_initial_state Initial state for the parallel run DFA
 * @param new_final_states Set of final states for the parallel run DFA
 */
void parallelRunEncryptAndConstructDFA(
        const std::map<std::pair<std::pair<State, State>, Symbol>, std::pair<State, State>> &new_pr_transitions,
        DFA &result, std::pair<State, State> new_initial_state,
        const std::set<std::pair<State, State>> &new_final_states) {
    DFA dfa;
    std::map<std::pair<State, State>, State> encrypt_table; // Encryption table for state pairs
    State state_cnt = 0;

    // Filling up the encryption table and adding states to the DFA
    for (const auto &transition: new_pr_transitions) {
        if (!encrypt_table.count(transition.first.first)) {
            encrypt_table.emplace(transition.first.first, state_cnt);
            state_cnt++;
        }
        dfa.m_States.emplace(encrypt_table[transition.first.first]);
    }

    // Filling up renamed transitions
    for (const auto &transition: new_pr_transitions) {
        dfa.m_Transitions.insert(
                std::make_pair(std::make_pair(encrypt_table.at(transition.first.first), transition.first.second),
                               encrypt_table.at(transition.second)));
    }

    // Adding final states to DFA
    for (const auto &f_state: new_final_states) {
        dfa.m_FinalStates.insert(encrypt_table.at(f_state));
    }

    // Setting the initial state and updating the result DFA
    result.m_InitialState = encrypt_table.at(new_initial_state);
    result.m_States = dfa.m_States;
    result.m_Transitions = dfa.m_Transitions;
    result.m_FinalStates = dfa.m_FinalStates;
}

/**
 * Function to perform a parallel run (intersection or union) on two DFAs.
 * @param dfa_a The first DFA
 * @param dfa_b The second DFA
 * @param operation Specifies whether to perform intersection ("intersect") or union ("unify")
 * @return The resulting DFA from the parallel run
 */
DFA parallelRun(DFA &dfa_a, DFA &dfa_b, const std::string &operation) {
    DFA result;
    result.m_Alphabet = parallelRunCreateAlphabet(dfa_a.m_Alphabet, dfa_b.m_Alphabet);
    std::map<std::pair<std::pair<State, State>, Symbol>, std::pair<State, State>> new_pr_transitions; // New transitions for the parallel run DFA
    std::set<std::pair<State, State>> new_final_states;
    std::set<std::pair<State, State>> all_pairs;
    std::queue<std::pair<State, State>> q;
    q.emplace(dfa_a.m_InitialState, dfa_b.m_InitialState); // Start with the initial states of both DFAs
    std::pair<State, State> new_initial_state{dfa_a.m_InitialState, dfa_b.m_InitialState}; // New initial state pair
    all_pairs.emplace(dfa_a.m_InitialState, dfa_b.m_InitialState);

    // Processing the state pairs in the queue
    while (!q.empty()) {
        std::pair<State, State> q_state = q.front();
        q.pop();
        for (const auto &sym: result.m_Alphabet) {
            std::pair<State, State> new_state = {dfa_a.m_Transitions.at({q_state.first, sym}),
                                                 dfa_b.m_Transitions.at({q_state.second, sym})};
            if ((dfa_a.m_FinalStates.count(q_state.first) && dfa_b.m_FinalStates.count(q_state.second) &&
                 operation == "intersect") ||
                ((dfa_a.m_FinalStates.count(q_state.first) || dfa_b.m_FinalStates.count(q_state.second)) &&
                 operation == "unify")) {
                new_final_states.emplace(q_state); // Add to final states based on the operation
            }
            if (!all_pairs.count(new_state)) {
                all_pairs.emplace(new_state);
                q.emplace(new_state);
            }
            new_pr_transitions.emplace(std::make_pair(q_state, sym), new_state);
        }
    }
    parallelRunEncryptAndConstructDFA(new_pr_transitions, result, new_initial_state, new_final_states);
    return result;
}

/**
 * Function to get the useful states in a DFA (those leading to final states).
 * @param dfa_a The DFA from which to extract useful states
 * @return A set of states that lead to final states in the DFA
 */
 std::set<State> getUsefulStates(const DFA &dfa_a) {
    std::set<State> useful_states;
    std::queue<State> q;
    for (const auto &f_state: dfa_a.m_FinalStates) {
        q.emplace(f_state); // Start with the final states
        useful_states.emplace(f_state);
    }

    // Traverse backwards to find all states leading to final states
    while (!q.empty()) {
        State q_state = q.front();
        q.pop();
        for (const auto &trans: dfa_a.m_Transitions) {
            if (trans.second == q_state) {
                if (!useful_states.count(trans.first.first)) {
                    q.emplace(trans.first.first);
                    useful_states.emplace(trans.first.first);
                }
            }
        }
    }
    return useful_states;
}

/**
 * Function to remove redundant states and rename remaining states in the DFA.
 * @param dfa_a The DFA from which to remove redundant states
 * @param useful_states Set of useful states that should be retained
 * @return A new DFA with only the useful states, renamed for simplicity
 */
DFA removeRedundantStatesAndRename(DFA &dfa_a, const std::set<State> &useful_states) {
    DFA result;
    size_t size_cnt = 0;
    std::map<State, State> encrypt_table;

    if (useful_states.empty()) {
        result.m_FinalStates = {};
        result.m_InitialState = dfa_a.m_InitialState;
        result.m_Transitions = {};
        result.m_States.emplace(dfa_a.m_InitialState);
        result.m_Alphabet = dfa_a.m_Alphabet;
        return result;
    }

    // Renaming useful states to ensure they are in order
    for (const auto &u_state: useful_states) {
        encrypt_table.emplace(u_state, size_cnt);
        result.m_States.emplace(encrypt_table.at(u_state));
        size_cnt++;
    }

    // Renaming transitions and filling in blank spaces
    for (const auto &transition: dfa_a.m_Transitions) {
        if (!useful_states.count(transition.first.first))
            continue;
        if (useful_states.count(transition.second)) {
            result.m_Transitions.insert(
                    std::make_pair(std::make_pair(encrypt_table.at(transition.first.first), transition.first.second),
                                   encrypt_table.at(transition.second)));
        } else {
            result.m_Transitions.insert(
                    std::make_pair(std::make_pair(encrypt_table.at(transition.first.first), transition.first.second),
                                   1234567897)); // Placeholder for undefined transitions
        }
    }

    // Adding final states to the result DFA
    for (const auto &f_state: dfa_a.m_FinalStates) {
        result.m_FinalStates.emplace(encrypt_table.at(f_state));
    }
    result.m_Alphabet = dfa_a.m_Alphabet;
    result.m_InitialState = encrypt_table.at(dfa_a.m_InitialState);
    return result;
}

/**
 * Helper function to update the encryption table during DFA minimization.
 * @param encrypt_table The encryption table to update
 * @param trans_first_first The state to be updated in the encryption table
 * @param state_cnt The new state identifier
 */
void updateEncTable(std::map<State, State> &encrypt_table, const State &trans_first_first, const size_t &state_cnt) {
    encrypt_table.erase(trans_first_first);
    encrypt_table.emplace(trans_first_first, state_cnt);
}

/**
 * Helper function to update transitions based on a new encryption table.
 * @param new_transitions The new transition map to be updated
 * @param old_transitions The original transitions from the DFA
 * @param encrypt_table The encryption table used to rename states
 */
 void updateTransition(std::map<std::pair<State, Symbol>, State> &new_transitions,
                       const std::map<std::pair<State, Symbol>, State> &old_transitions,
                       const std::map<State, State> &encrypt_table) {
    std::map<std::pair<State, Symbol>, State> tmp_trans;
    for (const auto &trans: old_transitions) {
        if (trans.second == 1234567897)
            tmp_trans.emplace(std::make_pair(trans.first.first, trans.first.second), 1234567897);
        else
            tmp_trans.emplace(std::make_pair(trans.first.first, trans.first.second), encrypt_table.at(trans.second));
    }
    new_transitions = tmp_trans;
}

/**
 * Helper function to update state vectors and all vectors for DFA minimization.
 * @param new_state_all_trans A map of new states to their transitions
 * @param new_transitions The updated transition map after minimization
 * @param m_Alphabet The DFA's alphabet
 * @param m_FinalStates The set of final states in the DFA
 * @param encrypt_table The encryption table for state renaming
 * @param all_vecs A map of all state vectors to their corresponding states
 */
 void updateNewStatesVecAndAllVecs(std::map<State, std::vector<State>> &new_state_all_trans,
                                        const std::map<std::pair<State, Symbol>, State> &new_transitions,
                                        const std::set<Symbol> &m_Alphabet, const std::set<State> &m_FinalStates,
                                        const std::map<State, State> &encrypt_table,
                                        std::map<std::vector<State>, State> &all_vecs) {
    all_vecs.clear(); // Clear the all_vecs map
    std::set<State> checked_states;
    std::set<State> checked_states_for_vec;

    for (auto &trans: new_transitions) {
        if (checked_states.count(trans.first.first))
            continue;
        else
            checked_states.emplace(trans.first.first);

        std::vector<State> aggregate_states_vec;
        for (const auto &sym: m_Alphabet) {
            if (new_transitions.count({trans.first.first, sym})) {
                aggregate_states_vec.push_back(new_transitions.at({trans.first.first, sym}));
            }
        }
        if (m_FinalStates.count(trans.first.first))
            aggregate_states_vec.push_back(5555555); // Special marker for final states

        if (!checked_states_for_vec.count(encrypt_table.at(trans.first.first))) {
            new_state_all_trans.at(encrypt_table.at(trans.first.first)) = aggregate_states_vec;
            all_vecs.emplace(aggregate_states_vec, encrypt_table.at(trans.first.first));
            checked_states_for_vec.emplace(encrypt_table.at(trans.first.first));
        }
    }
}

/**
 * Function to construct a minimized DFA from a set of minimized state transitions.
 * @param new_states_all_trans A map of new states to their minimized transitions
 * @param m_Alphabet The DFA's alphabet
 * @param m_Initial_State The initial state of the DFA
 * @param final_states Set of final states in the minimized DFA
 * @param encrypt_table The encryption table used for state renaming
 * @return The minimized DFA
 */
DFA constructMinimilized(const std::map<State, std::vector<State>> &new_states_all_trans,
                          const std::set<Symbol> &m_Alphabet, const State m_Initial_State,
                          const std::set<State> &final_states,
                          const std::map<State, State> &encrypt_table) {
    DFA result;
    result.m_Alphabet = m_Alphabet;
    if (encrypt_table.count(m_Initial_State))
        result.m_InitialState = encrypt_table.at(m_Initial_State);

    // Construct DFA from minimized state transitions
    for (const auto &trans: new_states_all_trans) {
        auto it = result.m_Alphabet.begin();
        for (const auto &vec_item: trans.second) {
            if (vec_item == 1234567897) {
                it++;
                continue;
            }
            if (vec_item == 5555555) {
                result.m_FinalStates.emplace(trans.first);
                continue;
            }
            result.m_Transitions.emplace(std::make_pair(trans.first, *it), vec_item);
            result.m_States.emplace(trans.first);
            result.m_States.emplace(vec_item);
            it++;
        }
    }
    return result;
}

/**
 * Function to minimize a DFA by merging equivalent states.
 * @param result The DFA to be minimized (input and output)
 */
void minimalise(DFA &result) {
    State starting_symbol = result.m_InitialState;
    std::map<State, State> encrypt_table;
    std::map<std::pair<State, Symbol>, State> new_transitions; // New transition map for minimized DFA
    std::map<State, std::vector<State>> new_states_all_trans; // State transitions in minimized DFA
    std::set<State> already_emplaced;
    State init_beh, finit_beh;
    bool init_flag = true, finit_flag = true;

    // Initial partitioning of states based on finality
    for (const auto &state: result.m_States) {
        if (!result.m_FinalStates.count(state)) {
            if (init_flag) {
                init_beh = state;
                init_flag = false;
            }
            encrypt_table.emplace(state, init_beh); // Non-final states grouped together
        } else {
            if (finit_flag) {
                finit_beh = state;
                finit_flag = false;
            }
            encrypt_table.emplace(state, finit_beh); // Final states grouped together
        }
    }

    // Creating the initial transitions for minimized DFA
    std::map<std::vector<State>, State> all_vecs;
    for (const auto &trans: result.m_Transitions) {
        if (trans.second == 1234567897) {
            new_transitions.emplace(std::make_pair(trans.first.first, trans.first.second), 1234567897);
        } else
            new_transitions.emplace(std::make_pair(trans.first.first, trans.first.second),
                                    encrypt_table.at(trans.second));
    }

    bool smth_changed = true;

    // Iteratively refine state partitions until no further changes
    while (smth_changed) {
        std::set<State> checked_states;
        smth_changed = false;
        std::map<State, State> new_potential_encrypt_table(encrypt_table);

        for (const auto &trans: new_transitions) {
            if (checked_states.count(trans.first.first))
                continue;
            else
                checked_states.emplace(trans.first.first);

            std::vector<State> aggregate_states_vec;
            for (const auto &sym: result.m_Alphabet) {
                if (new_transitions.count({trans.first.first, sym}))
                    aggregate_states_vec.push_back(new_transitions.at({trans.first.first, sym}));
            }
            if (result.m_FinalStates.count(trans.first.first))
                aggregate_states_vec.push_back(5555555);

            if (!all_vecs.count(aggregate_states_vec)) {
                new_states_all_trans.emplace(trans.first.first, aggregate_states_vec);
                all_vecs.emplace(aggregate_states_vec, trans.first.first);
                updateEncTable(new_potential_encrypt_table, trans.first.first,
                                 trans.first.first); // Updated potential encryption table
                smth_changed = true;
            }

            if (all_vecs.count(aggregate_states_vec)) {
                State rename_encrypt = all_vecs.at(aggregate_states_vec);
                updateEncTable(new_potential_encrypt_table, trans.first.first, rename_encrypt);
            }
        }

        // Update transitions based on the new encryption table
        updateTransition(new_transitions, result.m_Transitions, new_potential_encrypt_table);
        // Update state vectors and all vectors for minimization
        updateNewStatesVecAndAllVecs(new_states_all_trans, new_transitions, result.m_Alphabet,
                                           result.m_FinalStates,
                                           new_potential_encrypt_table, all_vecs);
        encrypt_table = new_potential_encrypt_table;
    }

    // Construct the minimized DFA
    result = constructMinimilized(new_states_all_trans, result.m_Alphabet, result.m_InitialState, result.m_FinalStates,
                                   encrypt_table);

    if (result.m_States.empty())
        result.m_States.emplace(encrypt_table.at(starting_symbol));
}

/**
 * Function to intersect two NFAs and return the resulting DFA.
 * @param a The first NFA
 * @param b The second NFA
 * @return The resulting DFA from the intersection of the two NFAs
 */
 DFA intersect(const NFA &a, const NFA &b) {
    std::set<Symbol> common_alphabet = parallelRunCreateAlphabet(a.m_Alphabet, b.m_Alphabet);

    DFA dfa_a = determinize(a, common_alphabet); // Determinize NFA a
    DFA dfa_b = determinize(b, common_alphabet); // Determinize NFA b
    DFA dfa_pr = parallelRun(dfa_a, dfa_b, "intersect"); // Perform parallel run for intersection
    std::set<State> useful_states = getUsefulStates(dfa_pr); // Get useful states
    DFA result = removeRedundantStatesAndRename(dfa_pr, useful_states); // Remove redundant states and rename
    minimalise(result); // Minimize the resulting DFA
    return result;
}

/**
 * Function to unify two NFAs and return the resulting DFA.
 * @param a The first NFA
 * @param b The second NFA
 * @return The resulting DFA from the union of the two NFAs
 */

DFA unify(const NFA &a, const NFA &b) {
    std::set<Symbol> common_alphabet = parallelRunCreateAlphabet(a.m_Alphabet, b.m_Alphabet);

    DFA dfa_a = determinize(a, common_alphabet); // Determinize NFA a
    DFA dfa_b = determinize(b, common_alphabet); // Determinize NFA b
    DFA dfa_pr = parallelRun(dfa_a, dfa_b, "unify"); // Perform parallel run for union
    std::set<State> useful_states = getUsefulStates(dfa_pr); // Get useful states
    DFA result = removeRedundantStatesAndRename(dfa_pr, useful_states); // Remove redundant states and rename
    minimalise(result); // Minimize the resulting DFA
    return result;
}

#ifndef __PROGTEST__

// Function to compare two DFAs
bool operator==(const DFA &a, const DFA &b) {
    return std::tie(a.m_States, a.m_Alphabet, a.m_Transitions, a.m_InitialState, a.m_FinalStates) ==
           std::tie(b.m_States, b.m_Alphabet, b.m_Transitions, b.m_InitialState, b.m_FinalStates);
}
int main() {
    NFA a1{
            {0, 1, 2},
            {'a', 'b'},
            {
                    {{0, 'a'}, {0, 1}},
                    {{0, 'b'}, {0}},
                    {{1, 'a'}, {2}},
            },
            0,
            {2},
    };
    NFA a2{
            {0, 1, 2},
            {'a', 'b'},
            {
                    {{0, 'a'}, {1}},
                    {{1, 'a'}, {2}},
                    {{2, 'a'}, {2}},
                    {{2, 'b'}, {2}},
            },
            0,
            {2},
    };
    DFA a{
            {0, 1, 2, 3, 4},
            {'a', 'b'},
            {
                    {{0, 'a'}, {1}},
                    {{1, 'a'}, {2}},
                    {{2, 'a'}, {2}},
                    {{2, 'b'}, {3}},
                    {{3, 'a'}, {4}},
                    {{3, 'b'}, {3}},
                    {{4, 'a'}, {2}},
                    {{4, 'b'}, {3}},
            },
            0,
            {2},
    };

//    DFA test = intersect(a1, a2);
    assert(intersect(a1, a2) == a);

    NFA b1{
            {0, 1, 2, 3, 4},
            {'a', 'b'},
            {
                    {{0, 'a'}, {1}},
                    {{0, 'b'}, {2}},
                    {{2, 'a'}, {2, 3}},
                    {{2, 'b'}, {2}},
                    {{3, 'a'}, {4}},
            },
            0,
            {1, 4},
    };
    NFA b2{
            {0, 1, 2, 3, 4},
            {'a', 'b'},
            {
                    {{0, 'b'}, {1}},
                    {{1, 'a'}, {2}},
                    {{2, 'b'}, {3}},
                    {{3, 'a'}, {4}},
                    {{4, 'a'}, {4}},
                    {{4, 'b'}, {4}},
            },
            0,
            {4},
    };
    DFA b{
            {0, 1, 2, 3, 4, 5, 6, 7, 8},
            {'a', 'b'},
            {
                    {{0, 'a'}, {1}},
                    {{0, 'b'}, {2}},
                    {{2, 'a'}, {3}},
                    {{2, 'b'}, {4}},
                    {{3, 'a'}, {5}},
                    {{3, 'b'}, {6}},
                    {{4, 'a'}, {7}},
                    {{4, 'b'}, {4}},
                    {{5, 'a'}, {5}},
                    {{5, 'b'}, {4}},
                    {{6, 'a'}, {8}},
                    {{6, 'b'}, {4}},
                    {{7, 'a'}, {5}},
                    {{7, 'b'}, {4}},
                    {{8, 'a'}, {8}},
                    {{8, 'b'}, {8}},
            },
            0,
            {1, 5, 8},
    };
    assert(unify(b1, b2) == b);

    NFA c1{
            {0, 1, 2, 3, 4},
            {'a', 'b'},
            {
                    {{0, 'a'}, {1}},
                    {{0, 'b'}, {2}},
                    {{2, 'a'}, {2, 3}},
                    {{2, 'b'}, {2}},
                    {{3, 'a'}, {4}},
            },
            0,
            {1, 4},
    };
    NFA c2{
            {0, 1, 2},
            {'a', 'b'},
            {
                    {{0, 'a'}, {0}},
                    {{0, 'b'}, {0, 1}},
                    {{1, 'b'}, {2}},
            },
            0,
            {2},
    };
    DFA c{
            {0},
            {'a', 'b'},
            {},
            0,
            {},
    };
    assert(intersect(c1, c2) == c);

    NFA d1{
            {0, 1, 2, 3},
            {'i', 'k', 'q'},
            {
                    {{0, 'i'}, {2}},
                    {{0, 'k'}, {1, 2, 3}},
                    {{0, 'q'}, {0, 3}},
                    {{1, 'i'}, {1}},
                    {{1, 'k'}, {0}},
                    {{1, 'q'}, {1, 2, 3}},
                    {{2, 'i'}, {0, 2}},
                    {{3, 'i'}, {3}},
                    {{3, 'k'}, {1, 2}},
            },
            0,
            {2, 3},
    };
    NFA d2{
            {0, 1, 2, 3},
            {'i', 'k'},
            {
                    {{0, 'i'}, {3}},
                    {{0, 'k'}, {1, 2, 3}},
                    {{1, 'k'}, {2}},
                    {{2, 'i'}, {0, 1, 3}},
                    {{2, 'k'}, {0, 1}},
            },
            0,
            {2, 3},
    };
    DFA d{
            {0, 1, 2, 3},
            {'i', 'k', 'q'},
            {
                    {{0, 'i'}, {1}},
                    {{0, 'k'}, {2}},
                    {{2, 'i'}, {3}},
                    {{2, 'k'}, {2}},
                    {{3, 'i'}, {1}},
                    {{3, 'k'}, {2}},
            },
            0,
            {1, 2, 3},
    };
    assert(intersect(d1, d2) == d);
}
#endif