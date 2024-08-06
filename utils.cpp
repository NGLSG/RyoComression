#include "utils.h"

#include <thread>

namespace RC {
    void Utils::sleep(int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    bool Utils::File::Exists(const std::string&path) {
        return std::filesystem::exists(path);
    }

    size_t Utils::File::FileSize(const std::string&path) {
        return std::filesystem::file_size(path);
    }

    std::string Utils::File::FileExtension(const std::string&path) {
        auto ext = std::filesystem::path(path).extension().string().substr(1);
        // 转换为不带点的字符串
        return ext;
    }

    std::string Utils::File::FileName(const std::string&path) {
        return std::filesystem::path(path).filename().string();
    }

    std::string Utils::File::FileNameWithoutExtension(const std::string&path) {
        return std::filesystem::path(path).stem().string();
    }

    std::string Utils::File::GetParentDirectory(const std::string&path) {
        return std::filesystem::path(path).parent_path().string();
    }

    std::string Utils::File::PlatformPath(const std::string&path) {
        return std::filesystem::path(path).make_preferred().string();
    }

    std::string Utils::File::GetCurrentDirectoryA() {
        return std::filesystem::current_path().string();
    }

    std::string Utils::File::AbsolutePath(const std::string&path) {
        return std::filesystem::absolute(path).string();
    }

    bool Utils::File::Copy(const std::string&source, const std::string&destination) {
        return std::filesystem::copy_file(source, destination);
    }

    void Utils::File::Move(const std::string&source, const std::string&destination) {
        return std::filesystem::rename(source, destination);
    }

    bool Utils::File::Remove(const std::string&path) {
        return std::filesystem::remove(path);
    }

    bool Utils::Directory::Create(const std::string&path) {
        return std::filesystem::create_directories(path);
    }

    bool Utils::Directory::Remove(const std::string&path) {
        return std::filesystem::remove_all(path);
    }

    size_t Utils::Directory::SizeOf(const std::string&path) {
        size_t size = 0;
        auto files = ListAll(path);
        for (const auto&file: files) {
            if (!IsDirectory(file))
                size += File::FileSize(file);
        }
        return size;
    }

    std::vector<std::string> Utils::Directory::List(const std::string&path) {
        if (!Exists(path)) {
            std::cerr << "Directory does not exist: " << path << std::endl;
            return {};
        }
        std::vector<std::string> files;
        for (const auto&file: std::filesystem::directory_iterator(path)) {
            files.push_back(file.path().string());
        }
        return files;
    }

    std::vector<std::string> Utils::Directory::ListAll(const std::string&path) {
        std::vector<std::string> files;
        for (const auto&file: std::filesystem::recursive_directory_iterator(path)) {
            files.push_back(file.path().string());
        }
        return files;
    }

    bool Utils::Directory::Exists(const std::string&path) {
        return std::filesystem::exists(path);
    }

    bool Utils::Directory::IsDirectory(const std::string&path) {
        if (Exists(path))
            return std::filesystem::is_directory(path);
        else
            return (path.back() == '/' || path.back() == '\\');;
    }

    bool Utils::Directory::Copy(const std::string&source, const std::string&destination) {
        std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);
        return true;
    }

    void Utils::Directory::Move(const std::string&source, const std::string&destination) {
        std::filesystem::rename(source, destination);
    }

    std::vector<std::string> Utils::impl_Compress::impl_list(archive* a, const std::string&file) {
        std::vector<std::string> files;
        if (archive_read_open_filename(a, file.c_str(), 10240) != ARCHIVE_OK) {
            std::cerr << "Failed to open archive: " << file << std::endl;
            return {};
        }
        archive_entry* entry;
        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            const char* current_file = archive_entry_pathname(entry);
            files.emplace_back(current_file);
            archive_read_data_skip(a);
        }

        cleanup(a);
        return files;
    }

    bool Utils::impl_Compress::impl_extract(archive* a, const std::string&filePath, const std::string&outputPath) {
        if (!Directory::Exists(outputPath)) {
            Directory::Create(outputPath);
        }
        int r = archive_read_open_filename(a, filePath.c_str(), 10240);
        if (r != ARCHIVE_OK) {
            std::cerr << "Failed to open archive: " << filePath << " due to: " << archive_error_string(a) << std::endl;
            return false;
        }
        archive_entry* entry;
        r = archive_read_next_header(a, &entry);
        if (r != ARCHIVE_OK) {
            std::cerr << "Failed to open archive: " << filePath << " due to: " << archive_error_string(a) << std::endl;
            return false;
        }

        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            const char* current_file = archive_entry_pathname(entry);
            std::string current_file_path = outputPath + "/" + current_file;

            if (archive_entry_filetype(entry) == AE_IFDIR) {
                Directory::Create(current_file_path);
            }
            else {
                create_directories(std::filesystem::path(current_file_path).parent_path());
                std::ofstream ofs(current_file_path, std::ios::binary);
                if (!ofs) {
                    std::cerr << "Failed to open output file: " << current_file_path << std::endl;
                    return false;
                }

                const void* buff;
                size_t size;
                int64_t offset;
                while (archive_read_data_block(a, &buff, &size, &offset) == ARCHIVE_OK) {
                    ofs.seekp(offset);
                    ofs.write(static_cast<const char *>(buff), size);
                }

                ofs.close();
            }
        }

        return cleanup(a);
    }

    bool Utils::impl_Compress::impl_compress(archive* a, const std::string&filePath, const std::string&outputPath,
                                             int level, int split) {
        // 设置压缩级别
        if (level >= 0) {
            archive_write_set_options(a, ("compression-level=" + std::to_string(level)).c_str());
        } // 设置分卷大小（如果需要）
        if (split > 0) {
            archive_write_set_bytes_in_last_block(a, split);
        }
        // 打开输出归档文件
        if (archive_write_open_filename(a, outputPath.c_str()) != ARCHIVE_OK) {
            std::cerr << "Failed to open output archive: " << outputPath << " dut to:" << archive_error_string(a) <<
                    std::endl;
            return false;
        }

        // 添加文件或文件夹到归档
        if (!add_file_to_archive(a, filePath, filePath)) {
            archive_write_close(a);
            return false;
        }

        // 关闭归档文件
        if (archive_write_close(a) != ARCHIVE_OK) {
            std::cerr << "Failed to close output archive: " << outputPath << std::endl;
            return false;
        }

        return true;
    }

    bool Utils::impl_Compress::impl_compress(archive* a, std::vector<std::string> files, const std::string&outPath,
                                             int level, long split) {
        // 设置压缩级别
        if (level >= 0) {
            archive_write_set_options(a, ("compression-level=" + std::to_string(level)).c_str());
        } // 设置分卷大小（如果需要）
        if (split > 0) {
            archive_write_set_bytes_in_last_block(a, split);
        }
        if (archive_write_open_filename(a, outPath.c_str()) != ARCHIVE_OK) {
            std::cerr << "Failed to open output archive: " << outPath << " dut to:" << archive_error_string(a) <<
                    std::endl;
            return false;
        }
        for (const auto&file: files) {
            if (!add_file_to_archive(a, file, "./")) {
                archive_write_close(a);
                return false;
            }
        }
        if (archive_write_close(a) != ARCHIVE_OK) {
            std::cerr << "Failed to close output archive: " << outPath << std::endl;
            return false;
        }
        return true;
    }

    bool Utils::impl_Compress::impl_extractSelectedFile(archive* a, const std::string&filePath,
                                                        const std::string&selectedFile, const std::string&outputPath) {
        // 打开输入归档文件
        if (archive_read_open_filename(a, filePath.c_str(), 10240) != ARCHIVE_OK) {
            std::cerr << "Failed to open input archive: " << filePath << std::endl;
            return false;
        }

        struct archive_entry* entry;
        int r;

        // 遍历归档文件中的所有条目
        while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
            const char* currentFile = archive_entry_pathname(entry);

            // 检查当前条目是否是我们要提取的文件
            if (std::string(currentFile) == selectedFile) {
                // 创建输出目录（如果需要）
                std::string outputFilePath = outputPath + "/" + selectedFile;
                std::string outputDir = outputFilePath.substr(0, outputFilePath.find_last_of('/'));
                if (!outputDir.empty()) {
                    Directory::Create(outputDir);
                    if (!Directory::Exists(outputDir) && errno != EEXIST) {
                        std::cerr << "Failed to create output directory: " << outputDir << std::endl;
                        archive_read_close(a);
                        return false;
                    }
                }

                // 打开输出文件
                FILE* outFile = fopen(outputFilePath.c_str(), "wb");
                if (!outFile) {
                    std::cerr << "Failed to open output file: " << outputFilePath << std::endl;
                    archive_read_close(a);
                    return false;
                }

                // 读取归档数据并写入输出文件
                char buffer[8192];
                size_t size;
                while ((size = archive_read_data(a, buffer, sizeof(buffer))) > 0) {
                    if (fwrite(buffer, 1, size, outFile) != size) {
                        std::cerr << "Failed to write data to output file: " << outputFilePath << std::endl;
                        fclose(outFile);
                        archive_read_close(a);
                        return false;
                    }
                }

                // 关闭输出文件
                fclose(outFile);

                // 关闭归档文件
                if (archive_read_close(a) != ARCHIVE_OK) {
                    std::cerr << "Failed to close input archive: " << filePath << std::endl;
                    return false;
                }

                return true;
            }
        }

        // 如果遍历完所有条目都没有找到指定的文件
        if (r != ARCHIVE_EOF) {
            std::cerr << "Failed to read archive: " << filePath << std::endl;
        }
        else {
            std::cerr << "Selected file not found in archive: " << selectedFile << std::endl;
        }

        // 关闭归档文件
        archive_read_close(a);
        return false;
    }


    bool Utils::impl_Compress::add_file_to_archive(archive* a, std::string path, const std::string&basePath) {
        // 检查路径是否存在
        if (!Utils::File::Exists(path) && !Utils::Directory::Exists(path)) {
            std::cerr << "Path does not exist: " << path << std::endl;
            return false;
        }

        // 如果是目录，递归添加其内容
        if (Utils::Directory::IsDirectory(path)) {
            for (const auto&entry: Utils::Directory::List(path)) {
                if (!add_file_to_archive(a, entry, basePath)) {
                    return false;
                }
            }
        }
        else {
            // 打开输入文件
            FILE* f = fopen(path.c_str(), "rb");
            if (!f) {
                std::cerr << "Failed to open input file: " << path << std::endl;
                return false;
            }

            // 获取文件大小
            fseek(f, 0, SEEK_END);
            long file_size = ftell(f);
            fseek(f, 0, SEEK_SET);

            // 创建归档条目
            struct archive_entry* entry = archive_entry_new();
            archive_entry_set_pathname(entry, path.c_str());
            archive_entry_set_size(entry, file_size);
            archive_entry_set_filetype(entry, AE_IFREG);
            archive_entry_set_perm(entry, 0644);

            // 写入归档头
            if (archive_write_header(a, entry) != ARCHIVE_OK) {
                std::cerr << "Failed to write archive header for file: " << path << "due to: " <<
                        archive_error_string(a) << std::endl;
                archive_entry_free(entry);
                fclose(f);
                return false;
            }

            // 读取文件内容并写入归档
            char buffer[8192];
            size_t size;
            while ((size = fread(buffer, 1, sizeof(buffer), f)) > 0) {
                if (archive_write_data(a, buffer, size) != size) {
                    std::cerr << "Failed to write data to archive for file: " << path << std::endl;
                    archive_entry_free(entry);
                    fclose(f);
                    return false;
                }
            }

            // 清理
            archive_entry_free(entry);
            fclose(f);
        }

        return true;
    }

    bool Utils::impl_Compress::cleanup(archive* a) {
        if (archive_read_close(a) != ARCHIVE_OK) {
            std::cerr << "Failed to close archive: " << archive_error_string(a) << std::endl;
            return false;
        }

        if (archive_read_free(a) != ARCHIVE_OK) {
            std::cerr << "Failed to free archive: " << archive_error_string(a) << std::endl;
            return false;
        }
        return true;
    }
}
