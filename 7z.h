#ifndef INC_7Z_H
#define INC_7Z_H
#include <string>
#include <vector>

namespace RC {
    class c7z {
    public :
        static std::vector<std::string> List(const std::string&filePath);

        static bool Extract(const std::string&filePath, const std::string&outputPath);

        static bool Compress(const std::string&filePath, const std::string&outputPath, int level, long split = 1);

        static bool ExtractSelectedFile(const std::string&filePath, const std::string&selectedFile,
                                        const std::string&outputPath);
    };
}
#endif //INC_7Z_H
