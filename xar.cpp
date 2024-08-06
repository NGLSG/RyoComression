#include "xar.h"
#include "utils.h"

namespace RC {
    std::vector<std::string> xar::List(const std::string&filePath) {
        archive* a = archive_read_new();
        int r = archive_read_support_format_xar(a);
        if (r != ARCHIVE_OK) {
            std::cerr << "set format error:" << archive_error_string(a) << std::endl;
            archive_write_free(a);
            return {};
        }
        return Utils::impl_Compress::impl_list(a, filePath);
    }

    bool xar::Extract(const std::string&filePath, const std::string&outputPath) {
        archive* a = archive_read_new();
        int r = archive_read_support_format_xar(a);
        if (r != ARCHIVE_OK) {
            std::cerr << "set format error:" << archive_error_string(a) << std::endl;
            archive_write_free(a);
            return false;
        }
        return Utils::impl_Compress::impl_extract(a, filePath, outputPath);
    }

    bool xar::Compress(const std::string&filePath, const std::string&outputPath, int level) {
        archive* a = archive_write_new();

        int r = archive_write_set_format_xar(a);
        if (r != ARCHIVE_OK) {
            std::cerr << "set format error:" << archive_error_string(a) << std::endl;
            archive_write_free(a);
            return false;
        }
        return Utils::impl_Compress::impl_compress(a, filePath, outputPath, level);
    }

    bool xar::Compress(std::vector<std::string> files, const std::string&outPath, int level, long split) {
        archive* a = archive_write_new();
        int r = archive_write_set_format_xar(a);
        if (r != ARCHIVE_OK) {
            std::cerr << "set format error:" << archive_error_string(a) << std::endl;
            archive_write_free(a);
            return false;
        }
        return Utils::impl_Compress::impl_compress(a, files, outPath, level, split);
    }

    bool xar::ExtractSelectedFile(const std::string&filePath, const std::string&selectedFile,
                                  const std::string&outputPath) {
        archive* a = archive_read_new();
        int r = archive_read_support_format_xar(a);
        if (r != ARCHIVE_OK) {
            std::cerr << "set format error:" << archive_error_string(a) << std::endl;
            archive_write_free(a);
            return false;
        }

        return Utils::impl_Compress::impl_extractSelectedFile(a, filePath, selectedFile, outputPath);
    }
} // RC
