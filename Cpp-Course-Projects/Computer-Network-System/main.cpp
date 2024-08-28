#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
#endif /* __PROGTEST__ */

// Abstract base class for all components (e.g., CPU, Memory, Disk)
class CComponent {
public:
    virtual ~CComponent() {} // Virtual destructor for safe polymorphic deletion

    // Pure virtual function to clone the component, enforcing derived classes to implement this method
    virtual std::unique_ptr<CComponent> clone() const = 0;

    // Pure virtual function to print component details, requires implementation in derived classes
    virtual void print(std::ostream &os, bool lastComponent = false, bool lastComputer = false,
                       bool isInsideNetwork = false) const = 0;
};

// Derived class representing a CPU component
class CCPU : public CComponent {
public:
    CCPU(int c, int f) : cores(c), frequency(f) {} // Constructor initializing cores and frequency
    CCPU(const CCPU &other) : cores(other.cores), frequency(other.frequency) {} // Copy constructor

    // Copy assignment operator for deep copying of CCPU objects
    CCPU &operator=(const CCPU &other) {
        if (this != &other) {
            cores = other.cores;
            frequency = other.frequency;
        }
        return *this;
    }

    // Override clone method to return a new instance of CCPU
    std::unique_ptr<CComponent> clone() const override {
        return std::make_unique<CCPU>(*this);
    }

    // Override print method to display CPU details in a specific format
    void print(std::ostream &os, bool lastComponent = false, bool lastComputer = false,
               bool isInsideNetwork = false) const override {
        os << (lastComponent ? "\\-" : "+-") << "CPU, " << cores << " cores @ " << frequency << "MHz" << "\n";
    }

private:
    int cores;      // Number of CPU cores
    int frequency;  // CPU frequency in MHz
};

// Derived class representing a Memory component
class CMemory : public CComponent {
public:
    CMemory(int s) : size(s) {} // Constructor initializing memory size
    CMemory(const CMemory &other) : size(other.size) {} // Copy constructor

    // Copy assignment operator for deep copying of CMemory objects
    CMemory &operator=(const CMemory &other) {
        if (this != &other) {
            size = other.size;
        }
        return *this;
    }

    // Getter method for memory size
    int getSize() const {
        return size;
    }

    // Override clone method to return a new instance of CMemory
    std::unique_ptr<CComponent> clone() const override {
        return std::make_unique<CMemory>(*this);
    }

    // Override print method to display memory details in a specific format
    void print(std::ostream &os, bool lastComponent = false, bool lastComputer = false,
               bool isInsideNetwork = false) const override {
        os << (!lastComponent ? "+-" : "\\-") << "Memory, " << size << " MiB" << "\n";
    }

private:
    int size; // Memory size in MiB
};

// Derived class representing a Disk component
class CDisk : public CComponent {
public:
    enum DiskType { SSD, MAGNETIC }; // Enum for disk types

    CDisk(DiskType t, int s) : type(t), size(s) {} // Constructor initializing disk type and size
    CDisk(const CDisk &other) : type(other.type), size(other.size), partitions(other.partitions) {} // Copy constructor

    // Copy assignment operator for deep copying of CDisk objects
    CDisk &operator=(const CDisk &other) {
        if (this != &other) {
            type = other.type;
            size = other.size;
            partitions = other.partitions;
        }
        return *this;
    }

    // Override clone method to return a new instance of CDisk
    std::unique_ptr<CComponent> clone() const override {
        return std::make_unique<CDisk>(*this);
    }

    // Method to add a partition to the disk
    CDisk &addPartition(int size, const std::string &label) {
        partitions.emplace_back(size, label);
        return *this;
    }

    // Override print method to display disk details and partitions in a specific format
    void print(std::ostream &os, bool lastComponent = false, bool lastComputer = false,
               bool isInsideNetwork = false) const override {
        os << (lastComponent ? "\\-" : "+-") << (type == SSD ? "SSD" : "HDD") << ", " << size << " GiB\n";

        // Calculate indentation based on whether inside a network and position of the component
        std::string baseIndent = (isInsideNetwork && !lastComputer) ? "| " : "  ";
        std::string partitionIndent = (isInsideNetwork && !lastComponent) ? "| " : "  ";

        // Print each partition with the appropriate indentation
        for (size_t i = 0; i < partitions.size(); ++i) {
            if (isInsideNetwork && !(lastComponent && lastComputer && i == partitions.size() - 1)) {
                os << baseIndent << partitionIndent;
            } else if (isInsideNetwork) {
                os << "    ";
            } else {
                os << (lastComponent ? "  " : "| ");
            }
            os << (i < partitions.size() - 1 ? "+-" : "\\-") << "[" << i << "]: "
               << partitions[i].first << " GiB, " << partitions[i].second << "\n";
        }
    }

private:
    DiskType type; // Disk type (SSD or MAGNETIC)
    int size; // Disk size in GiB
    std::vector<std::pair<int, std::string>> partitions; // List of partitions (size and label)
};

