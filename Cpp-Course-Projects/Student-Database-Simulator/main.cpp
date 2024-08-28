#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <utility>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <compare>

class CDate {
public:
    /**
     * Constructor to initialize the date with year, month, and day.
     * @param y Year of the date.
     * @param m Month of the date.
     * @param d Day of the date.
     */
    CDate(int y, int m, int d)
            : m_Y(y), m_M(m), m_D(d) {}

    /**
     * Operator to compare two CDate objects.
     * @param other Another CDate object to compare with.
     * @return std::strong_ordering indicating comparison result.
     */
    std::strong_ordering operator<=>(const CDate &other) const = default;

    /**
     * Output stream operator to print the date in "YYYY-MM-DD" format.
     * @param os Output stream.
     * @param d CDate object to print.
     * @return Output stream with formatted date.
     */
    friend std::ostream &operator<<(std::ostream &os, const CDate &d) {
        return os << d.m_Y << '-' << d.m_M << '-' << d.m_D;
    }

private:
    int m_Y;
    int m_M;
    int m_D;
};

enum class ESortKey {
    NAME,
    BIRTH_DATE,
    ENROLL_YEAR
};

#endif /* __PROGTEST__ */

class CStudent {
public:
    static inline int s_NextID = 0;

    /**
     * Constructor to initialize a student with a name, date of birth, and enrollment year.
     * @param name Student's name.
     * @param dateOfBirth Student's date of birth.
     * @param enrollYear Student's enrollment year.
     */
    CStudent(std::string name, const CDate &dateOfBirth, int enrollYear)
            : m_Name(std::move(name)), m_DateOfBirth(dateOfBirth), m_EnrollYear(enrollYear), m_ID(s_NextID++) {
        m_NormalizedName = normalizeName(m_Name);
    }

    /**
     * Normalizes a student's name for consistent comparison and sorting.
     * Converts all letters to lowercase and sorts the words.
     * @param name Student's name to normalize.
     * @return Normalized name string.
     */
    static std::string normalizeName(const std::string &name) {
        std::istringstream iss(name);
        std::vector<std::string> words;
        std::string word;
        while (iss >> word) {
            std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); });
            words.push_back(word);
        }
        std::sort(words.begin(), words.end());
        std::string normalized;
        for (const auto &w : words) {
            if (!normalized.empty()) normalized += " ";
            normalized += w;
        }
        return normalized;
    }

    /**
     * Comparison operator to check if two students are equal.
     * Compares names, dates of birth, and enrollment years.
     * @param other Another CStudent object to compare with.
     * @return True if all attributes are equal, false otherwise.
     */
    bool operator==(const CStudent &other) const {
        return m_Name == other.m_Name && m_DateOfBirth == other.m_DateOfBirth && m_EnrollYear == other.m_EnrollYear;
    }

    /**
     * Comparison operator to check if two students are not equal.
     * @param other Another CStudent object to compare with.
     * @return True if any attribute differs, false otherwise.
     */
    bool operator!=(const CStudent &other) const {
        return !(*this == other);
    }

    /**
     * Output stream operator to print student details.
     * @param os Output stream.
     * @param student CStudent object to print.
     * @return Output stream with formatted student details.
     */
    friend std::ostream &operator<<(std::ostream &os, const CStudent &student) {
        os << "Student: " << student.m_Name << ", Date of Birth: " << student.m_DateOfBirth
           << ", Enroll Year: " << student.m_EnrollYear;
        return os;
    }

    int getID() const { return m_ID; }
    const std::string &getName() const { return m_Name; }
    const std::string &getNormalizedName() const { return m_NormalizedName; }
    const CDate &getDateOfBirth() const { return m_DateOfBirth; }
    int getEnrollYear() const { return m_EnrollYear; }

private:
    std::string m_Name;
    std::string m_NormalizedName;
    CDate m_DateOfBirth;
    int m_EnrollYear;
    int m_ID; // Unique ID for each student
};

class CFilter {
public:
    /**
     * Constructor to initialize the filter with default values.
     * All criteria are initially set to be inclusive of any student.
     */
    CFilter()
            : m_BornBefore(CDate(0, 0, 0)),
              m_BornAfter(CDate(0, 0, 0)),
              hasBornBefore(false),
              hasBornAfter(false),
              hasEnrolledBefore(false),
              hasEnrolledAfter(false) {}

    /**
     * Sets the filter to only include students born before the given date.
     * @param date The cutoff date for student birthdates.
     * @return Reference to the modified filter.
     */
    CFilter &bornBefore(const CDate &date) {
        m_BornBefore = date;
        hasBornBefore = true;
        return *this;
    }

    /**
     * Sets the filter to only include students born after the given date.
     * @param date The cutoff date for student birthdates.
     * @return Reference to the modified filter.
     */
    CFilter &bornAfter(const CDate &date) {
        m_BornAfter = date;
        hasBornAfter = true;
        return *this;
    }

    /**
     * Sets the filter to only include students enrolled before the given year.
     * @param year The cutoff year for student enrollments.
     * @return Reference to the modified filter.
     */
    CFilter &enrolledBefore(int year) {
        m_EnrolledBefore = year;
        hasEnrolledBefore = true;
        return *this;
    }

    /**
     * Sets the filter to only include students enrolled after the given year.
     * @param year The cutoff year for student enrollments.
     * @return Reference to the modified filter.
     */
    CFilter &enrolledAfter(int year) {
        m_EnrolledAfter = year;
        hasEnrolledAfter = true;
        return *this;
    }

    /**
     * Adds a name pattern to the filter. Students' names must match this pattern.
     * @param namePattern The name pattern to match.
     * @return Reference to the modified filter.
     */
    CFilter &name(const std::string &namePattern) {
        m_NormalizedNames.insert(CStudent::normalizeName(namePattern));
        m_NamePatterns.push_back(CStudent::normalizeName(namePattern));
        return *this;
    }

    // Getter and utility functions to check if specific filters are set
    bool hasNameFilter() const { return !m_NamePatterns.empty(); }
    bool matchesDateOfBirth(const CStudent &student) const {
        if (hasBornBefore && !(student.getDateOfBirth() < m_BornBefore)) return false;
        if (hasBornAfter && !(student.getDateOfBirth() > m_BornAfter)) return false;
        return true;
    }

    bool matchesEnrollmentYear(const CStudent &student) const {
        if (hasEnrolledBefore && !(student.getEnrollYear() < m_EnrolledBefore)) return false;
        if (hasEnrolledAfter && !(student.getEnrollYear() > m_EnrolledAfter)) return false;
        return true;
    }

    bool matchesName(const std::string &normalizedStudentName) const {
        return m_NormalizedNames.find(normalizedStudentName) != m_NormalizedNames.end();
    }

    bool matches(const CStudent &student) const {
        if (hasBornBefore && !(student.getDateOfBirth() < m_BornBefore)) return false;
        if (hasBornAfter && !(student.getDateOfBirth() > m_BornAfter)) return false;
        if (hasEnrolledBefore && !(student.getEnrollYear() < m_EnrolledBefore)) return false;
        if (hasEnrolledAfter && !(student.getEnrollYear() > m_EnrolledAfter)) return false;

        if (!m_NamePatterns.empty()) {
            auto studentWordCounts = normalizeAndCountWords(student.getName());
            for (const auto &pattern : m_NamePatterns) {
                auto patternWordCounts = normalizeAndCountWords(pattern);
                if (studentWordCounts == patternWordCounts) {
                    return true;
                }
            }
            return false;
        }

        return true;
    }

    // Helper function to normalize and count word occurrences in a string
    std::map<std::string, int> normalizeAndCountWords(const std::string &input) const {
        std::map<std::string, int> wordCount;
        std::istringstream stream(input);
        std::string word;
        while (stream >> word) {
            std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); });
            wordCount[word]++;
        }
        return wordCount;
    }

    // Additional utility functions
    bool isHasBornBefore() const { return hasBornBefore; }
    bool isHasBornAfter() const { return hasBornAfter; }
    bool isHasEnrolledBefore() const { return hasEnrolledBefore; }
    bool isHasEnrolledAfter() const { return hasEnrolledAfter; }
    const CDate &getMBornBefore() const { return m_BornBefore; }
    const CDate &getMBornAfter() const { return m_BornAfter; }
    int getMEnrolledBefore() const { return m_EnrolledBefore; }
    int getMEnrolledAfter() const { return m_EnrolledAfter; }

