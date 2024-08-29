#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>
#include <numeric>
#include <optional>
#include <variant>
#include <any>

using namespace std;
#endif /* __PROGTEST__ */

/**
 * Template class CIndex for indexing and searching sequences within a collection.
 * @tparam T_ Type of the collection to index (e.g., string, vector<T>, list<T>).
 * @tparam C_ Comparator type for custom element comparison, defaults to std::less.
 */
template<typename T_, typename C_ = less<typename T_::value_type>>
class CIndex
{
public:
    /**
     * Constructor to initialize the index with a collection and optional comparator.
     * @param source_type Collection of elements to index.
     * @param compare_type (Optional) Comparator for element comparison.
     */
    CIndex(const T_ &source_type, const C_ &compare_type = C_()) : source_type(source_type),
                                                                   compare_type(compare_type)
    {

    }

    /**
     * Destructor for CIndex.
     */
    ~CIndex() {}

    /**
     * Initializes the partial match table (pi array) for the KMP algorithm.
     * @param find_sequence The sequence to search for within the indexed collection.
     * @return Vector representing the partial match table.
     */
    vector<size_t> initializePi(const T_ &find_sequence) const
    {
        vector<size_t> pi_array;
        pi_array.push_back(0);
        size_t seq_size = find_sequence.size(); // Size for sequence we want to find
        size_t j = 0;
        size_t i = 1;
        auto it_i = find_sequence.begin();
        it_i++;
        auto it_j = find_sequence.begin();

        for (; i < seq_size;)
        {
            if ((!(compare_type(*it_i, *it_j)) && !(compare_type(*it_j, *it_i))))
            {
                pi_array.push_back(j + 1); // pi[i] = j+1;
                it_j++, i++;
                it_i++, j++;
            }
            else if (j == 0) //*it_i != *it_j && j == 0
            {
                pi_array.push_back(0);
                it_i++, i++;
            }
            else //*it_i != *it_j && j != 0
            {
                size_t temp_j = j;
                j = pi_array[j - 1];
                while (temp_j != j) // Decrementing it_j to the correct state
                {
                    it_j--;
                    temp_j--;
                }

            }
        }
        return pi_array;
    }

    /**
     * Searches for all occurrences of the specified sequence within the indexed collection.
     * @param find_sequence The sequence to search for.
     * @return Set of positions where the find_sequence is found.
     */
    set<size_t> search(const T_ &find_sequence) const
    {
        vector<size_t> pi_array = initializePi(find_sequence);

        set<size_t> set_pos;
        if (find_sequence.size() == 0)
        {
            for (size_t i = 0; i < source_type.size(); i++)
                set_pos.insert(i);
            return set_pos;
        }

        size_t k = 0, l = 0; // k for string, l for sequence
        size_t N = find_sequence.size(); // Size of the sequence we are looking for
        size_t M = source_type.size(); // Size of the string we are looking in
        auto it_k = source_type.begin(); // Iterator to the first element in the string we are looking in
        auto it_l = find_sequence.begin(); // Iterator to the first element in the sequence we are looking for

        while (k < M)
        {
            if ((!(compare_type(*it_k, *it_l)) && !(compare_type(*it_l, *it_k)))) // *it_k == *it_l
            {
                k++, l++, it_k++, it_l++;
                if (l == N) // We found the sequence
                {
                    size_t temp_l = l;
                    set_pos.insert(k - l); // Insert the position to the set
                    l = pi_array[l - 1];
                    while (temp_l != l)
                    {
                        it_l--, temp_l--;
                    }
                }
            }
            else if (l == 0) // *it_k != *it_l && l first element
            {
                k++, it_k++;
                if (k == M)
                    break;
            }
            else // *it_k != *it_l && l !first element
            {
                size_t temp_l = l;
                l = pi_array[l - 1];
                while (temp_l != l)
                {
                    it_l--, temp_l--;
                }
            }
        }
        return set_pos;
    }

private:
    T_ source_type; /**< The collection to be indexed. */
    C_ compare_type; /**< Comparator for custom element comparison. */
};

#ifndef __PROGTEST__

/**
 * Functor for comparing strings, optionally case-sensitive.
 */
class CStrComparator
{
public:
    /**
     * Constructor to initialize the comparator with case sensitivity.
     * @param caseSensitive Whether the comparison is case-sensitive.
     */
    CStrComparator(bool caseSensitive)
            : m_CaseSensitive(caseSensitive)
    {
    }

    /**
     * Comparison operator for string comparison.
     * @param a First string to compare.
     * @param b Second string to compare.
     * @return True if `a` is less than `b` based on case sensitivity, false otherwise.
     */
    bool operator()(const string &a, const string &b) const
    {
        return m_CaseSensitive ? strcasecmp(a.c_str(), b.c_str()) < 0 : a < b;
    }