// Class representing a Computer, which can have multiple components and addresses
class CComputer {
public:
    // Constructor initializing computer name
    CComputer(std::string n) : name(std::move(n)), lastComputer(true) {}

    // Copy constructor for deep copying of CComputer objects
    CComputer(const CComputer &other)
            : name(other.name), addresses(other.addresses), lastComputer(other.lastComputer),
              isInsideNetwork(other.isInsideNetwork) {
        for (const auto &comp: other.components) {
            components.push_back(comp->clone());
        }
    }

    // Copy assignment operator for deep copying of CComputer objects
    CComputer &operator=(const CComputer &other) {
        if (this != &other) {
            name = other.name;
            addresses = other.addresses;
            components.clear();
            for (const auto &comp: other.components) {
                components.push_back(comp->clone());
            }
            lastComputer = other.lastComputer;
            isInsideNetwork = other.isInsideNetwork;
        }
        return *this;
    }

    // Method to add a network address to the computer
    CComputer &addAddress(const std::string &addr) {
        addresses.push_back(addr);
        return *this;
    }

    // Method to add a component to the computer
    CComputer &addComponent(const CComponent &comp) {
        components.push_back(comp.clone());
        return *this;
    }

    // Getters and setters for various attributes
    const std::vector<std::string> &getAddresses() const { return addresses; }
    const std::vector<std::shared_ptr<CComponent>> &getComponents() const { return components; }
    const std::string &getName() const { return name; }
    bool isLastComputer() const { return lastComputer; }
    void setLastComputer(bool lastComputer) { this->lastComputer = lastComputer; }
    const bool getIsInsideNetwork() const { return isInsideNetwork; }
    void setIsInsideNetwork(bool isInsideNetwork) { this->isInsideNetwork = isInsideNetwork; }

    // Friend function to overload the output operator for displaying computer details
    friend std::ostream &operator<<(std::ostream &os, const CComputer &comp);

private:
    std::string name; // Computer name
    std::vector<std::string> addresses; // List of network addresses
    std::vector<std::shared_ptr<CComponent>> components; // List of components
    bool lastComputer = false; // Flag to indicate if this is the last computer in a network
    mutable bool isInsideNetwork = true; // Flag to indicate if the computer is inside a network
};

// Function to print a computer's details in the specified format
void printComputer(std::ostream &os, const CComputer &comp, bool lastComputer, bool isPartOfNetwork = true) {
    os << "Host: " << comp.getName() << "\n";
    for (size_t i = 0; i < comp.getAddresses().size(); ++i) {
        if (!isPartOfNetwork) {
            os << (comp.getComponents().empty() && i == comp.getAddresses().size() - 1 ? "\\-" : "+-")
               << comp.getAddresses()[i]
               << "\n";
        } else
            os << (lastComputer ? "  " : "| ")
               << (comp.getComponents().empty() && i == comp.getAddresses().size() - 1 ? "\\-" : "+-")
               << comp.getAddresses()[i]
               << "\n";
    }
    for (size_t i = 0; i < comp.getComponents().size(); ++i) {
        if (!isPartOfNetwork) {
            comp.getComponents()[i]->print(os, i == comp.getComponents().size() - 1);
        } else {
            os << (lastComputer ? "  " : "| "); // Initial indentation for components
            comp.getComponents()[i]->print(os, i == comp.getComponents().size() - 1, comp.isLastComputer(),
                                           comp.getIsInsideNetwork());
        }
    }
}

// Overloaded output operator to print a computer's details
std::ostream &operator<<(std::ostream &os, const CComputer &comp) {
    comp.isInsideNetwork = false; // Temporarily set to false to handle output formatting
    printComputer(os, comp, true, comp.getIsInsideNetwork());
    comp.isInsideNetwork = true; // Reset after printing
    return os;
}

