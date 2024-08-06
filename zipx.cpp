#include "zipx.h"
#include "utils.h"

namespace RC {
    std::vector<std::string> zipx::List(const std::string&filePath) {
        archive* a = archive_read_new();
        archive_read_support_format_zip(a);
        archive_read_support_filter_all(a);
        return Utils::impl_Compress::impl_list(a, filePath);
    }

    bool zipx::Extract(const std::string&filePath, const std::string&outputPath) {
        archive* a = archive_read_new();
        archive_read_support_format_zip(a);
        archive_read_support_filter_all(a);
        return Utils::impl_Compress::impl_extract(a, filePath, outputPath);
    }

    bool zipx::Compress(const std::string&filePath, const std::string&outputPath, int level) {
        archive* a = archive_write_new();
        archive_write_set_format_zip(a);
        archive_write_add_filter_bzip2(a);
        return Utils::impl_Compress::impl_compress(a, filePath, outputPath, level);
    }

    bool zipx::Compress(std::vector<std::string> files, const std::string&outPath, int level, long split) {
        archive* a = archive_write_new();
        archive_write_set_format_zip(a);
        archive_write_add_filter_bzip2(a);
        return Utils::impl_Compress::impl_compress(a, files, outPath, level, split);
    }

    bool zipx::ExtractSelectedFile(const std::string&filePath, const std::string&selectedFile,
                                   const std::string&outputPath) {
        archive* a = archive_read_new();
        archive_read_support_format_zip(a);
        archive_read_support_filter_all(a);
        return Utils::impl_Compress::impl_extractSelectedFile(a, filePath, selectedFile, outputPath);
    }
}
