#include "Encryption.h"
#include "utils.h"
OPENSSL_MSVC_PRAGMA(warning(push))
OPENSSL_MSVC_PRAGMA(warning(disable: 4702))
OPENSSL_MSVC_PRAGMA(warning(pop))

namespace Encryption {
    void AES::Encrypt(std::string Path, std::string Password, std::string iv, std::string OutputPath, bool delAfter) {
        const std::string key = Password;

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        EVP_CIPHER_CTX_init(ctx);

        EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, reinterpret_cast<const unsigned char *>(key.c_str()),
                           reinterpret_cast<const unsigned char *>(iv.c_str()));

        // 打开待加密文件
        std::ifstream infile(Path, std::ios::binary);
        if (!infile) {
            std::cerr << "Unable to open input file." << std::endl;
            EVP_CIPHER_CTX_free(ctx);
            return;
        }

        // 读取文件内容
        std::vector<unsigned char> data(std::istreambuf_iterator<char>(infile), {});
        infile.close();

        // 为加密数据分配足够的空间（加上填充）
        std::vector<unsigned char> encryptedData(data.size() + AES_BLOCK_SIZE);

        // 执行加密
        int len = 0;
        int flags = 0;
        EVP_EncryptUpdate(ctx, &encryptedData[0], &len, data.data(), data.size());
        EVP_EncryptFinal_ex(ctx, &(encryptedData[len]), &flags);

        // 写入加密后的数据到新文件
        std::ofstream outfile(OutputPath, std::ios::binary);
        if (outfile) {
            outfile.write(reinterpret_cast<char *>(&encryptedData[0]), len + flags);
            outfile.close();
            if (delAfter)
                RC::Utils::File::Remove(Path);
        }
        else {
            std::cerr << "Unable to open output file." << std::endl;
        }

        // 清理
        EVP_CIPHER_CTX_free(ctx);
    }

    void AES::Decrypt(const std::string& inputPath, const std::string& password, const std::string& iv,
        const std::string& outputPath, bool delAfter) {
        const std::string key = password;

        // 创建EVP_CIPHER_CTX
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        EVP_CIPHER_CTX_init(ctx);

        // 设置解密算法，这里使用AES-256-CBC
        EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(key.c_str()),
                           reinterpret_cast<const unsigned char *>(iv.c_str()));

        // 打开待解密文件
        std::ifstream infile(inputPath, std::ios::binary);
        if (!infile) {
            std::cerr << "Unable to open input file for decryption." << std::endl;
            EVP_CIPHER_CTX_free(ctx);
            return;
        }

        // 读取文件内容
        std::vector<unsigned char> data(std::istreambuf_iterator<char>(infile), {});
        infile.close();

        // 为解密数据分配空间
        std::vector<unsigned char> decryptedData(data.size());

        // 执行解密
        int len = 0;
        int flags = 0;
        if (!EVP_DecryptUpdate(ctx, &decryptedData[0], &len, data.data(), data.size())) {
            std::cerr << "Failed to decrypt data." << std::endl;
            // 处理解密错误，例如打印错误信息
            EVP_CIPHER_CTX_free(ctx);
            return;
        }
        if (!EVP_DecryptFinal_ex(ctx, &(decryptedData[len]), &flags)) {
            std::cerr << "Failed to finalize decryption." << std::endl;
            // 处理解密错误
        }

        // 写入解密后的数据到新文件
        std::ofstream outfile(outputPath, std::ios::binary);
        if (outfile) {
            outfile.write(reinterpret_cast<char *>(&decryptedData[0]), len + flags);
            outfile.close();
            if (delAfter) {
                // 假设 RC::Utils::File::Remove 是一个有效的文件删除函数
                RC::Utils::File::Remove(inputPath);
            }
        }
        else {
            std::cerr << "Unable to open output file for decrypted data." << std::endl;
        }

        // 清理
        EVP_CIPHER_CTX_free(ctx);
    }
}