    bool m_CaseSensitive; /**< Flag indicating if the comparison is case-sensitive. */
};

/**
 * Function to compare characters ignoring case.
 * @param a First character to compare.
 * @param b Second character to compare.
 * @return True if `a` is less than `b` ignoring case, false otherwise.
 */
bool upperCaseCompare(const char &a, const char &b)
{
    return toupper(a) < toupper(b);
}

int main(void)
{
    CIndex<string> t0("abcabcabc");
    set<size_t> r0 = t0.search("a");
    assert(r0 == (set<size_t>{0, 3, 6}));
    set<size_t> r1 = t0.search("abc");
    assert(r1 == (set<size_t>{0, 3, 6}));
    set<size_t> r2 = t0.search("abcabc");
    assert(r2 == (set<size_t>{0, 3}));

    CIndex<string> t1("abcababc");
    set<size_t> r3 = t1.search("a");
    assert(r3 == (set<size_t>{0, 3, 5}));
    set<size_t> r4 = t1.search("abc");
    assert(r4 == (set<size_t>{0, 5}));
    set<size_t> r5 = t1.search("abcabc");
    assert(r5 == (set<size_t>{}));

    CIndex<string> t2("kokokokoskokosokos");
    set<size_t> r6 = t2.search("kos");
    assert(r6 == (set<size_t>{6, 11, 15}));
    set<size_t> r7 = t2.search("kokos");
    assert(r7 == (set<size_t>{4, 9}));

    CIndex<string> t3("aaaaaaau aaauaaaau");
    set<size_t> r8 = t3.search("aa");
    assert(r8 == (set<size_t>{0, 1, 2, 3, 4, 5, 9, 10, 13, 14, 15}));
    set<size_t> r9 = t3.search("aaa");
    assert(r9 == (set<size_t>{0, 1, 2, 3, 4, 9, 13, 14}));
    set<size_t> r10 = t3.search("aaaa");
    assert(r10 == (set<size_t>{0, 1, 2, 3, 13}));

    CIndex<string> t4("automatIc authentication automotive auTOmation raut");
    set<size_t> r11 = t4.search("auto");
    assert(r11 == (set<size_t>{0, 25}));
    set<size_t> r12 = t4.search("aut");
    assert(r12 == (set<size_t>{0, 10, 25, 48}));
    set<size_t> r13 = t4.search("tic");
    assert(r13 == (set<size_t>{16}));
    set<size_t> r14 = t4.search("trunk");
    assert(r14 == (set<size_t>{}));
    set<size_t> r15 = t4.search("a");
    assert(r15 == (set<size_t>{0, 5, 10, 19, 25, 36, 41, 48}));
    set<size_t> r16 = t4.search("");
    assert(r16 ==
           (set<size_t>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                        50}));

    CIndex<string, bool (*) (const char &, const char &)> t5("automatIc authentication automotive auTOmation raut",
                                                             upperCaseCompare);
    set<size_t> r17 = t5.search("auto");
    assert(r17 == (set<size_t>{0, 25, 36}));
    set<size_t> r18 = t5.search("aut");
    assert(r18 == (set<size_t>{0, 10, 25, 36, 48}));
    set<size_t> r19 = t5.search("tic");
    assert(r19 == (set<size_t>{6, 16}));
    set<size_t> r20 = t5.search("trunk");
    assert(r20 == (set<size_t>{}));
    set<size_t> r21 = t5.search("a");
    assert(r21 == (set<size_t>{0, 5, 10, 19, 25, 36, 41, 48}));
    set<size_t> r22 = t5.search("");
    assert(r22 ==
           (set<size_t>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                        50}));

    CIndex<list<string>, CStrComparator> t6(
            list<string>{"Hello", "world", "test", "this", "foo", "TEsT", "this", "done"}, CStrComparator(false));
    set<size_t> r23 = t6.search(list<string>{"test", "this", "foo"});
    assert(r23 == (set<size_t>{2}));
    set<size_t> r24 = t6.search(list<string>{"test", "this"});
    assert(r24 == (set<size_t>{2}));

    CIndex<list<string>, CStrComparator> t7(
            list<string>{"Hello", "world", "test", "this", "foo", "TEsT", "this", "done"}, CStrComparator(true));
    set<size_t> r25 = t7.search(list<string>{"test", "this", "foo"});
    assert(r25 == (set<size_t>{2}));
    set<size_t> r26 = t7.search(list<string>{"test", "this"});
    assert(r26 == (set<size_t>{2, 5}));

    return 0;
}
#endif /* __PROGTEST__ */
