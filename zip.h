#ifndef ZIP_H
#define ZIP_H

#include <functional>
#include <minizip/unzip.h>
#include <minizip/zip.h>

namespace RC {
    class zip {
    public:
        static std::vector<std::string> List(const std::string&filePath, const std::string&pwd = "");

        static bool Extract(const std::string&filePath, const std::string&outputPath, const std::string&pwd = "");

        static bool Compress(const std::string&filePath, const std::string&outputPath, int level,
                             const std::string&pwd = "", long split = 1);

        static bool Compress(std::vector<std::string> files, const std::string&outPath, const std::string&pwd = "",
                             int level, long split = 1);

        static bool ExtractSelectedFile(const std::string&filePath, const std::string&selectedFile,
                                        const std::string&outputPath, const std::string&pwd = "");

        static bool MergeParts(const std::string&outputZipPath,
                               const std::string&destPath, const std::string&pwd = "");

    private:
        static bool tryPwd(const std::string&filePath, const std::string&pwd);

        static bool zipExtractSingle(const std::string&filePath, const std::string&outputPath,
                                     const std::string&pwd);

        static bool zipExtractMulti(const std::string&zipPath, const std::string&outputPath,
                                    const std::string&pwd);

        static bool addFileToZip(zipFile zipFile, const std::string&filePath, const std::string&relativePath,
                                 const std::string&password, int compressionLevel, uint64_t&currentVolumeSize,
                                 long splitSize,
                                 std::function<bool()> openNewVolume);

        static bool addDirectoryToZip(zipFile zipFile, const std::string&directoryPath, const std::string&relativePath,
                                      const std::string&password, int compressionLevel);

        static bool addPathToZip(zipFile zipFile, const std::string&path, const std::string&relativePath,
                                 const std::string&password, int compressionLevel, uint64_t&currentVolumeSize,
                                 long splitSize,
                                 std::function<bool()> openNewVolume);
    };
}

#endif //ZIP_H