// Class representing a Network, which can contain multiple computers
class CNetwork {
public:
    // Constructor initializing network name
    CNetwork(const std::string &n) : name(n) {}

    // Copy constructor for deep copying of CNetwork objects
    CNetwork(const CNetwork &other) : name(other.name), computers(other.computers) {}

    // Copy assignment operator for deep copying of CNetwork objects
    CNetwork &operator=(const CNetwork &other) {
        if (this != &other) {
            name = other.name;
            computers = other.computers;  // Assumes that CComputer has a proper copy constructor.
        }
        return *this;
    }

    // Method to add a computer to the network
    CNetwork &addComputer(const CComputer &comp) {
        computers.push_back(comp);
        if (computers.size() > 1)
            computers[computers.size() - 2].setLastComputer(false); // Update the lastComputer flag for the second to last computer
        return *this;
    }

    // Method to find a computer by name within the network
    CComputer *findComputer(const std::string &name) {
        for (auto &comp: computers) {
            if (comp.getName() == name) {
                comp.setIsInsideNetwork(false);
                return &comp;
            }
        }
        return nullptr;
    }

    // Friend function to overload the output operator for displaying network details
    friend std::ostream &operator<<(std::ostream &os, const CNetwork &net);

private:
    std::string name; // Network name
    std::vector<CComputer> computers; // List of computers in the network
};

// Overloaded output operator to print network details in the specified format
std::ostream &operator<<(std::ostream &os, const CNetwork &net) {
    os << "Network: " << net.name << "\n";
    for (size_t i = 0; i < net.computers.size(); ++i) {
        os << (i < net.computers.size() - 1 ? "+-" : "\\-");
        printComputer(os, net.computers[i], i == net.computers.size() - 1);
    }
    return os;
}

#ifndef __PROGTEST__
template<typename T_>
std::string toString(const T_ &x) {
    std::ostringstream oss;
    oss << x;
    return oss.str();
}