private:
    CDate m_BornBefore, m_BornAfter;
    int m_EnrolledBefore, m_EnrolledAfter;
    std::vector<std::string> m_NamePatterns;
    bool hasBornBefore = false, hasBornAfter = false, hasEnrolledBefore = false, hasEnrolledAfter = false;
    std::set<std::string> m_NormalizedNames;
};

class CSort {
public:
    /**
     * Adds a sorting key with the specified order (ascending or descending).
     * @param key The sorting key (name, birth date, or enrollment year).
     * @param ascending Boolean indicating whether to sort in ascending order.
     * @return Reference to the modified CSort object.
     */
    CSort &addKey(ESortKey key, bool ascending = true) {
        m_SortingKeys.emplace_back(key, ascending);
        return *this;
    }

    bool isEmpty() const { return m_SortingKeys.empty(); }

    ESortKey getPrimarySortKey() const {
        if (!m_SortingKeys.empty()) {
            return m_SortingKeys.front().first;
        }
        throw std::logic_error("Sorting keys are empty");
    }

    /**
     * Sorts a vector of students according to the specified sorting keys.
     * @param students Vector of students to be sorted.
     */
    void applySort(std::vector<CStudent> &students) const {
        std::sort(students.begin(), students.end(), [this](const CStudent &a, const CStudent &b) {
            return compare(a, b);
        });
    }

private:
    std::vector<std::pair<ESortKey, bool>> m_SortingKeys;

    // Compares two students based on the sorting keys defined in the sort object.
    bool compare(const CStudent &a, const CStudent &b) const {
        for (const auto &[key, ascending] : m_SortingKeys) {
            switch (key) {
                case ESortKey::NAME:
                    if (a.getName() != b.getName())
                        return ascending ? a.getName() < b.getName() : a.getName() > b.getName();
                    break;
                case ESortKey::BIRTH_DATE:
                    if (a.getDateOfBirth() != b.getDateOfBirth())
                        return ascending ? a.getDateOfBirth() < b.getDateOfBirth()
                                         : a.getDateOfBirth() > b.getDateOfBirth();
                    break;
                case ESortKey::ENROLL_YEAR:
                    if (a.getEnrollYear() != b.getEnrollYear())
                        return ascending ? a.getEnrollYear() < b.getEnrollYear()
                                         : a.getEnrollYear() > b.getEnrollYear();
                    break;
            }
        }
        return false;
    }
};

// Comparison structures for sorting and organizing student records
struct CompareByName {
    bool operator()(const std::shared_ptr<CStudent> &lhs, const std::shared_ptr<CStudent> &rhs) const {
        if (lhs->getName() != rhs->getName()) {
            return lhs->getName() < rhs->getName();
        }
        return lhs->getID() < rhs->getID();
    }
};

struct CompareByBirthDate {
    bool operator()(const std::shared_ptr<CStudent> &lhs, const std::shared_ptr<CStudent> &rhs) const {
        if (lhs->getDateOfBirth() != rhs->getDateOfBirth()) {
            return lhs->getDateOfBirth() < rhs->getDateOfBirth();
        }
        return lhs->getID() < rhs->getID();
    }
};

struct CompareByEnrollYear {
    bool operator()(const std::shared_ptr<CStudent> &lhs, const std::shared_ptr<CStudent> &rhs) const {
        if (lhs->getEnrollYear() != rhs->getEnrollYear()) {
            return lhs->getEnrollYear() < rhs->getEnrollYear();
        }
        return lhs->getID() < rhs->getID();
    }
};

struct CompareStudent {
    bool operator()(const std::shared_ptr<CStudent> &lhs, const std::shared_ptr<CStudent> &rhs) const {
        if (lhs->getName() != rhs->getName()) {
            return lhs->getName() < rhs->getName();
        }
        if (lhs->getDateOfBirth() != rhs->getDateOfBirth()) {
            return lhs->getDateOfBirth() < rhs->getDateOfBirth();
        }
        return lhs->getEnrollYear() < rhs->getEnrollYear();
    }
};

class CStudyDept {
private:
    // Sets for managing unique students based on different attributes
    std::set<std::shared_ptr<CStudent>, CompareStudent> m_Students;
    std::set<std::shared_ptr<CStudent>, CompareByName> m_StudentsByName;
    std::set<std::shared_ptr<CStudent>, CompareByBirthDate> m_StudentsByBirthDate;
    std::set<std::shared_ptr<CStudent>, CompareByEnrollYear> m_StudentsByEnrollYear;
    std::list<std::shared_ptr<CStudent>> m_InsertionOrder; // List to track order of student insertion

public:
    /**
     * Adds a student to the study department's database.
     * @param student CStudent object to add.
     * @return True if the student was added successfully, false if a duplicate was found.
     */
    bool addStudent(const CStudent &student) {
        auto sharedStudent = std::make_shared<CStudent>(student);
        auto [iter, inserted] = m_Students.insert(sharedStudent);

        if (inserted) {
            m_StudentsByName.insert(sharedStudent);
            m_StudentsByBirthDate.insert(sharedStudent);
            m_StudentsByEnrollYear.insert(sharedStudent);
            m_InsertionOrder.push_back(sharedStudent);
        }

        return inserted;
    }

    /**
     * Deletes a student from the study department's database.
     * @param student CStudent object to delete.
     * @return True if the student was successfully deleted, false if not found.
     */
    bool delStudent(const CStudent &student) {
        auto it = std::find_if(m_Students.begin(), m_Students.end(),
                               [&student](const std::shared_ptr<CStudent> &ptr) { return *ptr == student; });

        if (it != m_Students.end()) {
            std::shared_ptr<CStudent> foundStudent = *it;
            m_Students.erase(it);
            m_StudentsByName.erase(foundStudent);
            m_StudentsByBirthDate.erase(foundStudent);
            m_StudentsByEnrollYear.erase(foundStudent);
            m_InsertionOrder.remove(foundStudent);
            return true;
        }

        return false;
    }

    /**
     * Searches for students that match the given filter and sort criteria.
     * @param filter CFilter object containing search criteria.
     * @param sort CSort object containing sorting criteria.
     * @return A list of students that match the filter, sorted according to the sort object.
     */
    std::list<CStudent> search(const CFilter &filter, const CSort &sort) const {
        std::list<CStudent> results;
        if (sort.isEmpty()) {
            if (filter.hasNameFilter()) {
                for (const auto &studentPtr : m_InsertionOrder) {
                    if (filter.matchesName(studentPtr->getNormalizedName()) &&
                        filter.matchesDateOfBirth(*studentPtr) &&
                        filter.matchesEnrollmentYear(*studentPtr)) {
                        results.push_back(*studentPtr);
                    }
                }
            } else if (filter.isHasBornBefore() || filter.isHasBornAfter()) {
                for (const auto &studentPtr : m_InsertionOrder) {
                    if (filter.matchesDateOfBirth(*studentPtr) && filter.matchesEnrollmentYear(*studentPtr)) {
                        results.push_back(*studentPtr);
                    }
                }
            } else if (filter.isHasEnrolledBefore() || filter.isHasEnrolledAfter()) {
                for (const auto &studentPtr : m_InsertionOrder) {
                    if (filter.matchesDateOfBirth(*studentPtr) && filter.matchesEnrollmentYear(*studentPtr)) {
                        results.push_back(*studentPtr);
                    }
                }
            } else {
                for (const auto &studentPtr : m_InsertionOrder) results.push_back(*studentPtr);
            }

            return results;
        }

        if (filter.hasNameFilter()) {
            for (const auto &studentPtr : m_StudentsByName) {
                if (filter.matchesName(studentPtr->getNormalizedName()) &&
                    filter.matchesDateOfBirth(*studentPtr) &&
                    filter.matchesEnrollmentYear(*studentPtr)) {
                    results.push_back(*studentPtr);
                }
            }
        } else if (filter.isHasBornBefore() || filter.isHasBornAfter()) {
            for (const auto &studentPtr : m_StudentsByName) {
                if (filter.matchesDateOfBirth(*studentPtr) && filter.matchesEnrollmentYear(*studentPtr)) {
                    results.push_back(*studentPtr);
                }
            }
        } else if (filter.isHasEnrolledBefore() || filter.isHasEnrolledAfter()) {
            for (const auto &studentPtr : m_StudentsByName) {
                if (filter.matchesDateOfBirth(*studentPtr) && filter.matchesEnrollmentYear(*studentPtr)) {
                    results.push_back(*studentPtr);
                }
            }
        } else {
            for (const auto &studentPtr : m_StudentsByName) results.push_back(*studentPtr);
        }

        if (!sort.isEmpty()) {
            std::vector<CStudent> vecResults(results.begin(), results.end());
            sort.applySort(vecResults);
            results.assign(vecResults.begin(), vecResults.end());
        }

        return results;
    }

