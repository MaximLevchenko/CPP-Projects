# **TGA File Encryption/Decryption Using OpenSSL** 

## **Overview** 

This project is a C++ implementation that provides encryption and decryption functionalities for TGA image files using the **OpenSSL** library. The primary goal is to securely encrypt the image data while preserving the integrity of the header and to correctly decrypt the image data back to its original form.

## **Project Structure** 

The project consists of the following key files and directories:

- **`main.cpp`**: The main source file containing the implementation of the encryption and decryption functions, as well as test cases.
- **`samples/`**: A directory containing the TGA files used for testing the encryption and decryption processes. It includes original images, encrypted references, and decrypted output files.

## **Functionality** 

### **Key Functions** 

1. **`encrypt_data`**:
    - **Purpose**: Encrypts a TGA file using a specified cryptographic configuration.
    - **Parameters**:
        - `in_filename`: The input TGA file to be encrypted.
        - `out_filename`: The output file where the encrypted data will be saved.
        - `config`: A `crypto_config` structure that holds the cryptographic settings such as the cipher type, key, and IV.
    - **Return**: `True` if encryption is successful, `False` otherwise.

2. **`decrypt_data`**:
    - **Purpose**: Decrypts a previously encrypted TGA file back to its original form.
    - **Parameters**:
        - `in_filename`: The input encrypted TGA file.
        - `out_filename`: The output file where the decrypted data will be saved.
        - `config`: A `crypto_config` structure that holds the cryptographic settings such as the cipher type, key, and IV.
    - **Return**: `True` if decryption is successful, `False` otherwise.

3. **`compare_files`**:
    - **Purpose**: Compares two files to check if they are identical.
    - **Parameters**:
        - `name1`: The first file to compare.
        - `name2`: The second file to compare.
    - **Return**: `True` if the files are identical, `False` otherwise.

### **Crypto Configuration (`crypto_config`)** 

The `crypto_config` structure is used to define the cryptographic settings for the encryption and decryption processes. It contains:

- **`m_crypto_function`**: The name of the cryptographic function (e.g., `"AES-128-ECB"`, `"AES-128-CBC"`).
- **`m_key`**: A unique pointer to the encryption/decryption key.
- **`m_IV`**: A unique pointer to the Initialization Vector (IV) if required by the chosen cryptographic function.
- **`m_key_len`**: The length of the key.
- **`m_IV_len`**: The length of the IV.

## **How It Works** ⚙

1. **Header Handling**: The first 18 bytes of the TGA file, representing the header, are read and copied directly to the output file without any encryption or modification.

2. **Data Encryption/Decryption**:
    - The rest of the file (including any optional header data and image data) is encrypted or decrypted using the OpenSSL library. The operation depends on the provided configuration and whether encryption or decryption is requested.
    - The `EVP` functions from OpenSSL are used to perform these operations, ensuring that the data is securely processed according to the selected cryptographic method.

3. **Key and IV Generation**:
    - If the provided key or IV is not sufficient, the program will automatically generate secure keys and IVs using the OpenSSL random functions.

4. **File Comparison**: After encryption and decryption, the `compare_files` function is used to ensure the decrypted file matches the original file, verifying the correctness of the encryption/decryption process.

## **Running Tests** 

### **Test Cases**

The `main.cpp` includes various test cases to validate the functionality of the encryption and decryption processes. The tests are structured to:

- Encrypt original TGA files using both ECB and CBC modes.
- Decrypt the encrypted files and compare them with the original files to ensure they match.
- Compare the encrypted files against known reference encrypted files to validate encryption correctness.

### **Running the Tests** 🧪

#### **Compile the Project:**
- Use a C++ compiler like `g++` to compile the `main.cpp` file with OpenSSL linked:

```bash
g++ -o encrypt_decrypt main.cpp -lcrypto -lssl
```
#### **Execute the Binary**:
- Run the compiled binary to execute the test cases:
```bash
./encrypt_decrypt
```

