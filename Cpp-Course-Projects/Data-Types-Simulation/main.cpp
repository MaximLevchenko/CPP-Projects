#ifndef PROGTEST

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
#endif /* PROGTEST */

/**
 * Abstract base class representing a generic data type.
 * This class provides an interface for derived data type classes to implement.
 */
class AbstractCLass
{
public:
    /**
     * Virtual destructor for safe polymorphic deletion.
     */
    virtual ~AbstractCLass() = default;

    /**
     * Pure virtual function to get the size of the data type.
     * @return Size of the data type in bytes.
     */
    virtual size_t getSize() const = 0;

    /**
     * Pure virtual function to get the name of the data type.
     * @return The name of the data type as a string.
     */
    virtual string getType() const = 0;

    /**
     * Pure virtual function to print the type.
     * @param os The output stream to print to.
     */
    virtual void print_type(ostream &os) const = 0;

    /**
     * Pure virtual function to check equality with another object.
     * @param obj The other object to compare with.
     * @return True if the objects are equal, false otherwise.
     */
    virtual bool check_equal(const AbstractCLass &obj) const = 0;

    /**
     * Pure virtual function to clone the object.
     * @return A pointer to the cloned object.
     */
    virtual AbstractCLass *clone() const = 0;

    /**
     * Virtual operator== to compare two data types.
     * @param obj The other object to compare with.
     * @return True if the objects are equal, false otherwise.
     */
    virtual bool operator==(const AbstractCLass &obj) const;

    /**
     * Virtual operator!= to compare two data types.
     * @param obj The other object to compare with.
     * @return True if the objects are not equal, false otherwise.
     */
    virtual bool operator!=(const AbstractCLass &obj) const;
};

/**
 * Overloaded output operator to print the type.
 * @param os The output stream to print to.
 * @param obj The object whose type is to be printed.
 * @return The output stream.
 */
ostream &operator<<(ostream &os, const AbstractCLass &obj)
{
    obj.print_type(os);
    return os;
}

/**
 * Definition of operator== to check if two objects are of the same type and have the same data.
 * @param obj The other object to compare with.
 * @return True if the objects are of the same type and have the same data, false otherwise.
 */
bool AbstractCLass::operator==(const AbstractCLass &obj) const
{
    return typeid(*this).name() == typeid(obj).name() && check_equal(obj);
}

/**
 * Definition of operator!= to check if two objects are not equal.
 * @param obj The other object to compare with.
 * @return True if the objects are not equal, false otherwise.
 */
bool AbstractCLass::operator!=(const AbstractCLass &obj) const
{
    return !(typeid(*this).name() == typeid(obj).name() && check_equal(obj));
}

/**
 * Derived class representing the 'int' data type.
 */
class CDataTypeInt : public AbstractCLass
{
public:
    /**
     * Default constructor for CDataTypeInt.
     */
    CDataTypeInt() = default;

    /**
     * Override clone method to return a new instance of CDataTypeInt.
     * @return A pointer to the cloned CDataTypeInt object.
     */
    AbstractCLass *clone() const override { return new CDataTypeInt(*this); }

    /**
     * Override getSize to return size of int (4 bytes).
     * @return Size of int data type.
     */
    size_t getSize() const override { return 4; }

    /**
     * Override getType to return type name "int".
     * @return The name of the data type as a string.
     */
    string getType() const override { return "int"; }

    /**
     * Override print_type to print "int".
     * @param os The output stream to print to.
     */
    void print_type(ostream &os) const override { os << "int"; }

    /**
     * Override check_equal to compare types.
     * @param obj The other object to compare with.
     * @return True if the objects are of the same type, false otherwise.
     */
    bool check_equal(const AbstractCLass &obj) const override { return typeid(*this).name() == typeid(obj).name(); }
};

/**
 * Derived class representing the 'double' data type.
 */
class CDataTypeDouble : public AbstractCLass
{
public:
    /**
     * Default constructor for CDataTypeDouble.
     */
    CDataTypeDouble() = default;

    /**
     * Override clone method to return a new instance of CDataTypeDouble.
     * @return A pointer to the cloned CDataTypeDouble object.
     */
    AbstractCLass *clone() const override { return new CDataTypeDouble(*this); }