int main() {
    CNetwork n ( "FIT network" );
    n . addComputer (
            CComputer ( "progtest.fit.cvut.cz" ) .
                    addAddress ( "147.32.232.142" ) .
                    addComponent ( CCPU ( 8, 2400 ) ) .
                    addComponent ( CCPU ( 8, 1200 ) ) .
                    addComponent ( CDisk ( CDisk::MAGNETIC, 1500 ) .
                    addPartition ( 50, "/" ) .
                    addPartition ( 5, "/boot" ).
                    addPartition ( 1000, "/var" ) ) .
                    addComponent ( CDisk ( CDisk::SSD, 60 ) .
                    addPartition ( 60, "/data" )  ) .
                    addComponent ( CMemory ( 2000 ) ).
                    addComponent ( CMemory ( 2000 ) ) ) .
            addComputer (
            CComputer ( "courses.fit.cvut.cz" ) .
                    addAddress ( "147.32.232.213" ) .
                    addComponent ( CCPU ( 4, 1600 ) ) .
                    addComponent ( CMemory ( 4000 ) ).
                    addComponent ( CDisk ( CDisk::MAGNETIC, 2000 ) .
                    addPartition ( 100, "/" )   .
                    addPartition ( 1900, "/data" ) ) ) .
            addComputer (
            CComputer ( "imap.fit.cvut.cz" ) .
                    addAddress ( "147.32.232.238" ) .
                    addComponent ( CCPU ( 4, 2500 ) ) .
                    addAddress ( "2001:718:2:2901::238" ) .
                    addComponent ( CMemory ( 8000 ) ) );
    assert ( toString ( n ) ==
             "Network: FIT network\n"
             "+-Host: progtest.fit.cvut.cz\n"
             "| +-147.32.232.142\n"
             "| +-CPU, 8 cores @ 2400MHz\n"
             "| +-CPU, 8 cores @ 1200MHz\n"
             "| +-HDD, 1500 GiB\n"
             "| | +-[0]: 50 GiB, /\n"
             "| | +-[1]: 5 GiB, /boot\n"
             "| | \\-[2]: 1000 GiB, /var\n"
             "| +-SSD, 60 GiB\n"
             "| | \\-[0]: 60 GiB, /data\n"
             "| +-Memory, 2000 MiB\n"
             "| \\-Memory, 2000 MiB\n"
             "+-Host: courses.fit.cvut.cz\n"
             "| +-147.32.232.213\n"
             "| +-CPU, 4 cores @ 1600MHz\n"
             "| +-Memory, 4000 MiB\n"
             "| \\-HDD, 2000 GiB\n"
             "|   +-[0]: 100 GiB, /\n"
             "|   \\-[1]: 1900 GiB, /data\n"
             "\\-Host: imap.fit.cvut.cz\n"
             "  +-147.32.232.238\n"
             "  +-2001:718:2:2901::238\n"
             "  +-CPU, 4 cores @ 2500MHz\n"
             "  \\-Memory, 8000 MiB\n" );
    CNetwork x = n;
    auto c = x . findComputer ( "imap.fit.cvut.cz" );
    assert ( toString ( *c ) ==
             "Host: imap.fit.cvut.cz\n"
             "+-147.32.232.238\n"
             "+-2001:718:2:2901::238\n"
             "+-CPU, 4 cores @ 2500MHz\n"
             "\\-Memory, 8000 MiB\n" );
    c -> addComponent ( CDisk ( CDisk::MAGNETIC, 1000 ) .
            addPartition ( 100, "system" ) .
            addPartition ( 200, "WWW" ) .
            addPartition ( 700, "mail" ) );
    assert ( toString ( x ) ==
             "Network: FIT network\n"
             "+-Host: progtest.fit.cvut.cz\n"
             "| +-147.32.232.142\n"
             "| +-CPU, 8 cores @ 2400MHz\n"
             "| +-CPU, 8 cores @ 1200MHz\n"
             "| +-HDD, 1500 GiB\n"
             "| | +-[0]: 50 GiB, /\n"
             "| | +-[1]: 5 GiB, /boot\n"
             "| | \\-[2]: 1000 GiB, /var\n"
             "| +-SSD, 60 GiB\n"
             "| | \\-[0]: 60 GiB, /data\n"
             "| +-Memory, 2000 MiB\n"
             "| \\-Memory, 2000 MiB\n"
             "+-Host: courses.fit.cvut.cz\n"
             "| +-147.32.232.213\n"
             "| +-CPU, 4 cores @ 1600MHz\n"
             "| +-Memory, 4000 MiB\n"
             "| \\-HDD, 2000 GiB\n"
             "|   +-[0]: 100 GiB, /\n"
             "|   \\-[1]: 1900 GiB, /data\n"
             "\\-Host: imap.fit.cvut.cz\n"
             "  +-147.32.232.238\n"
             "  +-2001:718:2:2901::238\n"
             "  +-CPU, 4 cores @ 2500MHz\n"
             "  +-Memory, 8000 MiB\n"
             "  \\-HDD, 1000 GiB\n"
             "    +-[0]: 100 GiB, system\n"
             "    +-[1]: 200 GiB, WWW\n"
             "    \\-[2]: 700 GiB, mail\n" );
    assert ( toString ( n ) ==
             "Network: FIT network\n"
             "+-Host: progtest.fit.cvut.cz\n"
             "| +-147.32.232.142\n"
             "| +-CPU, 8 cores @ 2400MHz\n"
             "| +-CPU, 8 cores @ 1200MHz\n"
             "| +-HDD, 1500 GiB\n"
             "| | +-[0]: 50 GiB, /\n"
             "| | +-[1]: 5 GiB, /boot\n"
             "| | \\-[2]: 1000 GiB, /var\n"
             "| +-SSD, 60 GiB\n"
             "| | \\-[0]: 60 GiB, /data\n"
             "| +-Memory, 2000 MiB\n"
             "| \\-Memory, 2000 MiB\n"
             "+-Host: courses.fit.cvut.cz\n"
             "| +-147.32.232.213\n"
             "| +-CPU, 4 cores @ 1600MHz\n"
             "| +-Memory, 4000 MiB\n"
             "| \\-HDD, 2000 GiB\n"
             "|   +-[0]: 100 GiB, /\n"
             "|   \\-[1]: 1900 GiB, /data\n"
             "\\-Host: imap.fit.cvut.cz\n"
             "  +-147.32.232.238\n"
             "  +-2001:718:2:2901::238\n"
             "  +-CPU, 4 cores @ 2500MHz\n"
             "  \\-Memory, 8000 MiB\n" );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */