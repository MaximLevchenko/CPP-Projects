#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <compare>
#include <stdexcept>

#endif /* __PROGTEST__ */
class CBigInt {
public:
    /**
     * Constructors and Destructors
     * These methods initialize the CBigInt objects.
     * - Default constructor initializes to 0.
     * - Integer constructor initializes with an integer value.
     * - String constructor initializes with a string representation of a number.
     * - Copy constructor uses the default behavior.
     */
    CBigInt() : num("0"), isNegative(false) {}
    CBigInt(int value);
    CBigInt(const std::string &value);
    CBigInt(const CBigInt &other) = default;

    /**
     * Assignment Operators
     * These operators assign new values to the CBigInt object.
     * - operator= assigns from another CBigInt object.
     * - operator= assigns from an integer.
     * - operator= assigns from a string representation of a number.
     */
    CBigInt &operator=(const CBigInt &rhs);
    CBigInt &operator=(int rhs);
    CBigInt &operator=(const std::string &rhs);
    void normalize();
    /**
     * Arithmetic Operators
     * These operators perform arithmetic operations on CBigInt objects.
     * - operator+ adds two numbers.
     * - operator* multiplies two numbers.
     * - operator+= adds a number to the current object and assigns the result.
     * - operator*= multiplies a number with the current object and assigns the result.
     * These operators work with combinations of CBigInt, int, and string types.
     */
    CBigInt operator+(const CBigInt &rhs) const;
    CBigInt operator+(int rhs) const;
    CBigInt operator+(const std::string &rhs) const;
    friend CBigInt operator+(int lhs, const CBigInt &rhs);
    friend CBigInt operator+(const std::string &lhs, const CBigInt &rhs);

    CBigInt &operator+=(const CBigInt &rhs);
    CBigInt operator+=(int rhs);
    CBigInt operator+=(const std::string &rhs);

    CBigInt operator*(const CBigInt &rhs) const;
    CBigInt operator*(int rhs) const;
    CBigInt operator*(const std::string &rhs) const;
    friend CBigInt operator*(int lhs, const CBigInt &rhs);
    friend CBigInt operator*(const std::string &lhs, const CBigInt &rhs);

    CBigInt &operator*=(const CBigInt &rhs);
    CBigInt operator*=(int rhs);
    CBigInt operator*=(const std::string &rhs);

    /**
     * Comparison Operators
     * These operators compare CBigInt objects with each other or with integers and strings.
     * - operator== checks for equality.
     * - operator!= checks for inequality.
     * - operator< checks if one number is less than the other.
     * - operator<= checks if one number is less than or equal to the other.
     * - operator> checks if one number is greater than the other.
     * - operator>= checks if one number is greater than or equal to the other.
     * These operators work with combinations of CBigInt, int, and string types.
     */
    bool operator==(const CBigInt &rhs) const;
    bool operator==(int rhs) const;
    bool operator==(const std::string &rhs) const;
    friend bool operator==(int lhs, const CBigInt &rhs);
    friend bool operator==(const std::string &lhs, const CBigInt &rhs);

    bool operator!=(const CBigInt &rhs) const;
    bool operator!=(int rhs) const;
    bool operator!=(const std::string &rhs) const;
    friend bool operator!=(int lhs, const CBigInt &rhs);
    friend bool operator!=(const std::string &lhs, const CBigInt &rhs);

    bool operator<(const CBigInt &rhs) const;
    bool operator<(int rhs) const;
    bool operator<(const std::string &rhs) const;
    friend bool operator<(int lhs, const CBigInt &rhs);
    friend bool operator<(const std::string &lhs, const CBigInt &rhs);

    bool operator<=(const CBigInt &rhs) const;
    bool operator<=(int rhs) const;
    bool operator<=(const std::string &rhs) const;
    friend bool operator<=(int lhs, const CBigInt &rhs);
    friend bool operator<=(const std::string &lhs, const CBigInt &rhs);

    bool operator>(const CBigInt &rhs) const;
    bool operator>(int rhs) const;
    bool operator>(const std::string &rhs) const;
    friend bool operator>(int lhs, const CBigInt &rhs);
    friend bool operator>(const std::string &lhs, const CBigInt &rhs);

    bool operator>=(const CBigInt &rhs) const;
    bool operator>=(int rhs) const;
    bool operator>=(const std::string &rhs) const;
    friend bool operator>=(int lhs, const CBigInt &rhs);
    friend bool operator>=(const std::string &lhs, const CBigInt &rhs);

    /**
     * Stream Operators
     * These operators handle input and output streams for CBigInt objects.
     * - operator<< outputs a CBigInt object to a stream.
     * - operator>> inputs a CBigInt object from a stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const CBigInt &obj);
    friend std::istream &operator>>(std::istream &is, CBigInt &obj);

private:
    std::string num; // The number in string representation
    bool isNegative; // Sign of the number
};

// Constructors and assignment operators implementation
CBigInt::CBigInt(int value) {
    if (value < 0) {
        isNegative = true;
        value = -value; // Make the value positive for storage
    } else {
        isNegative = false;
    }
    num = std::to_string(value);
}

CBigInt::CBigInt(const std::string &value) {
    if (value.empty()) {
        throw std::invalid_argument("Invalid number: string is empty");
    }

    size_t startIndex = 0;
    isNegative = false;

    if (value[startIndex] == '-') {
        isNegative = true;
        startIndex++;
    } else if (value[startIndex] == '+') {
        throw std::invalid_argument("Invalid number: '+' sign not allowed");
    }

    for (size_t i = startIndex; i < value.length(); ++i) {
        if (!isdigit(value[i])) {
            throw std::invalid_argument("Invalid number: contains non-digit characters");
        }
    }

    std::string temp = value.substr(startIndex);
    temp.erase(0, std::min(temp.find_first_not_of('0'), temp.size() - 1));

    if (temp.empty() || temp == "0") {
        num = "0";
        isNegative = false;
    } else {
        num = temp;
    }
}

std::ostream &operator<<(std::ostream &os, const CBigInt &obj) {
    if (obj.isNegative) os << '-';
    os << obj.num;
    return os;
}
std::istream &operator>>(std::istream &is, CBigInt &obj) {
    std::string input;
    char ch;

    // Skip leading white spaces
    while (std::isspace(is.peek())) {
        is.get(ch); // Actually consume the space character
    }

    ch = is.peek(); // Look at the next character without extracting it

    // If the stream is at the end or the character is not valid, set failbit and return
    if (is.eof() || (!std::isdigit(ch) && ch != '-')) {
        is.setstate(std::ios::failbit);
        return is;
    }

    if (ch == '-') {
        is.get(ch); // Consume the '-' since it's part of the number
        if (!std::isdigit(is.peek())) { // Ensure the next character is a digit
            is.setstate(std::ios::failbit);
            return is;
        }
        input += '-';
    }

    // Loop to read the digits
    while (std::isdigit(is.peek())) {
        is.get(ch); // Consume the digit
        input += ch;
    }

    // If no number was started (input is just "-"), set failbit
    if (input.empty() || input == "-") {
        is.setstate(std::ios::failbit);
        return is;
    }

    // Update the CBigInt object
    obj = CBigInt(input);

    return is;
}

//---------------------------- = operation
CBigInt &CBigInt::operator=(const CBigInt &rhs) = default;

CBigInt &CBigInt::operator=(int rhs) {
    *this = CBigInt(rhs);
    return *this;
}

CBigInt &CBigInt::operator=(const std::string &rhs) {
    *this = CBigInt(rhs);
    return *this;
}
//---------------------------- = operation


//---------------------------- + operation
/**
 * Helper function to add absolute values of two strings representing large integers.
 * @param num1 The first number in string format.
 * @param num2 The second number in string format.
 * @return The sum of num1 and num2 as a string.
 */
std::string addAbs(const std::string &num1, const std::string &num2) {
    std::string result;
    int carry = 0;
    int n1 = num1.size(), n2 = num2.size();
    int diff = abs(n1 - n2);

    // Pad the shorter string with zeros
    std::string str1 = n1 > n2 ? num1 : std::string(diff, '0') + num1;
    std::string str2 = n1 > n2 ? std::string(diff, '0') + num2 : num2;

    // Add the numbers from right to left
    for (int i = str1.size() - 1; i >= 0; --i) {
        int sum = (str1[i] - '0') + (str2[i] - '0') + carry;
        result.push_back(sum % 10 + '0');
        carry = sum / 10;
    }

    if (carry) {
        result.push_back(carry + '0');
    }

    // Reverse result as it's constructed backwards
    std::reverse(result.begin(), result.end());

    return result;
}
/**
 * Helper function to subtract the absolute values of two strings representing large integers.
 * @param num1 The first number in string format.
 * @param num2 The second number in string format.
 * @return The difference between num1 and num2 as a string.
 */
std::string subtractAbs(const std::string &num1, const std::string &num2) {
    std::string result;
    int carry = 0;
    int n1 = num1.length(), n2 = num2.length();
    int diff = n1 - n2;

    for (int i = n1 - 1; i >= 0; i--) {
        int sub = (num1[i] - '0') - (i - diff >= 0 ? (num2[i - diff] - '0') : 0) - carry;
        if (sub < 0) {
            sub += 10;
            carry = 1;
        } else {
            carry = 0;
        }
        result.push_back(sub + '0');
    }

    // Reverse result as it's constructed backwards
    std::reverse(result.begin(), result.end());

    return result;
}
/**
 * Helper function to check if the absolute value of one string number is greater than another.
 * @param num1 The first number in string format.
 * @param num2 The second number in string format.
 * @return True if num1 is greater than num2, otherwise false.
 */
bool absGreater(const std::string &num1, const std::string &num2) {
    if (num1.size() != num2.size()) {
        return num1.size() > num2.size();
    }
    return num1 > num2;
}
/**
 * Normalize function to remove leading zeros from the CBigInt object.
 */
void CBigInt::normalize() {
    // Remove leading zeros
    size_t nonZero = num.find_first_not_of('0');
    if (nonZero != std::string::npos) {
        num = num.substr(nonZero);
    } else {
        num = "0";
        isNegative = false; // Zero is not negative
    }
}

CBigInt CBigInt::operator+(const CBigInt &rhs) const {
    CBigInt result;

    if (isNegative == rhs.isNegative) {
        // Both numbers have the same sign. Add their absolute values.
        result.num = addAbs(num, rhs.num); // Assuming addAbs exists and adds the absolute values
        result.isNegative = isNegative;
    } else {
        // The numbers have different signs. Subtract the smaller from the larger.
        if (absGreater(num, rhs.num)) { // Assuming absGreater checks which absolute value is greater
            result.num = subtractAbs(num,
                                     rhs.num); // Assuming subtractAbs exists and subtracts the second abs value from the first
            result.isNegative = isNegative;
        } else {
            result.num = subtractAbs(rhs.num, num);
            result.isNegative = rhs.isNegative;
        }
    }

    // Normalize result to handle potential leading zeros after addition
    result.normalize();
    return result;
}
CBigInt CBigInt::operator+(int rhs) const {
    return *this + CBigInt(rhs);
}

CBigInt CBigInt::operator+(const std::string &rhs) const {
    return *this + CBigInt(rhs);
}

CBigInt operator+(int lhs, const CBigInt &rhs) {
    return CBigInt(lhs) + rhs;
}

CBigInt operator+(const std::string &lhs, const CBigInt &rhs) {
    return CBigInt(lhs) + rhs;
}
//---------------------------- + operation


//---------------------------- += operation
CBigInt &CBigInt::operator+=(const CBigInt &rhs) {
    *this = *this + rhs;
    return *this;
}

CBigInt CBigInt::operator+=(int rhs) {
    *this = *this + CBigInt(rhs);
    return *this;
}

CBigInt CBigInt::operator+=(const std::string &rhs) {
    *this = *this + CBigInt(rhs);
    return *this;
}
//---------------------------- += operation


//---------------------------- * operation
/**
 * Helper function to multiply the absolute values of two strings representing large integers.
 * @param num1 The first number in string format.
 * @param num2 The second number in string format.
 * @return The product of num1 and num2 as a string.
 */
std::string multiplyAbs(const std::string &num1, const std::string &num2) {
    int n1 = num1.size(), n2 = num2.size();
    std::vector<int> result(n1 + n2, 0);

    for (int i = n1 - 1; i >= 0; --i) {
        for (int j = n2 - 1; j >= 0; --j) {
            int mul = (num1[i] - '0') * (num2[j] - '0');
            int sum = mul + result[i + j + 1];

            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }

    std::string mulResult;
    for (int num: result) {
        if (!(mulResult.empty() && num == 0)) { // Skip leading zeros
            mulResult.push_back(num + '0');
        }
    }

    return mulResult.empty() ? "0" : mulResult; // Return "0" if result is empty after removing leading zeros
}

CBigInt CBigInt::operator*(const CBigInt &rhs) const {
    CBigInt result;
    // Assuming multiplyAbs exists and multiplies the absolute values
    result.num = multiplyAbs(num,
                             rhs.num); // Implement this based on schoolbook multiplication or a more efficient algorithm
    result.isNegative = isNegative != rhs.isNegative; // The result is negative iff one operand is negative

    // Normalize result to handle potential leading zeros after multiplication
    result.normalize();
    return result;
}
CBigInt CBigInt::operator*(int rhs) const {
    return *this * CBigInt(rhs);
}

CBigInt CBigInt::operator*(const std::string &rhs) const {
    return *this * CBigInt(rhs);
}

CBigInt operator*(int lhs, const CBigInt &rhs) {
    return CBigInt(lhs) * rhs;
}

CBigInt operator*(const std::string &lhs, const CBigInt &rhs) {
    return CBigInt(lhs) * rhs;
}

//---------------------------- * operation


//---------------------------- *= operation

CBigInt &CBigInt::operator*=(const CBigInt &rhs) {
    *this = *this * rhs;
    return *this;
}

CBigInt CBigInt::operator*=(int rhs) {
    *this = *this * CBigInt(rhs);
    return *this;
}

CBigInt CBigInt::operator*=(const std::string &rhs) {
    *this = *this * CBigInt(rhs);
    return *this;
}
//---------------------------- *=operation


//---------------------------- ==operation
bool CBigInt::operator==(const CBigInt &rhs) const {
    return this->isNegative == rhs.isNegative && this->num == rhs.num;
}
bool CBigInt::operator==(int rhs) const {
    return *this == CBigInt(rhs);
}

bool CBigInt::operator==(const std::string &rhs) const {
    return *this == CBigInt(rhs);
}

bool operator==(int lhs, const CBigInt &rhs) {
    return CBigInt(lhs) == rhs;
}

bool operator==(const std::string &lhs, const CBigInt &rhs) {
    return CBigInt(lhs) == rhs;
}
//---------------------------- ==operation


//---------------------------- !=operation
bool CBigInt::operator!=(const CBigInt &rhs) const { return !(*this == rhs); }
bool CBigInt::operator!=(int rhs) const { return !(*this == rhs); }
bool CBigInt::operator!=(const std::string &rhs) const { return !(*this == rhs); }
bool operator!=(int lhs, const CBigInt &rhs) { return !(lhs == rhs); }
bool operator!=(const std::string &lhs, const CBigInt &rhs) { return !(lhs == rhs); }
//---------------------------- !=operation


//---------------------------- <operation
bool CBigInt::operator<(const CBigInt &rhs) const {
    if (this->isNegative != rhs.isNegative) return this->isNegative;
    if (num.size() != rhs.num.size())
        return this->isNegative ? this->num.size() > rhs.num.size() : this->num.size() < rhs.num.size();
    return this->isNegative ? this->num > rhs.num : this->num < rhs.num;
}
bool CBigInt::operator<(int rhs) const {
    return *this < CBigInt(rhs);
}
bool CBigInt::operator<(const std::string &rhs) const {
    return *this < CBigInt(rhs);
}
bool operator<(int lhs, const CBigInt &rhs) {
    return CBigInt(lhs) < rhs;
}
bool operator<(const std::string &lhs, const CBigInt &rhs) {
    return CBigInt(lhs) < rhs;
}
//---------------------------- <operation




//---------------------------- <=operation
bool CBigInt::operator<=(const CBigInt &rhs) const {
    return *this < rhs || *this == rhs;
}
bool CBigInt::operator<=(int rhs) const {
    return *this <= CBigInt(rhs);
}
bool CBigInt::operator<=(const std::string &rhs) const {
    return *this <= CBigInt(rhs);
}
bool operator<=(int lhs, const CBigInt &rhs) {
    return CBigInt(lhs) <= rhs;
}
bool operator<=(const std::string &lhs, const CBigInt &rhs) {
    return CBigInt(lhs) <= rhs;
}
//---------------------------- <=operation


//---------------------------- >operation
bool CBigInt::operator>(const CBigInt &rhs) const {
    return !(*this <= rhs);
}
bool CBigInt::operator>(int rhs) const {
    return *this > CBigInt(rhs);
}
bool CBigInt::operator>(const std::string &rhs) const {
    return *this > CBigInt(rhs);
}
bool operator>(int lhs, const CBigInt &rhs) {
    return CBigInt(lhs) > rhs;
}
bool operator>(const std::string &lhs, const CBigInt &rhs) {
    return CBigInt(lhs) > rhs;
}
//---------------------------- >operation



//---------------------------- >=operation
bool CBigInt::operator>=(const CBigInt &rhs) const {
    return !(*this < rhs);
}
bool CBigInt::operator>=(int rhs) const {
    return *this >= CBigInt(rhs);
}
bool CBigInt::operator>=(const std::string &rhs) const {
    return *this >= CBigInt(rhs);
}
bool operator>=(int lhs, const CBigInt &rhs) {
    return CBigInt(lhs) >= rhs;
}
bool operator>=(const std::string &lhs, const CBigInt &rhs) {
    return CBigInt(lhs) >= rhs;
}
//---------------------------- >=operation



#ifndef __PROGTEST__

static bool equal(const CBigInt &x, const char val[]) {
    std::ostringstream oss;
    oss << x;
    return oss.str() == val;
}

static bool equalHex(const CBigInt &x, const char val[]) {
    return true; // hex output is needed for bonus tests only
    // std::ostringstream oss;
    // oss << std::hex << x;
    // return oss . str () == val;
}

int main() {
    CBigInt c;
    c = "-00100";
    assert(c == "-000000000000000000000100");

    CBigInt a, b;
    std::istringstream is;
    a = 10;
    a += 20;
    assert (equal(a, "30"));
    a *= 5;
    assert (equal(a, "150"));
    b = a + 3;
    assert (equal(b, "153"));
    b = a * 7;
    assert (equal(b, "1050"));
    assert (equal(a, "150"));
    assert (equalHex(a, "96"));

    a = 10;
    a += -20;
    assert (equal(a, "-10"));
    a *= 5;
    assert (equal(a, "-50"));
    b = a + 73;
    assert (equal(b, "23"));
    b = a * -7;
    assert (equal(b, "350"));
    assert (equal(a, "-50"));
    assert (equalHex(a, "-32"));

    a = "12345678901234567890";
    a += "-99999999999999999999";
    assert (equal(a, "-87654321098765432109"));
    a *= "54321987654321987654";
    assert (equal(a, "-4761556948575111126880627366067073182286"));
    a *= 0;
    assert (equal(a, "0"));
    a = 10;
    b = a + "400";
    assert (equal(b, "410"));
    b = a * "15";
    assert (equal(b, "150"));
    assert (equal(a, "10"));
    assert (equalHex(a, "a"));

    is.clear();
    is.str(" 1234");
//    std::string bc = is.str();
    assert (is >> b);
    assert (equal(b, "1234"));
    is.clear();
    is.str(" 12 34");
    assert (is >> b);
    assert (equal(b, "12"));
    is.clear();
    is.str("999z");
    assert (is >> b);
    assert (equal(b, "999"));
    is.clear();
    is.str("abcd");
    assert (!(is >> b));
    is.clear();
    is.str("- 758");
    assert (!(is >> b));
    a = 42;
    try {
        a = "-xyz";
        assert ("missing an exception" == nullptr);
    }
    catch (const std::invalid_argument &e) {
        assert (equal(a, "42"));
    }

    a = "73786976294838206464";
    assert (equal(a, "73786976294838206464"));
    assert (equalHex(a, "40000000000000000"));
    assert (a < "1361129467683753853853498429727072845824");
    assert (a <= "1361129467683753853853498429727072845824");
    assert (!(a > "1361129467683753853853498429727072845824"));
    assert (!(a >= "1361129467683753853853498429727072845824"));
    assert (!(a == "1361129467683753853853498429727072845824"));
    assert (a != "1361129467683753853853498429727072845824");
    assert (!(a < "73786976294838206464"));
    assert (a <= "73786976294838206464");
    assert (!(a > "73786976294838206464"));
    assert (a >= "73786976294838206464");
    assert (a == "73786976294838206464");
    assert (!(a != "73786976294838206464"));
    assert (a < "73786976294838206465");
    assert (a <= "73786976294838206465");
    assert (!(a > "73786976294838206465"));
    assert (!(a >= "73786976294838206465"));
    assert (!(a == "73786976294838206465"));
    assert (a != "73786976294838206465");
    a = "2147483648";
    assert (!(a < -2147483648));
    assert (!(a <= -2147483648));
    assert (a > -2147483648);
    assert (a >= -2147483648);
    assert (!(a == -2147483648));
    assert (a != -2147483648);
    a = "-12345678";
    assert (!(a < -87654321));
    assert (!(a <= -87654321));
    assert (a > -87654321);
    assert (a >= -87654321);
    assert (!(a == -87654321));
    assert (a != -87654321);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */