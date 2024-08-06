#include "Compression.h"

#include "7z.h"
#include "zip.h"
#include "bzip.h"
#include "lzma.h"
#include "tar.h"
#include "xz.h"
#include "zipx.h"
#include "gzip.h"
#include "rar.h"
#include "ar.h"
#include "xar.h"
#include "zstd.h"
#include "utils.h"

namespace RC {
    std::vector<std::string> Compression::List(const std::string&file, const Type&type, const std::string&pwd) {
        switch (type) {
            case Count: {
                std::string fileExt = Utils::File::FileExtension(file);
                if (strcmp(fileExt.c_str(), "7z") == 0) {
                    return c7z::List(file);
                }
                if (strcmp(fileExt.c_str(), "zip") == 0) {
                    return zip::List(file, pwd);
                }
                if (strcmp(fileExt.c_str(), "bz2") == 0) {
                    return bzip::List(file);
                }
                if (strcmp(fileExt.c_str(), "lzma") == 0) {
                    return lzma::List(file);
                }
                if (strcmp(fileExt.c_str(), "tar") == 0) {
                    return tar::List(file);
                }
                if (strcmp(fileExt.c_str(), "xz") == 0) {
                    return xz::List(file);
                }
                if (strcmp(fileExt.c_str(), "zipx") == 0) {
                    return zipx::List(file);
                }
                if (strcmp(fileExt.c_str(), "gz") == 0) {
                    return gzip::List(file);
                }
                if (strcmp(fileExt.c_str(), "rar") == 0) {
                    return rar::List(file);
                }
                if (strcmp(fileExt.c_str(), "ar") == 0) {
                    return ar::List(file);
                }
                if (strcmp(fileExt.c_str(), "xar") == 0) {
                    return xar::List(file);
                }
                if (strcmp(fileExt.c_str(), "zst") == 0) {
                    return zstd::List(file);
                }
                std::cout << "Unsupported file extension:" << fileExt << std::endl;
                return {};
            }
            case ZIP: {
                return zip::List(file, pwd);
            }
            case ZIPX: {
                return zipx::List(file);
            }
            case RAR: {
                return rar::List(file);
            }
            case TAR: {
                return tar::List(file);
            }
            case TAR_BZ2: {
                return bzip::List(file);
            }
            case TAR_GZ: {
                return gzip::List(file);
            }
            case TAR_LZ4: {
                return lzma::List(file);
            }
            case TAR_LZMA: {
                return lzma::List(file);
            }
            case TAR_LZMA2: {
                return lzma::List(file);
            }
            case TAR_XZ: {
                return xz::List(file);
            }
            case TAR_ZSTD: {
                return zstd::List(file);
            }
            case XAR: {
                return xar::List(file);
            }
            case AR: {
                return ar::List(file);
            }
            default:
                std::cout << "Unsupported file type:" << type << std::endl;
                return {};
        }
    }

    bool Compression::Extract(const std::string&file, const std::string&dest, const Type&type, const std::string&pwd) {
        switch (type) {
            case Count: {
                std::string fileExt = Utils::File::FileExtension(file);
                if (strcmp(fileExt.c_str(), "7z") == 0) {
                    return c7z::Extract(file, dest);
                }
                if (strcmp(fileExt.c_str(), "zip") == 0) {
                    return zip::Extract(file, dest, pwd);
                }
                if (strcmp(fileExt.c_str(), "bz2") == 0) {
                    return bzip::Extract(file, dest);
                }
                if (strcmp(fileExt.c_str(), "lzma") == 0) {
                    return lzma::Extract(file, dest);
                }
                if (strcmp(fileExt.c_str(), "tar") == 0) {
                    return tar::Extract(file, dest);
                }
                if (strcmp(fileExt.c_str(), "xz") == 0) {
                    return xz::Extract(file, dest);
                }
                if (strcmp(fileExt.c_str(), "zipx") == 0) {
                    return zipx::Extract(file, dest);
                }
                if (strcmp(fileExt.c_str(), "gz") == 0) {
                    return gzip::Extract(file, dest);
                }
                if (strcmp(fileExt.c_str(), "rar") == 0) {
                    return rar::Extract(file, dest);
                }
                if (strcmp(fileExt.c_str(), "ar") == 0) {
                    return ar::Extract(file, dest);
                }
                if (strcmp(fileExt.c_str(), "xar") == 0) {
                    return xar::Extract(file, dest);
                }
                if (strcmp(fileExt.c_str(), "zst") == 0) {
                    return zstd::Extract(file, dest);
                }

                std::cout << "Unsupported file extension:" << fileExt << std::endl;
                return false;
            }
            case ZIP: {
                return zip::Extract(file, dest, pwd);
            }
            case ZIPX: {
                return zipx::Extract(file, dest);
            }
            case RAR: {
                return rar::Extract(file, dest);
            }
            case TAR: {
                return tar::Extract(file, dest);
            }
            case TAR_BZ2: {
                return bzip::Extract(file, dest);
            }
            case TAR_GZ: {
                return gzip::Extract(file, dest);
            }
            case TAR_LZ4: {
                return lzma::Extract(file, dest);
            }
            case TAR_LZMA: {
                return lzma::Extract(file, dest);
            }
            case TAR_LZMA2: {
                return lzma::Extract(file, dest);
            }
            case TAR_XZ: {
                return xz::Extract(file, dest);
            }
            case TAR_ZSTD: {
                return zstd::Extract(file, dest);
            }
            case XAR: {
                return xar::Extract(file, dest);
            }
            case AR: {
                return ar::Extract(file, dest);
            }
            default:
                std::cout << "Unsupported file type:" << type << std::endl;
                return false;
        }
    }

    bool Compression::ExtractSelectedFile(const std::string&filePath, const std::string&selectedFile,
                                          const std::string&outputPath, const Type&type, const std::string&pwd) {
        switch (type) {
            case Count: {
                std::string fileExt = Utils::File::FileExtension(filePath);
                if (strcmp(fileExt.c_str(), "7z") == 0) {
                    return c7z::ExtractSelectedFile(filePath, selectedFile, outputPath);
                }
                if (strcmp(fileExt.c_str(), "zip") == 0) {
                    return zip::ExtractSelectedFile(filePath, selectedFile, outputPath, pwd);
                }
                if (strcmp(fileExt.c_str(), "bz2") == 0) {
                    return bzip::ExtractSelectedFile(filePath, selectedFile, outputPath);
                }
                if (strcmp(fileExt.c_str(), "lzma") == 0) {
                    return lzma::ExtractSelectedFile(filePath, selectedFile, outputPath);
                }
                if (strcmp(fileExt.c_str(), "tar") == 0) {
                    return tar::ExtractSelectedFile(filePath, selectedFile, outputPath);
                }
                if (strcmp(fileExt.c_str(), "xz") == 0) {
                    return xz::ExtractSelectedFile(filePath, selectedFile, outputPath);
                }
                if (strcmp(fileExt.c_str(), "zipx") == 0) {
                    return zipx::ExtractSelectedFile(filePath, selectedFile, outputPath);
                }
                if (strcmp(fileExt.c_str(), "gz") == 0) {
                    return gzip::ExtractSelectedFile(filePath, selectedFile, outputPath);
                }
                if (strcmp(fileExt.c_str(), "rar") == 0) {
                    return rar::ExtractSelectedFile(filePath, selectedFile, outputPath);
                }
                if (strcmp(fileExt.c_str(), "ar") == 0) {
                    return ar::ExtractSelectedFile(filePath, selectedFile, outputPath);
                }
                if (strcmp(fileExt.c_str(), "xar") == 0) {
                    return xar::ExtractSelectedFile(filePath, selectedFile, outputPath);
                }
                if (strcmp(fileExt.c_str(), "zst") == 0) {
                    return zstd::ExtractSelectedFile(filePath, selectedFile, outputPath);
                }
                std::cout << "Unsupported file extension:" << fileExt << std::endl;


                return false;
            }
            case ZIP: {
                return zip::ExtractSelectedFile(filePath, selectedFile, outputPath, pwd);
            }
            case ZIPX: {
                return zipx::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            case RAR: {
                return rar::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            case TAR: {
                return tar::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            case TAR_BZ2: {
                return bzip::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            case TAR_GZ: {
                return gzip::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            case TAR_LZ4: {
                return lzma::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            case TAR_LZMA: {
                return lzma::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            case TAR_LZMA2: {
                return lzma::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            case TAR_XZ: {
                return xz::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            case TAR_ZSTD: {
                return zstd::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            case XAR: {
                return xar::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            case AR: {
                return ar::ExtractSelectedFile(filePath, selectedFile, outputPath);
            }
            default:
                std::cout << "Unsupported file type:" << type << std::endl;
                return false;
        }
    }


    bool Compression::Compress(const std::string&file, const std::string&outputPath, const Type&type, int level,
                               const std::string&password, long split) {
        switch (type) {
            case Count: {
                std::string fileExt = Utils::File::FileExtension(outputPath);
                if (strcmp(fileExt.c_str(), "7z") == 0) {
                    return c7z::Compress(file, outputPath, level, split);
                }
                if (strcmp(fileExt.c_str(), "zip") == 0) {
                    return zip::Compress(file, outputPath, level, password, split);
                }
                if (strcmp(fileExt.c_str(), "bz2") == 0) {
                    return bzip::Compress(file, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "lzma") == 0) {
                    return lzma::Compress(file, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "tar") == 0) {
                    return tar::Compress(file, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "xz") == 0) {
                    return xz::Compress(file, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "zipx") == 0) {
                    return zipx::Compress(file, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "gz") == 0) {
                    return gzip::Compress(file, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "rar") == 0) {
                    return rar::Compress(file, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "ar") == 0) {
                    return ar::Compress(file, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "xar") == 0) {
                    return xar::Compress(file, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "zst") == 0) {
                    return zstd::Compress(file, outputPath, level);
                }

                std::cout << "Unsupported file extension:" << fileExt << std::endl;

                return false;
            }
            case ZIP: {
                return zip::Compress(file, outputPath, level, password, split);
            }
            case ZIPX: {
                return zipx::Compress(file, outputPath, level);
            }
            case RAR: {
                return rar::Compress(file, outputPath, level);
            }
            case TAR: {
                return tar::Compress(file, outputPath, level);
            }
            case TAR_BZ2: {
                return bzip::Compress(file, outputPath, level);
            }
            case TAR_GZ: {
                return gzip::Compress(file, outputPath, level);
            }
            case TAR_LZ4: {
                return lzma::Compress(file, outputPath, level);
            }
            case TAR_LZMA: {
                return lzma::Compress(file, outputPath, level);
            }
            case TAR_LZMA2: {
                return lzma::Compress(file, outputPath, level);
            }
            case TAR_XZ: {
                return xz::Compress(file, outputPath, level);
            }
            case TAR_ZSTD: {
                return zstd::Compress(file, outputPath, level);
            }
            case XAR: {
                return xar::Compress(file, outputPath, level);
            }
            case AR: {
                return ar::Compress(file, outputPath, level);
            }
            default:
                std::cout << "Unsupported file type:" << type << std::endl;
                return false;
        }
    }

    bool Compression::Compress(std::vector<std::string> files, const std::string&outputPath, const Type&type,
                               int level, const std::string&password, long split) {
        switch (type) {
            case Count: {
                std::string fileExt = Utils::File::FileExtension(outputPath);
                if (strcmp(fileExt.c_str(), "7z") == 0) {
                    return c7z::Compress(files, outputPath, level, split);
                }
                if (strcmp(fileExt.c_str(), "zip") == 0) {
                    return zip::Compress(files, outputPath, password, level, split);
                }
                if (strcmp(fileExt.c_str(), "bz2") == 0) {
                    return bzip::Compress(files, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "lzma") == 0) {
                    return lzma::Compress(files, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "tar") == 0) {
                    return tar::Compress(files, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "xz") == 0) {
                    return xz::Compress(files, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "zipx") == 0) {
                    return zipx::Compress(files, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "gz") == 0) {
                    return gzip::Compress(files, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "rar") == 0) {
                    return rar::Compress(files, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "ar") == 0) {
                    return ar::Compress(files, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "xar") == 0) {
                    return xar::Compress(files, outputPath, level);
                }
                if (strcmp(fileExt.c_str(), "zst") == 0) {
                    return zstd::Compress(files, outputPath, level);
                }
                std::cout << "Unsupported file extension:" << fileExt << std::endl;
                return false;
            }
            case ZIP: {
                return zip::Compress(files, outputPath, password, level, split);
            }
            case ZIPX: {
                return zipx::Compress(files, outputPath, level);
            }
            case RAR: {
                return rar::Compress(files, outputPath, level);
            }
            case TAR: {
                return tar::Compress(files, outputPath, level);
            }
            case TAR_BZ2: {
                return bzip::Compress(files, outputPath, level);
            }
            case TAR_GZ: {
                return gzip::Compress(files, outputPath, level);
            }
            case TAR_LZ4: {
                return lzma::Compress(files, outputPath, level);
            }
            case TAR_LZMA: {
                return lzma::Compress(files, outputPath, level);
            }
            case TAR_LZMA2: {
                return lzma::Compress(files, outputPath, level);
            }
            case TAR_XZ: {
                return xz::Compress(files, outputPath, level);
            }
            case TAR_ZSTD: {
                return zstd::Compress(files, outputPath, level);
            }
            case XAR: {
                return xar::Compress(files, outputPath, level);
            }
            case AR: {
                return ar::Compress(files, outputPath, level);
            }
            default:
                std::cout << "Unsupported file type:" << type << std::endl;
                return false;
        }
    }


    bool Compression::MergeParts(const std::string&outputZipPath, const std::string&destPath, const Type&type,
                                 const std::string&pwd) {
        std::string fileExt = Utils::File::FileExtension(outputZipPath);
        if (strcmp(fileExt.c_str(), "zip") == 0) {
            return zip::MergeParts(outputZipPath, destPath, pwd);
        }
        std::cout << "Unsupported file extension:" << fileExt << std::endl;
        return false;
    }
}
