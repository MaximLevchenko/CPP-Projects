#ifndef __PROGTEST__

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rand.h>

#endif /* __PROGTEST__ */

/**
 * Generates a random 64-byte string using OpenSSL's RAND_bytes function.
 * @param text_to_cipher A pointer to the buffer where the generated string will be stored.
 */
void getRandomlyGeneratedString(unsigned char *text_to_cipher) {

    std::vector<std::string> vec_text;
    RAND_bytes(text_to_cipher, 64);

}

/**
 * Checks if the given hash has the required number of leading zero bits.
 * @param dexHashArray The array representing the hash to be checked.
 * @param bits The required number of leading zero bits.
 * @return True if the hash meets the criteria, false otherwise.
 */
bool hasRightAmountOfNulls(const unsigned char *dexHashArray, const int &bits) {
    size_t full_bytes = bits / 8;
    size_t leftover = bits % 8;
    for (size_t i = 0; i < full_bytes; i++) {
        if (dexHashArray[i] != 0)
            return false;
    }
    if ((dexHashArray[full_bytes] >> (8 - leftover)) != 0)
        return false;
    return true;
}

/**
 * Converts a binary array into its hexadecimal string representation.
 * @param str_to_transform The binary array to be converted.
 * @param hash_tmp A buffer used for temporary storage during the conversion.
 * @return A pointer to the hexadecimal string. The caller is responsible for freeing this memory.
 */
char *transfromToHex(const unsigned char *str_to_transform, unsigned char hash_tmp[64]) {
    size_t str_to_transform_length;
    str_to_transform_length = 64;
    char *buffer_hex = (char *) malloc(str_to_transform_length * 2 + 1);
    for (size_t i = 0; i < str_to_transform_length; i++) {
        sprintf(buffer_hex + 2 * i, "%02x", str_to_transform[i]);
    }
    buffer_hex[2 * str_to_transform_length] = '\0';

    return buffer_hex;
}

/**
 * Finds a random message whose hash, computed with the specified hash function, starts with a given number of leading zero bits.
 * @param bits The required number of leading zero bits in the hash.
 * @param message Pointer to a string where the generated message in hexadecimal format will be stored.
 * @param hash Pointer to a string where the resulting hash in hexadecimal format will be stored.
 * @param hashFunction The name of the hash function to use (e.g., "sha512", "md5").
 * @return 1 if a valid hash is found, 0 if the input parameters are invalid or the hash could not be found.
 */
int findHashEx(int bits, char **message, char **hash, const char *hashFunction) {
    unsigned char text_to_cipher[64] = {0};

    std::string hash_function = hashFunction;  // Chosen hash function ("sha1", "md5", ...)
    EVP_MD_CTX *ctx;  // Context structure for hashing
    const EVP_MD *type; // Type of the chosen hash function
    unsigned char dexHashArray[64] = {0};
    unsigned int length;  // Length of the resulting hash

    OpenSSL_add_all_digests();
    type = EVP_get_digestbyname(hash_function.c_str());
    if (!type) {
        printf("Hash %s does not exist.\n", hash_function.c_str());
        return 0;
    }
    bool found_right_message = false;
    while (true) {
        getRandomlyGeneratedString(text_to_cipher);  // Generate a random string

        ctx = EVP_MD_CTX_new();  // Create context for hashing

        EVP_DigestInit_ex(ctx, type, NULL);  // Initialize the context for the chosen hash type

        EVP_DigestUpdate(ctx, text_to_cipher, 64);  // Feed the message into the context

        EVP_DigestFinal_ex(ctx, dexHashArray, &length);  // Finalize and obtain the hash
        if (bits < 0 || bits > 8 * (int) length)
            return 0;
        if (hasRightAmountOfNulls(dexHashArray, bits)) {
            *hash = transfromToHex(dexHashArray, dexHashArray);
            *message = transfromToHex(text_to_cipher, dexHashArray);
            found_right_message = true;
        }
        EVP_MD_CTX_free(ctx);  // Destroy the context
        if (found_right_message)
            break;
    }

    return 1;
}

/**
 * A specialized version of findHashEx that uses the "sha512" hash function by default.
 * @param bits The required number of leading zero bits in the hash.
 * @param message Pointer to a string where the generated message in hexadecimal format will be stored.
 * @param hash Pointer to a string where the resulting hash in hexadecimal format will be stored.
 * @return 1 if a valid hash is found, 0 if the input parameters are invalid or the hash could not be found.
 */
int findHash(int bits, char **message, char **hash) {
    return findHashEx(bits, message, hash, "sha512");
}

#ifndef __PROGTEST__

/**
 * Placeholder function to check if a hash meets certain criteria.
 * @param bits The required number of leading zero bits in the hash.
 * @param hexString The hash value to be checked.
 * @return 1 if the hash meets the criteria, 0 otherwise (not implemented).
 */
int checkHash(int bits, char *hexString) {
}

/**
 * Main function to test the hash finding functionality.
 * @return EXIT_SUCCESS if all tests pass, otherwise an error code.
 */
int main(void) {
    char *message, *hash;
    std::vector<std::string> p;
//    findHash(0, (char **) p[0].c_str(), &hash);
    findHash(0, &message, &hash);
    assert(findHash(0, &message, &hash) == 1);
//    assert(message && hash && checkHash(0, hash));
    free(message);
    free(hash);
    assert(findHash(1, &message, &hash) == 1);
//    assert(message && hash && checkHash(1, hash));
    free(message);
    free(hash);
    assert(findHash(2, &message, &hash) == 1);
//    assert(message && hash && checkHash(2, hash));
    free(message);
    free(hash);
    assert(findHash(9, &message, &hash) == 1);
//    assert(message && hash && checkHash(3, hash));
    free(message);
    free(hash);
    assert(findHash(-1, &message, &hash) == 0);
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
