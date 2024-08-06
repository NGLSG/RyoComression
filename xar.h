#ifndef XTAR_H
#define XTAR_H

namespace RC {
    class xar {
    public:
        static std::vector<std::string> List(const std::string&filePath);

        static bool Extract(const std::string&filePath, const std::string&outputPath);

        static bool Compress(const std::string&filePath, const std::string&outputPath, int level);

        static bool Compress(std::vector<std::string> files, const std::string&outPath, int level, long split = 1);

        static bool ExtractSelectedFile(const std::string&filePath, const std::string&selectedFile,
                                        const std::string&outputPath);
    };
} // RC

#endif //XTAR_H
