#ifndef COMPRESS_H
#define COMPRESS_H

#include <string>
#include <vector>
#include <map>

namespace RC {
    enum SplitSize {
        Split_1M = 1,
        Split_4M = 4,
        Split_16M = 16,
        Split_64M = 64,
        Split_256M = 256,
        Split_1G = 1024,
        Split_4G = 4096,
        Split_16G = 16384,
    };

    enum Type {
        ZIP = 0,
        RAR = 1,
        TAR = 2,
        TAR_GZ = 3,
        TAR_BZ2 = 4,
        TAR_XZ = 5,
        TAR_LZMA = 6,
        TAR_LZIP = 7,
        TAR_LZMA2 = 8,
        TAR_LZ4 = 10,
        TAR_ZSTD = 11,
        ZIPX = 12,
        AR,
        XAR,
        Count
    };

    class Compression {
    public :
        inline const static std::map<SplitSize, long> SplitMap = {
            {Split_1M, 1024 * 1024},
            {Split_4M, 4 * 1024 * 1024},
            {Split_16M, 16 * 1024 * 1024},
            {Split_64M, 64 * 1024 * 1024},
            {Split_256M, 256 * 1024 * 1024},
            {Split_1G, 1024 * 1024 * 1024},
            {Split_4G, 4 * 1024 * 1024 * 1024},
            {Split_16G, 16 * 1024 * 1024 * 1024},
        };

        static std::vector<std::string> List(const std::string&file, const Type&type = Count,
                                             const std::string&pwd = "");

        static bool Extract(const std::string&file, const std::string&dest, const Type&type = Count,
                            const std::string&pwd = "");

        static bool ExtractSelectedFile(const std::string&filePath, const std::string&selectedFile,
                                        const std::string&outputPath, const Type&type = Count,
                                        const std::string&pwd = "");

        static bool Compress(const std::string&filePath, const std::string&outputPath, const Type&type = Count,
                             int level = 1,
                             const std::string&password = "", long split = 1);

        static bool Compress( std::vector<std::string> files, const std::string&outputPath, const Type&type = Count,
                             int level = 1,
                             const std::string&password = "", long split = 1);

        static bool MergeParts(const std::string&outputZipPath, const std::string&destPath = "",
                               const Type&type = Count,
                               const std::string&pwd = "");
    };
}


#endif //COMPRESS_H
