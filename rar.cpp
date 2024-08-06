#include "rar.h"
#include "utils.h"
namespace RC {
    std::vector<std::string> rar::List(const std::string&filePath) {
        archive* a = archive_read_new();
        archive_read_support_format_rar(a);
        archive_read_support_filter_all(a);
        return Utils::impl_Compress::impl_list(a, filePath);
    }

    bool rar::Extract(const std::string&filePath, const std::string&outputPath) {
        archive* a = archive_read_new();
        archive_read_support_format_rar(a);
        archive_read_support_filter_all(a);
        return Utils::impl_Compress::impl_extract(a, filePath, outputPath);
    }

    bool rar::Compress(const std::string&filePath, const std::string&outputPath, int level) {
        std::cout << "Unsupported format: RAR" << std::endl;
        return false;
    }

    bool rar::Compress(std::vector<std::string> files, const std::string& outPath, int level, long split) {
        std::cout << "Unsupported format: RAR" << std::endl;
        return false;
    }

    bool rar::ExtractSelectedFile(const std::string&filePath, const std::string&selectedFile,
                                  const std::string&outputPath) {
        archive* a = archive_read_new();
        archive_read_support_format_rar(a);
        archive_read_support_filter_all(a);
        return Utils::impl_Compress::impl_extractSelectedFile(a, filePath, selectedFile, outputPath);
    }
}
