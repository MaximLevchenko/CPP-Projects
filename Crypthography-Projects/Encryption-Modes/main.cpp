#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

struct crypto_config {
    const char *m_crypto_function;
    std::unique_ptr<uint8_t[]> m_key;
    std::unique_ptr<uint8_t[]> m_IV;
    size_t m_key_len;
    size_t m_IV_len;
};

#endif /* _PROGTEST_ */

/**
 * Reads the first 18 bytes of the input file, which represents the header.
 * @param input_file The input file stream from which to read the header.
 * @param header_buffer A buffer to store the header data.
 * @return True if the header was successfully read, false otherwise.
 */
bool readHeader(std::ifstream &input_file, unsigned char *header_buffer) {

    input_file.read((char *) (header_buffer), 18);
    if (!input_file) {
        input_file.close();
        return false;
    }
    return true;
}

/**
 * Writes a buffer to the output file.
 * @param out_file The output file stream where the data will be written.
 * @param buffer The buffer containing the data to be written.
 * @param buffer_size The size of the buffer in bytes.
 * @return True if the data was successfully written, false otherwise.
 */
bool writeToOutputFile(std::ofstream &out_file, unsigned char *buffer, const int &buffer_size) {
    if (!out_file.write((char *) (buffer), buffer_size).good()) {
        out_file.close();
        return false;
    }
    return true;
}

/**
 * Encrypts the main part of the data from the input file and writes the encrypted data to the output file.
 * @param ctx The OpenSSL cipher context.
 * @param in_file The input file stream to read data from.
 * @param out_file The output file stream to write encrypted data to.
 * @param buffer_read A buffer to store the data read from the input file.
 * @param st A buffer to store the encrypted data.
 * @param st_length A reference to an integer where the length of the encrypted data will be stored.
 * @param ot_length The size of the buffer_read in bytes.
 * @return True if encryption and writing were successful, false otherwise.
 */
bool encryptMainPartOfData(EVP_CIPHER_CTX *&ctx, std::ifstream &in_file, std::ofstream &out_file,
                           unsigned char *&buffer_read, unsigned char *&st, int &st_length, const int ot_length) {
    while (true) {
        in_file.read((char *) buffer_read, ot_length);
        if (in_file.fail()) {
            if (!in_file.eof()) {
                std::cerr << "Failed to read input file" << std::endl;
                in_file.close();
                out_file.close();
                EVP_CIPHER_CTX_free(ctx);
                delete[] buffer_read;
                delete[] st;
                return false;
            } else {
                break;
            }
        }
        std::streamsize num_read = in_file.gcount();
        if (EVP_CipherUpdate(ctx, st, &st_length, buffer_read, static_cast<int>(num_read)) != 1) {
            in_file.close();
            out_file.close();
            EVP_CIPHER_CTX_free(ctx);
            delete[] buffer_read;
            delete[] st;
            return false;
        }
        if (!writeToOutputFile(out_file, st, st_length)) // if write() failed
        {
            in_file.close();
            EVP_CIPHER_CTX_free(ctx);
            delete[] buffer_read;
            delete[] st;
            return false;
        }

    }

    return true;
}

/**
 * Encrypts the remaining part of the data from the input file that doesn't fit into the main buffer size.
 * @param ctx The OpenSSL cipher context.
 * @param in_file The input file stream to read data from.
 * @param out_file The output file stream to write encrypted data to.
 * @param buffer_read A buffer to store the data read from the input file.
 * @param st A buffer to store the encrypted data.
 * @param st_length A reference to an integer where the length of the encrypted data will be stored.
 * @return True if encryption and writing were successful, false otherwise.
 */
bool encryptRemainderPartOfData(EVP_CIPHER_CTX *&ctx, std::ifstream &in_file, std::ofstream &out_file,
                                unsigned char *&buffer_read, unsigned char *&st, int &st_length) {
    if (in_file.gcount() > 0) {
        std::streamsize num_read = in_file.gcount();
        if (EVP_CipherUpdate(ctx, st, &st_length, buffer_read, static_cast<int>(num_read)) != 1) {
            in_file.close();
            out_file.close();
            EVP_CIPHER_CTX_free(ctx);
            delete[] buffer_read;
            delete[] st;
            return false;
        }
        if (!writeToOutputFile(out_file, st, st_length)) // if write() failed
        {
            in_file.close();
            EVP_CIPHER_CTX_free(ctx);
            delete[] buffer_read;
            delete[] st;
            return false;
        }
    }
    return true;
}

/**
 * Finalizes the encryption process by handling any necessary padding and writing the final encrypted block.
 * @param ctx The OpenSSL cipher context.
 * @param in_file The input file stream to close if an error occurs.
 * @param out_file The output file stream to close if an error occurs.
 * @param buffer_read A buffer to be deleted if an error occurs.
 * @param st A buffer to store the final encrypted data.
 * @param st_length A reference to an integer where the length of the encrypted data will be stored.
 * @return True if padding encryption and writing were successful, false otherwise.
 */
bool encryptPaddingPartOfData(EVP_CIPHER_CTX *&ctx, std::ifstream &in_file, std::ofstream &out_file,
                              unsigned char *&buffer_read, unsigned char *&st, int &st_length) {
    if (EVP_CipherFinal_ex(ctx, st, &st_length) != 1) {
        in_file.close();
        out_file.close();
        EVP_CIPHER_CTX_free(ctx);
        delete[] buffer_read;
        delete[] st;
        return false;
    }
    if (!writeToOutputFile(out_file, st, st_length)) // if write() failed
    {
        in_file.close();
        EVP_CIPHER_CTX_free(ctx);
        delete[] buffer_read;
        delete[] st;
        return false;
    }
    return true;
}

/**
 * Checks if the input file has a valid size, ensuring it is larger than the header size (18 bytes).
 * @param in_file The input file stream to check.
 * @return True if the file size is valid, false otherwise.
 */
bool checkForFileSize(std::ifstream &in_file) {
    std::streampos begin = in_file.tellg();
    in_file.seekg(0, std::ios::end);
    std::streampos end = in_file.tellg();
    in_file.seekg(0, std::ios::beg); // reset back to start
    return end - begin > 18; // if there is only header or less than 18 bytes, return false
}

/**
 * Encrypts or decrypts the data of a TGA file based on the given configuration.
 * @param in_filename The name of the input file to be encrypted/decrypted.
 * @param out_filename The name of the output file where the result will be saved.
 * @param config The crypto configuration, including the cipher function, key, and IV.
 * @param encrypt True for encryption, false for decryption.
 * @return True if the operation was successful, false otherwise.
 */
bool encryptDecryptData(const std::string &in_filename, const std::string &out_filename, crypto_config &config,
                        bool encrypt) {
    const int ot_length = 1024;
    unsigned char header_buffer[18];
    const int header_size = 18;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == nullptr) return false;
    std::ifstream in_file(in_filename, ios::binary);

    if (!in_file.is_open() || !in_file.good()) {
        EVP_CIPHER_CTX_free(ctx);
        return false; // failed to open file
    }
    if (!checkForFileSize(in_file)) {
        in_file.close();
        EVP_CIPHER_CTX_free(ctx);
        return false; // invalid file size
    }
    std::ofstream out_file(out_filename, ios::binary);
    if (!out_file.is_open() || !out_file.good()) {
        in_file.close();
        EVP_CIPHER_CTX_free(ctx);
        return false; // failed to open file
    }

    // Read header and write it to the output file without modification
    if (!readHeader(in_file, header_buffer)) {
        out_file.close();
        EVP_CIPHER_CTX_free(ctx);
        return false; // failed to read header
    }
    if (!writeToOutputFile(out_file, header_buffer, header_size)) {
        in_file.close();
        EVP_CIPHER_CTX_free(ctx);
        return false; // failed to write header
    }

    const EVP_CIPHER *type = EVP_get_cipherbyname(config.m_crypto_function);
    if (!type) {
        in_file.close();
        out_file.close();
        EVP_CIPHER_CTX_free(ctx);
        return false; // failed to get cipher type
    }

    size_t key_len = EVP_CIPHER_key_length(type);
    size_t iv_len = EVP_CIPHER_iv_length(type);
    if (config.m_key == nullptr || config.m_key_len < key_len) {
        if (!encrypt) {
            in_file.close();
            out_file.close();
            EVP_CIPHER_CTX_free(ctx);
            return false; // missing or invalid decryption key
        }
        config.m_key = make_unique<uint8_t[]>(key_len);
        if (RAND_bytes(config.m_key.get(), (int) key_len) != 1) {
            in_file.close();
            out_file.close();
            EVP_CIPHER_CTX_free(ctx);
            return false; // failed to generate key
        }
        config.m_key_len = key_len;
    }
    if (iv_len > 0 && (config.m_IV == nullptr || config.m_IV_len < iv_len)) {
        if (!encrypt) {
            in_file.close();
            out_file.close();
            EVP_CIPHER_CTX_free(ctx);
            return false; // missing or invalid decryption IV
        }
        config.m_IV = make_unique<uint8_t[]>(iv_len);
        if (RAND_bytes(config.m_IV.get(), (int) iv_len) != 1) {
            in_file.close();
            out_file.close();
            EVP_CIPHER_CTX_free(ctx);
            return false; // failed to generate IV
        }
        config.m_IV_len = iv_len;
    }
    bool operation;
    encrypt ? operation = true : operation = false; // if encrypt, then flag should be 1, for decrypt 0
    if (!EVP_CipherInit_ex(ctx, type, nullptr, config.m_key.get(), config.m_IV.get(), operation)) {
        in_file.close();
        out_file.close();
        EVP_CIPHER_CTX_free(ctx);
        return false; // failed to initialize cipher
    }

    auto *buffer_read = new unsigned char[ot_length];
    auto *st = new unsigned char[ot_length + EVP_MAX_BLOCK_LENGTH]; // EVP_MAX_BLOCK_LENGTH is for padding space
    int st_length;

    // Encrypt or decrypt the main part of the data
    if (!encryptMainPartOfData(ctx, in_file, out_file, buffer_read, st, st_length, ot_length)) return false;

    // Encrypt or decrypt the remaining bytes if the file size is not divisible by the buffer size
    if (!encryptRemainderPartOfData(ctx, in_file, out_file, buffer_read, st, st_length)) return false;

    // Encrypt or decrypt the final padding block
    if (!encryptPaddingPartOfData(ctx, in_file, out_file, buffer_read, st, st_length)) return false;

    delete[] buffer_read;
    delete[] st;
    EVP_CIPHER_CTX_free(ctx);
    in_file.close();
    out_file.close();
    return true;
}

