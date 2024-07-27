#ifndef LZMA_H
#define LZMA_H


namespace RC {
    class lzma {
    public:
        static std::vector<std::string> List(const std::string&filePath);

        static bool Extract(const std::string&filePath, const std::string&outputPath);

        static bool Compress(const std::string&filePath, const std::string&outputPath, int level);

        static bool ExtractSelectedFile(const std::string&filePath, const std::string&selectedFile,
                                        const std::string&outputPath);
    };
}



#endif //LZMA_H
