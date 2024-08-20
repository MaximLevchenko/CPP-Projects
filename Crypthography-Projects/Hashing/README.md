# **Random Message Generation and Hash Validation Using OpenSSL** 

## **Project Overview** 
This project presents a C++ implementation focused on generating random messages and validating their hash values using the **`OpenSSL`** library. The primary objective is to create random messages and compute their hashes while ensuring these hashes meet specific requirements, such as having a defined number of leading zero bits.

## **Core Functions and Their Roles** 

### 1. **`hasRightAmountOfNulls`**:
- Verifies whether a given hash contains the required number of leading zero bits.
- **Parameters**:
    - `bits`: The desired number of leading zero bits.
    - `dexHashArray`: The array representing the hash that needs to be checked.
- **Returns**: `true` if the hash meets the criteria, otherwise `false`.

### 2. **`transfromToHex`**:
- Converts binary data (like a message or hash) into its hexadecimal string representation.
- **Parameters**:
    - `str_to_transform`: The binary data that needs to be converted.
    - `hash_tmp`: A buffer to temporarily store hash data during conversion.
- **Returns**: A `char*` that points to the hexadecimal string. The caller must free this memory after use.

### 3. **`getRandomlyGeneratedString`**:
- Generates a random string of 64 bytes, leveraging OpenSSL's `RAND_bytes` function.
- **Parameters**:
    - `text_to_cipher`: A buffer that will hold the generated random string.

### 4. **`findHashEx`**:
- Finds a random message and computes its hash, ensuring the hash begins with the specified number of leading zero bits. This function allows the user to specify which hash function to use.
- **Parameters**:
    - `bits`: The required number of leading zero bits in the hash.
    - `message`: A pointer to store the generated message in hexadecimal format.
    - `hash`: A pointer to store the computed hash in hexadecimal format.
    - `hashFunction`: A string specifying the hash function to use (e.g., "sha512", "sha256").
- **Returns**: `1` if a suitable hash is found, otherwise `0`.

### 5. **`findHash`**:
- A specialized version of `findHashEx` that defaults to using the "sha512" hash function.
- **Parameters**:
    - `bits`: The required number of leading zero bits in the hash.
    - `message`: A pointer to store the generated message in hexadecimal format.
    - `hash`: A pointer to store the computed hash in hexadecimal format.
- **Returns**: `1` if successful, `0` otherwise.

## **Example Usage** 
Here's an example of how to use the `findHash` function in a typical scenario:

```cpp
int main(void) {
    char *message, *hash;

    assert(findHash(9, &message, &hash) == 1);
    // Ensure message and hash are properly generated and verified.
    free(message);
    free(hash);

    assert(findHash(0, &message, &hash) == 1);
    free(message);
    free(hash);

    assert(findHash(-1, &message, &hash) == 0);  // Invalid bits should fail

    return EXIT_SUCCESS;
}
```
    ## **Technologies and Techniques Employed** 🛠

### **OpenSSL**:
- Used for generating cryptographic hashes.
- Key functions include `EVP_MD_CTX_new`, `EVP_DigestInit_ex`, `EVP_DigestUpdate`, and `EVP_DigestFinal_ex` to handle the creation and finalization of hashes.

### **C++ Standard Library**:
- Utilized for dynamic memory management (`malloc` and `free`), as well as for handling strings and basic input/output operations.

### **Error Handling**:
- Implemented checks for memory allocation failures and validation of the success of cryptographic operations.
- Ensured that resources are freed correctly to prevent memory leaks.

## **Concepts Demonstrated** 

### **Cryptographic Hashing**:
- Implementation of secure message hashing using SHA-512 or other specified hash functions to ensure data integrity and validation.

### **Random Message Generation**:
- The project demonstrates how to generate random strings and modify them to meet specific cryptographic criteria.

### **Memory Management**:
- Careful allocation and deallocation of memory resources to ensure efficient and error-free operation.

### **Hexadecimal Conversion**:
- Converting binary data to a human-readable hexadecimal format, which is essential for verifying and displaying cryptographic data.

### **Compile the C++ Code:**

Use the following command to compile your code. Replace `your_code.cpp` with the name of your C++ source file.

```bash
g++ -o find_hash your_code.cpp -lcrypto
```

- `-lcrypto`: Links the OpenSSL crypto library, which is required for the cryptographic operations.

### **Run the Executable:**

After successful compilation, you can run the executable using:

```bash
./find_hash
```