    /**
     * Suggests names of students that match the given pattern.
     * @param pattern String pattern to match against student names.
     * @return A set of matching student names.
     */
    std::set<std::string> suggest(const std::string &pattern) const {
        std::set<std::string> results;
        auto patternWordCounts = normalizeAndCountWords(pattern);

        for (const auto &studentPtr : m_Students) {
            auto studentWordCounts = normalizeAndCountWords(studentPtr->getName());
            if (isSubset(studentWordCounts, patternWordCounts)) {
                results.insert(studentPtr->getName());
            }
        }
        return results;
    }

private:
    // Helper functions for name pattern matching and word counting
    std::map<std::string, int> normalizeAndCountWords(const std::string &input) const {
        std::map<std::string, int> wordCount;
        std::istringstream stream(input);
        std::string word;
        while (stream >> word) {
            std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); });
            wordCount[word]++;
        }
        return wordCount;
    }

    bool isSubset(const std::map<std::string, int> &studentWords, const std::map<std::string, int> &patternWords) const {
        for (const auto &[key, val] : patternWords) {
            if (studentWords.find(key) == studentWords.end()) {
                return false;
            }
        }
        return true;
    }
};

#ifndef __PROGTEST__
int main(void) {
    CStudyDept x0;
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2016)));
    assert (x0.addStudent(CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014)));
    assert (x0.addStudent(CStudent("John Taylor", CDate(1981, 6, 30), 2012)));
    assert (x0.addStudent(CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)));
    assert (x0.addStudent(CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1982, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 8, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 17), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2012)));
    assert (x0.addStudent(CStudent("Bond James", CDate(1981, 7, 16), 2013)));
    assert (x0.search(CFilter(), CSort()) == (std::list<CStudent>
            {
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    auto temp = x0.search(CFilter(), CSort().addKey(ESortKey::NAME, true));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, false)) == (std::list<CStudent>
            {
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    auto temp2 = x0.search(CFilter(),
                           CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, true).addKey(
                                   ESortKey::NAME, true));
    assert (x0.search(CFilter(),
                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                              ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));
    auto temm = x0.search(CFilter().name("james bond"),
                          CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                                  ESortKey::NAME, true));
    assert (x0.search(CFilter().name("james bond"),
                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                              ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012)
            }));
    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
            ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
            }));
    auto tempp = x0.search(CFilter().name("james"), CSort().addKey(ESortKey::NAME, true));
    assert (x0.search(CFilter().name("james"), CSort().addKey(ESortKey::NAME, true)) == (std::list<CStudent>
            {
            }));
    assert (x0.suggest("peter") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor",
                    "Peter Taylor"
            }));
    assert (x0.suggest("bond") == (std::set<std::string>
            {
                    "Bond James",
                    "James Bond"
            }));
    assert (x0.suggest("peter joHn") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }));
    assert (x0.suggest("peter joHn bond") == (std::set<std::string>
            {
            }));
    assert (x0.suggest("pete") == (std::set<std::string>
            {
            }));
    auto temp3 = x0.suggest("peter joHn PETER");

    assert (x0.suggest("peter joHn PETER") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }));
    assert (!x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
            ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
            }));
    assert (!x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
