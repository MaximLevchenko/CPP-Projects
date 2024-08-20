# **Cryptography Projects Overview** 

This repository contains a collection of cryptography-related projects implemented in C++. Each project demonstrates different aspects of cryptographic operations, such as hashing and encryption/decryption of files. Below is a brief description of the projects included in this repository.

## **Projects Included** 

### 1. **Encryption-Modes** 

This project focuses on the encryption and decryption of TGA image files using various modes of the AES block cipher. The project demonstrates how to securely encrypt image data while preserving the header and how to correctly decrypt the data back to its original form.

- **Key Features**:
    - Supports multiple AES modes, including ECB and CBC.
    - Preserves the integrity of the TGA file header during encryption and decryption.
    - Automatically generates cryptographic keys and Initialization Vectors (IVs) if not provided.

- **Key Functions**:
    - `encrypt_data`: Encrypts the contents of a TGA file.
    - `decrypt_data`: Decrypts an encrypted TGA file.
    - `compare_files`: Ensures that the decrypted file matches the original file.

- **Location**: `Encryption-Modes` directory.

### 2. **Hashing** #️

This project is centered around generating and verifying cryptographic hashes using the OpenSSL library. The focus is on finding specific messages that produce hash values with a defined number of leading zero bits, which is a common requirement in various cryptographic protocols.

- **Key Features**:
    - Utilizes the SHA-512 hashing algorithm by default.
    - Allows for the selection of different hash functions.
    - Implements a random message generator to find hash values with specific characteristics.

- **Key Functions**:
    - `findHash`: Finds a random message whose hash starts with a given number of leading zero bits.
    - `findHashEx`: An extended version of `findHash` that supports different hash functions.

- **Location**: `Hashing` directory.

## **How to Get Started** 

### **Running the Projects**

1. **Compile the Code**:
    - Navigate to the respective project directory (`Encryption-Modes` or `Hashing`).
    - Use the following command to compile the C++ source files with OpenSSL linked:
      ```bash
      g++ -o project_name main.cpp -lcrypto -lssl
      ```
      Replace `project_name` with a suitable name for the executable.

2. **Execute the Compiled Binary**:
    - Run the compiled binary to test the functionality:
      ```bash
      ./project_name
      ```

### **Dependencies**

- **OpenSSL**: Ensure that the OpenSSL library is installed and properly linked during compilation, as it is required for cryptographic operations in both projects.

## **Repository Structure** 

- **Encryption-Modes/**: Contains the TGA file encryption/decryption project.
- **Hashing/**: Contains the cryptographic hashing project.
- **README.md**: This file, providing an overview of the projects contained within the repository.

## **Conclusion** 

This repository showcases a variety of cryptographic techniques implemented in C++, providing practical examples of how to secure data using industry-standard methods. Each project is self-contained with its own detailed documentation and tests to verify functionality.