    /**
     * Override getSize to return size of double (8 bytes).
     * @return Size of double data type.
     */
    size_t getSize() const override { return 8; }

    /**
     * Override getType to return type name "double".
     * @return The name of the data type as a string.
     */
    string getType() const override { return "double"; }

    /**
     * Override check_equal to compare types.
     * @param obj The other object to compare with.
     * @return True if the objects are of the same type, false otherwise.
     */
    bool check_equal(const AbstractCLass &obj) const override { return typeid(*this).name() == typeid(obj).name(); }

    /**
     * Override print_type to print "double".
     * @param os The output stream to print to.
     */
    void print_type(ostream &os) const override { os << "double"; }
};

/**
 * Derived class representing an 'enum' data type.
 */
class CDataTypeEnum : public AbstractCLass
{
public:
    /**
     * Default constructor for CDataTypeEnum.
     */
    CDataTypeEnum() = default;

    /**
     * Copy constructor to clone str_vec.
     * @param obj The CDataTypeEnum object to copy from.
     */
    CDataTypeEnum(const CDataTypeEnum &obj) : str_vec(obj.str_vec) {}

    /**
     * Override clone method to return a new instance of CDataTypeEnum.
     * @return A pointer to the cloned CDataTypeEnum object.
     */
    AbstractCLass *clone() const override { return new CDataTypeEnum(*this); }

    /**
     * Override getSize to return size of enum (4 bytes).
     * @return Size of enum data type.
     */
    size_t getSize() const override { return 4; }

    /**
     * Override getType to return type name "enum".
     * @return The name of the data type as a string.
     */
    string getType() const override { return "enum"; }

    /**
     * Override print_type to print enum details.
     * @param os The output stream to print to.
     */
    void print_type(ostream &os) const override;

    /**
     * Override check_equal to compare enums.
     * @param obj The other object to compare with.
     * @return True if the objects are of the same type and have the same values, false otherwise.
     */
    bool check_equal(const AbstractCLass &obj) const override;

    /**
     * Method to add a new value to the enum.
     * @param name The name of the value to add.
     * @return A reference to the updated CDataTypeEnum object.
     * @throws invalid_argument if the value already exists in the enum.
     */
    CDataTypeEnum &add(const string &name);

protected:
    vector<string> str_vec; /**< Vector to store enum values */
};

/**
 * Adds a value to the enum, throws an exception if the value already exists.
 * @param name The name of the value to add.
 * @return A reference to the updated CDataTypeEnum object.
 * @throws invalid_argument if the value already exists in the enum.
 */
CDataTypeEnum &CDataTypeEnum::add(const string &name)
{
    if (std::find(str_vec.begin(), str_vec.end(), name) == str_vec.end())
        str_vec.push_back(name);
    else
        throw invalid_argument("Duplicate enum value: " + name);
    return *this;
}

/**
 * Prints the enum type and its values.
 * @param os The output stream to print to.
 */
void CDataTypeEnum::print_type(ostream &os) const
{
    os << getType() << "\n{" << endl;
    auto vec_end = str_vec.end();
    --vec_end;
    auto x = str_vec.begin();
    for (; x != vec_end; ++x)
    {
        os << *x << "," << endl;
    }
    os << *x << endl;
    os << "}";
}

/**
 * Checks equality of two enums by comparing their values.
 * @param obj The other object to compare with.
 * @return True if the objects are of the same type and have the same values, false otherwise.
 */
bool CDataTypeEnum::check_equal(const AbstractCLass &obj) const
{
    if (this->str_vec.size() != dynamic_cast<const CDataTypeEnum &>(obj).str_vec.size())
        return false;
    for (size_t i = 0; i < str_vec.size(); i++)
    {
        if (str_vec[i] != dynamic_cast<const CDataTypeEnum &>(obj).str_vec[i])
            return false;
    }
    return true;
}

/**
 * Derived class representing a 'struct' data type.
 */
class CDataTypeStruct : public AbstractCLass
{
public:
    /**
     * Default constructor for CDataTypeStruct.
     */
    CDataTypeStruct() = default;

    /**
     * Copy constructor for deep copying of the structure.
     * @param obj The CDataTypeStruct object to copy from.
     */
    CDataTypeStruct(const CDataTypeStruct &obj) : vec_abstract(obj.vec_abstract)
    {
        for (auto x = obj.map_abstract.begin(); x != obj.map_abstract.end(); ++x)
        {
            map_abstract.insert(make_pair(x->first, obj.map_abstract.at(x->first)->clone()));
        }
    }

    /**
     * Override clone method to return a new instance of CDataTypeStruct.
     * @return A pointer to the cloned CDataTypeStruct object.
     */
    AbstractCLass *clone() const override { return new CDataTypeStruct(*this); }

    /**
     * Override getSize to return size of the struct.
     * @return Size of struct data type.
     */
    size_t getSize() const override;

    /**
     * Override getType to return type name "struct".
     * @return The name of the data type as a string.
     */
    string getType() const override { return "struct"; }

    /**
     * Override print_type to print struct details.
     * @param os The output stream to print to.
     */
    void print_type(ostream &os) const override;

    /**
     * Override check_equal to compare structs.
     * @param obj The other object to compare with.
     * @return True if the objects are of the same type and have the same fields, false otherwise.
     */
    bool check_equal(const AbstractCLass &obj) const override;

    /**
     * Method to add a new field to the struct.
     * @param name The name of the field.
     * @param obj The data type of the field.
     * @return A reference to the updated CDataTypeStruct object.
     * @throws invalid_argument if a field with the same name already exists.
     */
    CDataTypeStruct &addField(const string &name, const AbstractCLass &obj);

    /**
     * Method to access a field by name.
     * @param field_find The name of the field to find.
     * @return A reference to the AbstractCLass representing the field's data type.
     * @throws invalid_argument if the field does not exist.
     */
    AbstractCLass &field(const string &field_find) const;

protected:
    unordered_map<string, unique_ptr<AbstractCLass>> map_abstract; /**< Map to store fields by name */
    vector<string> vec_abstract; /**< Vector to store field names in order of addition */
};

/**
 * Adds a field to the struct, throws an exception if the field already exists.
 * @param name The name of the field.
 * @param obj The data type of the field.
 * @return A reference to the updated CDataTypeStruct object.
 * @throws invalid_argument if a field with the same name already exists.
 */
CDataTypeStruct &CDataTypeStruct::addField(const string &name, const AbstractCLass &obj)
{
    if (map_abstract.count(name) == 0)
    {
        vec_abstract.push_back(name);
        map_abstract.insert(make_pair(name, unique_ptr<AbstractCLass>(obj.clone())));
    }
    else
        throw invalid_argument("Duplicate field: " + name);
    return *this;
}

/**
 * Prints the struct type and its fields.
 * @param os The output stream to print to.
 */
void CDataTypeStruct::print_type(ostream &os) const
{
    os << "struct\n{\n";
    for (auto const &x : vec_abstract)
    {
        map_abstract.find(x)->second->print_type(os);
        os << " " << map_abstract.find(x)->first << ";" << endl;
    }
    os << "}";
}

/**
 * Returns the total size of the struct by summing the sizes of its fields.
 * @return Total size of the struct.
 */
size_t CDataTypeStruct::getSize() const
{
    size_t res = 0;
    for (const auto &x : map_abstract)
    {
        res += x.second->getSize();
    }
    return res;
}

/**
 * Checks equality of two structs by comparing their fields and types.
 * @param obj The other object to compare with.
 * @return True if the objects are of the same type and have the same fields, false otherwise.
 */
bool CDataTypeStruct::check_equal(const AbstractCLass &obj) const
{
    if (this->getSize() != obj.getSize())
        return false;
    auto obj_vec_it = dynamic_cast<const CDataTypeStruct &>(obj).vec_abstract.begin();

    for (auto x = vec_abstract.begin(); x != vec_abstract.end(); ++x, ++obj_vec_it)
    {
        if (map_abstract.at(*x)->getType() ==
            dynamic_cast<const CDataTypeStruct &>(obj).map_abstract.at(*obj_vec_it)->getType() &&
            *map_abstract.at(*x) == *dynamic_cast<const CDataTypeStruct &>(obj).map_abstract.at(*obj_vec_it))
            continue;
        else
            return false;
    }
    return true;
}

/**
 * Accesses a field by name, throws an exception if the field does not exist.
 * @param field_find The name of the field to find.
 * @return A reference to the AbstractCLass representing the field's data type.
 * @throws invalid_argument if the field does not exist.
 */
AbstractCLass &CDataTypeStruct::field(const string &field_find) const
{
    if (map_abstract.count(field_find) == 0)
        throw invalid_argument("Unknown field: " + field_find);
    else
        return *map_abstract.at(field_find);
}

#ifndef PROGTEST

/**
 * Helper function to compare two strings ignoring whitespace.
 * @param a The first string to compare.
 * @param b The second string to compare.
 * @return True if the strings are equal ignoring whitespace, false otherwise.
 */
static bool whitespaceMatch(const string &a, const string &b)
{
    string temp1 = a;
    string temp2 = b;
    std::string::iterator end_pos1 = std::remove(temp1.begin(), temp1.end(), ' ');
    temp1.erase(end_pos1, temp1.end());
    std::string::iterator end_pos2 = std::remove(temp2.begin(), temp2.end(), ' ');
    temp2.erase(end_pos2, temp2.end());
    cout << temp1 << endl;
    cout << temp2 << endl;
    return temp1 == temp2;
}

/**
 * Template version of whitespaceMatch for comparing objects with a reference string.
 * @tparam T_ Type of the object to compare.
 * @param x The object to compare.
 * @param ref The reference string to compare with.
 * @return True if the object's string representation matches the reference string ignoring whitespace, false otherwise.
 */
template <typename T_>
static bool whitespaceMatch(const T_ &x, const string &ref)
{
    ostringstream oss;
    oss << x;
    return whitespaceMatch(oss.str(), ref);
}

/**
 * Main function to demonstrate usage and perform tests.
 * @return EXIT_SUCCESS on successful execution.
 */
int main(void)
{
    CDataTypeStruct a = CDataTypeStruct().addField("m_Length",
                                                   CDataTypeInt())
            .addField("m_Status",
                      CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("DEAD"))
            .addField("m_Ratio",
                      CDataTypeDouble());

    CDataTypeStruct b = CDataTypeStruct().addField("m_Length",
                                                   CDataTypeInt())
            .addField("m_Status",
                      CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("READY"))
            .addField("m_Ratio",
                      CDataTypeDouble());

    CDataTypeStruct c = CDataTypeStruct().addField("m_First",
                                                   CDataTypeInt())
            .addField("m_Second",
                      CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("DEAD"))
            .addField("m_Third",
                      CDataTypeDouble());

    CDataTypeStruct d = CDataTypeStruct().addField("m_Length",
                                                   CDataTypeInt())
            .addField("m_Status",
                      CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("DEAD"))
            .addField("m_Ratio",
                      CDataTypeInt());

    // Perform various assertions to test functionality
    assert(whitespaceMatch(a, "struct\n"
                              "{\n"
                              "  int m_Length;\n"
                              "  enum\n"
                              "  {\n"
                              "    NEW,\n"
                              "    FIXED,\n"
                              "    BROKEN,\n"
                              "    DEAD\n"
                              "  } m_Status;\n"
                              "  double m_Ratio;\n"
                              "}"));

    assert(whitespaceMatch(b, "struct\n"
                              "{\n"
                              "  int m_Length;\n"
                              "  enum\n"
                              "  {\n"
                              "    NEW,\n"
                              "    FIXED,\n"
                              "    BROKEN,\n"
                              "    READY\n"
                              "  } m_Status;\n"
                              "  double m_Ratio;\n"
                              "}"));
    assert(whitespaceMatch(c, "struct\n"
                              "{\n"
                              "  int m_First;\n"
                              "  enum\n"
                              "  {\n"
                              "    NEW,\n"
                              "    FIXED,\n"
                              "    BROKEN,\n"
                              "    DEAD\n"
                              "  } m_Second;\n"
                              "  double m_Third;\n"
                              "}"));

    assert(whitespaceMatch(d, "struct\n"
                              "{\n"
                              "  int m_Length;\n"
                              "  enum\n"
                              "  {\n"
                              "    NEW,\n"
                              "    FIXED,\n"
                              "    BROKEN,\n"
                              "    DEAD\n"
                              "  } m_Status;\n"
                              "  int m_Ratio;\n"
                              "}"));

    assert(a != b);
    assert(a == c);
    assert(a != d);
    assert(a.field("m_Status") ==
           CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("DEAD"));
    assert(a.field("m_Status") !=
           CDataTypeEnum().add("NEW").add("BROKEN").add("FIXED").add("DEAD"));
    assert(a != CDataTypeInt());
    assert(whitespaceMatch(a.field("m_Status"), "enum\n"
                                                "{\n"
                                                "  NEW,\n"
                                                "  FIXED,\n"
                                                "  BROKEN,\n"
                                                "  DEAD\n"
                                                "}"));

    CDataTypeStruct aOld = a;
    b.addField("m_Other", CDataTypeDouble());

    a.addField("m_Sum", CDataTypeInt());

    assert(a != aOld);
    assert(a != c);
    assert(aOld == c);
    assert(whitespaceMatch(a, "struct\n"
                              "{\n"
                              "  int m_Length;\n"
                              "  enum\n"
                              "  {\n"
                              "    NEW,\n"
                              "    FIXED,\n"
                              "    BROKEN,\n"
                              "    DEAD\n"
                              "  } m_Status;\n"
                              "  double m_Ratio;\n"
                              "  int m_Sum;\n"
                              "}"));

    assert(whitespaceMatch(b, "struct\n"
                              "{\n"
                              "  int m_Length;\n"
                              "  enum\n"
                              "  {\n"
                              "    NEW,\n"
                              "    FIXED,\n"
                              "    BROKEN,\n"
                              "    READY\n"
                              "  } m_Status;\n"
                              "  double m_Ratio;\n"
                              "  double m_Other;\n"
                              "}"));

    c.addField("m_Another", a.field("m_Status"));
    assert(whitespaceMatch(c, "struct\n"
                              "{\n"
                              "  int m_First;\n"
                              "  enum\n"
                              "  {\n"
                              "    NEW,\n"
                              "    FIXED,\n"
                              "    BROKEN,\n"
                              "    DEAD\n"
                              "  } m_Second;\n"
                              "  double m_Third;\n"
                              "  enum\n"
                              "  {\n"
                              "    NEW,\n"
                              "    FIXED,\n"
                              "    BROKEN,\n"
                              "    DEAD\n"
                              "  } m_Another;\n"
                              "}"));

    d.addField("m_Another", a.field("m_Ratio"));

    assert(whitespaceMatch(d, "struct\n"
                              "{\n"
                              "  int m_Length;\n"
                              "  enum\n"
                              "  {\n"
                              "    NEW,\n"
                              "    FIXED,\n"
                              "    BROKEN,\n"
                              "    DEAD\n"
                              "  } m_Status;\n"
                              "  int m_Ratio;\n"
                              "  double m_Another;\n"
                              "}"));

    assert(a.getSize() == 20);
    assert(b.getSize() == 24);
    try
    {
        a.addField("m_Status", CDataTypeInt());
        assert("addField: missing exception!" == nullptr);
    }
    catch (const invalid_argument &e)
    {
        assert(e.what() == "Duplicate field: m_Status"s);
    }

    try
    {
        cout << a.field("m_Fail") << endl;
        assert("field: missing exception!" == nullptr);
    }
    catch (const invalid_argument &e)
    {
        assert(e.what() == "Unknown field: m_Fail"s);
    }

    try
    {
        CDataTypeEnum en;
        en.add("FIRST").add("SECOND").add("FIRST");
        assert("add: missing exception!" == nullptr);
    }
    catch (const invalid_argument &e)
    {
        assert(e.what() == "Duplicate enum value: FIRST"s);
    }

    return EXIT_SUCCESS;
}

#endif /* PROGTEST */
