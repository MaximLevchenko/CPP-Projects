# Student Management System

This project implements a simple student management system for a university's study department. The system allows for the addition, deletion, and searching of student records in a database. Each student is represented by their name, date of birth, and year of enrollment. This project includes functionality for filtering, sorting, and suggesting students based on various criteria.

## Classes Overview

### 1. `CDate`

The `CDate` class represents a simple date object used to store students' dates of birth and for filtering based on birth dates.

- **Constructor**: `CDate(int y, int m, int d)` initializes an instance with the specified year, month, and day.
- **Operator `<=>`**: Implements three-way comparison for ordering dates.
- **Output Operator `<<`**: Outputs the date in `YYYY-MM-DD` format.

### 2. `ESortKey`

`ESortKey` is an enum class used to identify sorting criteria for students in the database.

- **Possible Values**:
    - `NAME`: Sort by student's name.
    - `BIRTH_DATE`: Sort by student's date of birth.
    - `ENROLL_YEAR`: Sort by the year the student enrolled.

### 3. `CStudent`

The `CStudent` class represents a single student in the database.

- **Constructor**: `CStudent(std::string name, const CDate &dateOfBirth, int enrollYear)` initializes an instance with the given name, date of birth, and enrollment year.
- **Operator `==`**: Compares two students for equality based on name, date of birth, and enrollment year.
- **Operator `!=`**: Compares two students for inequality.
- **Methods**:
    - `getID()`: Returns the unique ID of the student.
    - `getName()`: Returns the student's name.
    - `getNormalizedName()`: Returns the student's name in normalized form.
    - `getDateOfBirth()`: Returns the student's date of birth.
    - `getEnrollYear()`: Returns the student's year of enrollment.

### 4. `CFilter`

The `CFilter` class represents filtering criteria for searching students in the database.

- **Constructor**: Initializes an empty filter.
- **Methods**:
    - `bornBefore(const CDate &date)`: Adds a filter for students born before the given date.
    - `bornAfter(const CDate &date)`: Adds a filter for students born after the given date.
    - `enrolledBefore(int year)`: Adds a filter for students enrolled before the given year.
    - `enrolledAfter(int year)`: Adds a filter for students enrolled after the given year.
    - `name(const std::string &namePattern)`: Adds a filter for student names matching the given pattern.
    - `matches(const CStudent &student)`: Checks if a student meets the filter criteria.
    - **Helper Methods**: Normalize and count words in names for name-based filtering.

### 5. `CSort`

The `CSort` class represents sorting criteria for ordering students in search results.

- **Constructor**: Initializes an empty list of sorting criteria.
- **Method**:
    - `addKey(ESortKey key, bool ascending)`: Adds a sorting key with order indication.
    - `applySort(std::vector<CStudent> &students)`: Applies the sorting criteria to a list of students.

### 6. `CStudyDept`

The `CStudyDept` class represents the student database.

- **Constructor**: Initializes an empty student database.
- **Methods**:
    - `addStudent(const CStudent &student)`: Adds a student to the database, returns `true` if successful, `false` if the student is a duplicate.
    - `delStudent(const CStudent &student)`: Removes a student from the database, returns `true` if successful, `false` if the student was not found.
    - `search(const CFilter &filter, const CSort &sortOpt)`: Searches the database for students matching the filter criteria and returns a sorted list of students.
    - `suggest(const std::string &name)`: Suggests student names based on a partial name match.

## Features

- **Add and Delete Students**: Easily manage student records by adding new students or deleting existing ones.
- **Filter and Search**: Use multiple criteria to filter students based on their name, date of birth, and enrollment year.
- **Sorting**: Sort search results based on multiple criteria in ascending or descending order.
- **Name Suggestion**: Suggests student names that match a given pattern, regardless of the order of the words.

## Example Usage

```cpp
int main() {
    CStudyDept dept;
    dept.addStudent(CStudent("John Doe", CDate(1990, 5, 15), 2010));
    dept.addStudent(CStudent("Jane Smith", CDate(1992, 8, 22), 2011));

    CFilter filter;
    filter.bornBefore(CDate(1993, 1, 1));

    CSort sort;
    sort.addKey(ESortKey::NAME, true);

    auto results = dept.search(filter, sort);
    for (const auto &student : results) {
        std::cout << student << std::endl;
    }

    return 0;
}
```
## Compilation and Running

To compile the project, use a C++ compiler with C++11 (or later) support:

```bash
g++ -std=c++11 -o student_management main.cpp
```
To run the compiled program:

```bash
./student_management
```

## Conclusion
This student management system provides a robust solution for managing student records in a university's study department. It allows for efficient searching, filtering, sorting, and suggestion functionalities to manage and retrieve student information based on various criteria.