/**
 * Encrypts a TGA file with the given configuration.
 * @param in_filename The name of the input file to be encrypted.
 * @param out_filename The name of the output file where the encrypted data will be saved.
 * @param config The crypto configuration, including the cipher function, key, and IV.
 * @return True if the encryption was successful, false otherwise.
 */
bool encrypt_data(const std::string &in_filename, const std::string &out_filename, crypto_config &config) {
    return encryptDecryptData(in_filename, out_filename, config, true);
}

/**
 * Decrypts a TGA file with the given configuration.
 * @param in_filename The name of the input file to be decrypted.
 * @param out_filename The name of the output file where the decrypted data will be saved.
 * @param config The crypto configuration, including the cipher function, key, and IV.
 * @return True if the decryption was successful, false otherwise.
 */
bool decrypt_data(const std::string &in_filename, const std::string &out_filename, crypto_config &config) {
    return encryptDecryptData(in_filename, out_filename, config, false);
}

#ifndef __PROGTEST__

/**
 * Compares two files to check if they are identical.
 * @param name1 The name of the first file to compare.
 * @param name2 The name of the second file to compare.
 * @return True if the files are identical, false otherwise.
 */
bool compare_files(const char *name1, const char *name2) {
    // Implementation would go here
}

/**
 * Main function to test the encryption and decryption functionality.
 * @return 0 if all tests pass, otherwise an error code.
 */
int main(void) {
    crypto_config config{nullptr, nullptr, nullptr, 0, 0};

// ECB mode
    config.m_crypto_function = "AES-128-ECB";
    config.m_key = std::make_unique<uint8_t[]>(16);
    memset(config.m_key.get(), 0, 16);
    config.m_key_len = 16;

    encrypt_data("samples/homer-simpson.TGA", "samples/out_file.TGA", config);
    assert(encrypt_data("samples/homer-simpson.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/homer-simpson_enc_ecb.TGA"));

    decrypt_data("samples/homer-simpson_enc_ecb.TGA", "samples/out_file.TGA", config);
    assert(compare_files("samples/out_file.TGA", "samples/homer-simpson.TGA"));

    assert(encrypt_data("samples/UCM8.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/UCM8_enc_ecb.TGA"));

    assert(decrypt_data("samples/UCM8_enc_ecb.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/UCM8.TGA"));

    assert(encrypt_data("samples/image_1.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/ref_1_enc_ecb.TGA"));

    assert(encrypt_data("samples/image_2.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/ref_2_enc_ecb.TGA"));

    assert(decrypt_data("samples/image_3_enc_ecb.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/ref_3_dec_ecb.TGA"));

    assert(decrypt_data("samples/image_4_enc_ecb.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/ref_4_dec_ecb.TGA"));

// CBC mode
    config.m_crypto_function = "AES-128-CBC";
    config.m_IV = std::make_unique<uint8_t[]>(16);
    config.m_IV_len = 16;
    memset(config.m_IV.get(), 0, 16);

    assert(encrypt_data("samples/UCM8.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/UCM8_enc_cbc.TGA"));

    assert(decrypt_data("samples/UCM8_enc_cbc.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/UCM8.TGA"));

    assert(encrypt_data("samples/homer-simpson.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/homer-simpson_enc_cbc.TGA"));

    assert(decrypt_data("samples/homer-simpson_enc_cbc.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/homer-simpson.TGA"));

    assert(encrypt_data("samples/image_1.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/ref_5_enc_cbc.TGA"));

    assert(encrypt_data("samples/image_2.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/ref_6_enc_cbc.TGA"));

    assert(decrypt_data("samples/image_7_enc_cbc.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/ref_7_dec_cbc.TGA"));

    assert(decrypt_data("samples/image_8_enc_cbc.TGA", "samples/out_file.TGA", config) &&
           compare_files("samples/out_file.TGA", "samples/ref_8_dec_cbc.TGA"));

    return 0;

}

#endif /* _PROGTEST_ */
