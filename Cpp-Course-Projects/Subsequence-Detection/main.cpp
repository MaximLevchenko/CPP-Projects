#ifndef __PROGTEST__

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <array>
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <compare>
#include <algorithm>
#include <cassert>
#include <memory>
#include <iterator>
#include <functional>
#include <stdexcept>

using namespace std::literals;

// Dummy class to demonstrate minimal requirements for sequence elements
class CDummy {
public:
    // Constructor initializes the CDummy object with a character
    CDummy(char c)
            : m_C(c) {
    }

    // Equality operator to compare two CDummy objects
    bool operator==(const CDummy &other) const = default;

    // Output stream operator to print the CDummy object
    friend std::ostream &operator<<(std::ostream &os, const CDummy &x) {
        return os << '\'' << x.m_C << '\'';
    }

private:
    char m_C; // Character data member
};

#endif /* __PROGTEST__ */

// Main template class for detecting repeated subsequences
template<typename T_>
class CSelfMatch {
public:
    /**
     * Constructor to initialize CSelfMatch with an initializer list
     * @param init Initializer list of elements to initialize the sequence
     */
    CSelfMatch(std::initializer_list<T_> init) : data(init.begin(), init.end()) {}

    /**
     * Constructor to initialize CSelfMatch with a range of iterators
     * @param begin Iterator pointing to the start of the range
     * @param end Iterator pointing to the end of the range
     */
    template<typename Iter>
    CSelfMatch(Iter begin, Iter end) : data(begin, end) {}

    /**
     * Constructor to initialize CSelfMatch with elements from a container
     * @param cont Container holding the elements to initialize the sequence
     */
    template<typename Container>
    CSelfMatch(const Container &cont) : CSelfMatch(std::begin(cont), std::end(cont)) {}

    /**
     * Constructor to initialize CSelfMatch with characters from a string
     * @param str String whose characters are used to initialize the sequence
     */
    CSelfMatch(const std::string &str) : CSelfMatch(str.begin(), str.end()) {}

    /**
     * Method to add multiple elements to the end of the sequence
     * @param val Elements to be added to the sequence
     */
    void push_back(const T_ &val) {
        data.push_back(val);
    }

    /**
     * Variadic template method to add multiple elements to the sequence
     * @param args Arguments representing elements to be added to the sequence
     */
    template<typename ... Args>
    void push_back(Args&&... args) {
        (data.push_back(std::forward<Args>(args)), ...);
    }

    /**
     * Method to find the length of the longest subsequence repeated at least 'n' times
     * @param n Minimum number of times the subsequence should repeat
     * @return Length of the longest subsequence that repeats at least 'n' times
     * @throws std::invalid_argument if 'n' is zero
     */
    size_t sequenceLen(size_t n) const {
        if (n == 0) throw std::invalid_argument("N cannot be zero");
        size_t maxLength = 0;

        // Iterate through all possible lengths of subsequences
        for (size_t len = 1; len <= data.size(); ++len) {
            for (size_t start = 0; start <= data.size() - len; ++start) {
                size_t count = 0;
                std::vector<T_> sub(data.begin() + start, data.begin() + start + len);
                // Check all positions for subsequence match
                for (size_t matchStart = 0; matchStart <= data.size() - len; ++matchStart) {
                    if (std::equal(data.begin() + matchStart, data.begin() + matchStart + len, sub.begin())) {
                        count++;
                    }
                }
                // Update maximum length if conditions are met
                if (count >= n) {
                    maxLength = len > maxLength ? len : maxLength;
                }
            }
        }
        return maxLength;
    }

    /**
     * Method to find all combinations of positions where the longest subsequence appears at least N times
     * @return Vector of arrays with the start positions of subsequences
     * @throws std::invalid_argument if 'N_' is zero
     */
    template<size_t N_>
    std::vector<std::array<size_t, N_>> findSequences() const {
        if (N_ == 0) throw std::invalid_argument("N cannot be zero");

        std::vector<std::array<size_t, N_>> results;
        size_t maxLen = sequenceLen(N_);
        if (maxLen == 0)
            return {};

        // Iterate through all possible starting points for the longest subsequence
        for (size_t start = 0; start <= data.size() - maxLen; ++start) {
            std::vector<T_> sub(data.begin() + start, data.begin() + start + maxLen);
            std::vector<size_t> occurrences;

            // Find all occurrences of the current subsequence
            for (size_t matchStart = 0; matchStart <= data.size() - maxLen; ++matchStart) {
                if (std::equal(data.begin() + matchStart, data.begin() + matchStart + maxLen, sub.begin())) {
                    occurrences.push_back(matchStart);
                }
            }

            // Generate combinations of positions if enough occurrences are found
            if (occurrences.size() >= N_) {
                std::vector<size_t> currentCombo;
                findCombinations(results, occurrences, currentCombo, 0, N_);
            }
        }

        // Remove duplicate combinations and sort the results
        std::sort(results.begin(), results.end());
        results.erase(unique(results.begin(), results.end()), results.end());

        return results;
    }

private:
    std::vector<T_> data; // Internal storage for the sequence elements

    /**
     * Helper function to find all combinations of subsequence start positions
     * @param results Vector to store the resulting combinations
     * @param positions Vector containing valid start positions for the subsequences
     * @param current Current combination being generated
     * @param start Current index in the positions vector
     * @param k Remaining number of positions to add to the combination
     */
    template<size_t N_>
    static void findCombinations(std::vector<std::array<size_t, N_>> &results,
                                 const std::vector<size_t> &positions,
                                 std::vector<size_t> &current,
                                 size_t start, size_t k) {
        if (k == 0) {
            results.push_back({});
            std::copy_n(current.begin(), N_, results.back().begin());
            return;
        }
        // Generate combinations recursively
        for (size_t i = start; i <= positions.size() - k; ++i) {
            current.push_back(positions[i]);
            findCombinations(results, positions, current, i + 1, k - 1);
            current.pop_back();
        }
    }
};

// Deduction guide for constructing CSelfMatch from a string
CSelfMatch(const std::string&) -> CSelfMatch<char>;

// Deduction guide for constructing CSelfMatch from iterators
template<typename Iter>
CSelfMatch(Iter, Iter) -> CSelfMatch<typename std::iterator_traits<Iter>::value_type>;

#ifndef __PROGTEST__
template<size_t N_>
bool positionMatch(std::vector<std::array<size_t, N_>> a,
                   std::vector<std::array<size_t, N_>> b) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}
int main() {
    CSelfMatch<char> x0("aaaaaaaaaaa"s);
    assert (x0.sequenceLen(2) == 10);
    assert (positionMatch(x0.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 1}}));
    CSelfMatch<char> x1("abababababa"s);
    assert (x1.sequenceLen(2) == 9);
    assert (positionMatch(x1.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 2}}));
    CSelfMatch<char> x2("abababababab"s);
    assert (x2.sequenceLen(2) == 10);
    assert (positionMatch(x2.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 2}}));
    CSelfMatch<char> x3("aaaaaaaaaaa"s);
    assert (x3.sequenceLen(3) == 9);
    assert (positionMatch(x3.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 1, 2}}));
    CSelfMatch<char> x4("abababababa"s);
    assert (x4.sequenceLen(3) == 7);
    assert (positionMatch(x4.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 2, 4}}));
    CSelfMatch<char> x5("abababababab"s);
    assert (x5.sequenceLen(3) == 8);
    assert (positionMatch(x5.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 2, 4}}));
    CSelfMatch<char> x6("abcdXabcd"s);
    assert (x6.sequenceLen(1) == 9);
    assert (positionMatch(x6.findSequences<1>(), std::vector<std::array<size_t, 1> >{{0}}));
    CSelfMatch<char> x7("abcdXabcd"s);
    assert (x7.sequenceLen(2) == 4);
    assert (positionMatch(x7.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 5}}));
    CSelfMatch<char> x8("abcdXabcdeYabcdZabcd"s);
    assert (x8.sequenceLen(2) == 4);
    assert (positionMatch(x8.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0,  5},
                                                                                     {0,  11},
                                                                                     {0,  16},
                                                                                     {5,  11},
                                                                                     {5,  16},
                                                                                     {11, 16}}));
    CSelfMatch<char> x9("abcdXabcdYabcd"s);
    assert (x9.sequenceLen(3) == 4);
    assert (positionMatch(x9.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 5, 10}}));
    CSelfMatch<char> x10("abcdefghijklmn"s);
    assert (x10.sequenceLen(2) == 0);
    assert (positionMatch(x10.findSequences<2>(), std::vector<std::array<size_t, 2> >{}));
    CSelfMatch<char> x11("abcXabcYabcZdefXdef"s);
    assert (x11.sequenceLen(2) == 3);
    assert (positionMatch(x11.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0,  4},
                                                                                      {0,  8},
                                                                                      {4,  8},
                                                                                      {12, 16}}));
    CSelfMatch<int> x12{1, 2, 3, 1, 2, 4, 1, 2};
    assert (x12.sequenceLen(2) == 2);
    assert (positionMatch(x12.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 3},
                                                                                      {0, 6},
                                                                                      {3, 6}}));
    assert (x12.sequenceLen(3) == 2);
    assert (positionMatch(x12.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 3, 6}}));
    std::initializer_list<CDummy> init13{'a', 'b', 'c', 'd', 'X', 'a', 'b', 'c', 'd', 'Y', 'a', 'b', 'c', 'd'};
    CSelfMatch<CDummy> x13(init13.begin(), init13.end());
    assert (x13.sequenceLen(2) == 4);
    assert (positionMatch(x13.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 5},
                                                                                      {0, 10},
                                                                                      {5, 10}}));
    std::initializer_list<int> init14{1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2,
                                      1, 0};
    CSelfMatch<int> x14(init14.begin(), init14.end());
    assert (x14.sequenceLen(2) == 5);
    assert (positionMatch(x14.findSequences<2>(), std::vector<std::array<size_t, 2> >{{11, 14},
                                                                                      {7,  24}}));
    std::initializer_list<int> init15{1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2,
                                      1, 0};
    CSelfMatch<int> x15(init15.begin(), init15.end());
    assert (x15.sequenceLen(3) == 4);
    assert (positionMatch(x15.findSequences<3>(), std::vector<std::array<size_t, 3> >{{3, 8, 25}}));
#ifndef TEST_EXTRA_INTERFACE
    CSelfMatch y0("aaaaaaaaaaa"s);
    assert (y0.sequenceLen(2) == 10);

    std::string s1("abcd");
    CSelfMatch y1(s1.begin(), s1.end());
    assert (y1.sequenceLen(2) == 0);

    CSelfMatch y2(""s);
    y2.push_back('a', 'b', 'c', 'X');
    y2.push_back('a');
    y2.push_back('b', 'c');
    assert (y2.sequenceLen(2) == 3);
#endif /* TEST_EXTRA_INTERFACE */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */