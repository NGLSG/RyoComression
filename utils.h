#ifndef UTILS_H
#define UTILS_H
#include <filesystem>
#include <fstream>
#include <iostream>
#include <archive.h>
#include <archive_entry.h>

namespace RC {
    class Utils {
    public :
        static void sleep(int milliseconds);
        class File {
        public :
            static bool Exists(const std::string&path);

            static size_t FileSize(const std::string&path);

            static std::string FileExtension(const std::string&path);

            static std::string FileName(const std::string&path);

            static std::string FileNameWithoutExtension(const std::string&path);

            static std::string GetParentDirectory(const std::string&path);

            static std::string PlatformPath(const std::string&path);

            static std::string GetCurrentDirectoryA();

            static std::string AbsolutePath(const std::string&path);

            static bool Copy(const std::string&source, const std::string&destination);

            static void Move(const std::string&source, const std::string&destination);

            static bool Remove(const std::string&path);
        };

        class Directory {
        public :
            static bool Create(const std::string&path);

            static bool Remove(const std::string&path);

            static size_t SizeOf(const std::string&path);

            static std::vector<std::string> List(const std::string&path);

            static std::vector<std::string> ListAll(const std::string&path);

            static bool Exists(const std::string&path);

            static bool IsDirectory(const std::string&path);

            static bool Copy(const std::string&source, const std::string&destination);

            static void Move(const std::string&source, const std::string&destination);
        };

        class impl_Compress {
        public :
            static std::vector<std::string> impl_list(archive* a, const std::string&file);

            static bool impl_extract(archive* a, const std::string&filePath, const std::string&outputPath);


            static bool impl_compress(archive* a, const std::string&filePath, const std::string&outputPath,
                                      int level, int split = 0);

            static bool impl_extractSelectedFile(archive* a, const std::string&filePath, const std::string&selectedFile,
                                                 const std::string&outputPath);

        private:
            static bool add_file_to_archive(archive* a, std::string path, const std::string&basePath);

            static bool cleanup(archive* a);
        };
    };
}
#endif //UTILS_H
