#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <random>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

namespace Encryption {
    inline static std::pair<std::string, std::string> GenerateKeyAndIV(size_t keyBits, size_t ivBits) {
        std::string key;
        std::string iv;


        size_t keyBytes = (keyBits + 7) / 8;
        size_t ivBytes = (ivBits + 7) / 8;

        // Ensure valid key and IV sizes
        if (keyBytes == 0 || ivBytes == 0) {
            throw std::invalid_argument("Invalid key or IV size");
        }


        key.resize(keyBytes);
        iv.resize(ivBytes);

        // Generate random key
        if (!RAND_bytes(reinterpret_cast<unsigned char *>(key.data()), keyBytes)) {
            throw std::runtime_error("Failed to generate random key");
        }

        // Generate random IV
        if (!RAND_bytes(reinterpret_cast<unsigned char *>(iv.data()), ivBytes)) {
            throw std::runtime_error("Failed to generate random IV");
        }

        return {key, iv};
    }

    enum Type {
        AES,
    };

    class AES {
    public:
        static void Encrypt(std::string Path, std::string Password, std::string iv, std::string OutputPath,
                            bool delAfter = false);

        static void Decrypt(const std::string&inputPath, const std::string&password, const std::string&iv,
                            const std::string&outputPath, bool delAfter = false);
    };
};


#endif //ENCRYPTION_H
