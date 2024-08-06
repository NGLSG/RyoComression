#include "7z.h"

#include <iostream>
#include "utils.h"

namespace RC {
    std::vector<std::string> c7z::List(const std::string&filePath) {
        archive* a = archive_read_new();
        archive_read_support_format_7zip(a);
        archive_read_support_format_zip(a);
        return Utils::impl_Compress::impl_list(a, filePath);
    }

    bool c7z::Extract(const std::string&filePath, const std::string&outputPath) {
        archive* a = archive_read_new();
        archive_read_support_format_7zip(a);
        archive_read_support_format_zip(a);
        return Utils::impl_Compress::impl_extract(a, filePath, outputPath);
    }

    bool c7z::Compress(const std::string&filePath, const std::string&outputPath, int level, long split) {
        std::cout << "7z not support" << std::endl;
        return false;
    }

    bool c7z::Compress(std::vector<std::string> files, const std::string& outPath, int level, long split) {
        std::cout << "7z not support" << std::endl;
        return false;
    }

    bool c7z::ExtractSelectedFile(const std::string&filePath, const std::string&selectedFile,
                                  const std::string&outputPath) {
        archive* a = archive_read_new();
        archive_read_support_format_7zip(a);
        archive_read_support_format_zip(a);
        return Utils::impl_Compress::impl_extractSelectedFile(a, filePath, selectedFile, outputPath);
    }
}
