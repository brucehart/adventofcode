#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <openssl/evp.h>
#include <cstring>

std::string computeMD5(const std::string& input) {
    // Create a context for the hash
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) {
        throw std::runtime_error("Failed to create MD5 context");
    }

    // Initialize the context with the MD5 algorithm
    if (EVP_DigestInit_ex(context, EVP_md5(), nullptr) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to initialize MD5 context");
    }

    // Feed the data into the hash function
    if (EVP_DigestUpdate(context, input.c_str(), input.size()) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to update MD5 context");
    }

    // Finalize and retrieve the hash
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;
    if (EVP_DigestFinal_ex(context, hash, &length) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to finalize MD5 context");
    }

    // Free the context
    EVP_MD_CTX_free(context);

    // Convert hash to hexadecimal string
    std::ostringstream oss;
    for (unsigned int i = 0; i < length; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return oss.str();
}

int main() {
    std::string input;
    std::cin >> input;
    unsigned int i = 0;    

    while(true){
        std::string hash = computeMD5(input + std::to_string(i));
        if(hash.substr(0, 5) == "00000"){
            std::cout << i << std::endl;
            break;
        }

        i++;
    }
        
    return 0;
}