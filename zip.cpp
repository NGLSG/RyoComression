#include "zip.h"

#include <functional>

namespace RC {
    std::vector<std::string> zip::List(const std::string&filePath, const std::string&pwd) {
        unzFile zipFile = unzOpen(filePath.c_str());
        if (zipFile == nullptr) {
            std::cerr << "zip open error: " << filePath << std::endl;
        }
        if (unzGoToFirstFile(zipFile) != UNZ_OK) {
            std::cerr << "zip open error: " << filePath << std::endl;
        }
        std::vector<std::string> files;
        while (true) {
            unz_file_info64 fileInfo;
            char fileName[256];
            if (unzGetCurrentFileInfo64(zipFile, &fileInfo, fileName, sizeof(fileName), nullptr, 0, nullptr, 0) !=
                UNZ_OK) {
                std::cerr << "zip open error: " << filePath << std::endl;
            }
            if (!pwd.empty()) {
                if (tryPwd(filePath, pwd))
                    unzOpenCurrentFilePassword(zipFile, pwd.c_str());
            }
            files.emplace_back(fileName);
            int r = unzGoToNextFile(zipFile);
            if (r == UNZ_END_OF_LIST_OF_FILE) {
                break;
            }
            else if (r != UNZ_OK) {
                std::cerr << "zip open error: " << filePath << std::endl;
                break;
            }
        }
        unzClose(zipFile);
        return files;
    }

    bool zip::Extract(const std::string&filePath, const std::string&outputPath, const std::string&pwd) {
        unzFile zipFile = unzOpen(filePath.c_str());
        if (zipFile == nullptr) {
            return zipExtractMulti(filePath, outputPath, pwd);
        }

        bool result = zipExtractSingle(filePath, outputPath, pwd);
        unzClose(zipFile);
        return result;
    }

    bool zip::Compress(const std::string&sourcePath, const std::string&destPath, int compressionLevel,
                       const std::string&password, long splitSize) {
        if (splitSize >= 1024 * 1024) {
            std::string baseName = destPath.substr(0, destPath.find_last_of('.'));
            std::string extension = destPath.substr(destPath.find_last_of('.'));
            int volumeNumber = 0;
            zipFile zipFile = nullptr;
            uint64_t currentVolumeSize = 0;

            auto openNewVolume = [&]() -> bool {
                if (zipFile) {
                    zipClose(zipFile, nullptr);
                }
                std::string volumePath = baseName + "." + std::to_string(volumeNumber++) + extension;
                zipFile = zipOpen2(volumePath.c_str(), APPEND_STATUS_CREATE, nullptr, nullptr);
                if (zipFile == nullptr) {
                    std::cerr << "Failed to open output zip file: " << volumePath << std::endl;
                    return false;
                }
                currentVolumeSize = 0;
                return true;
            };

            if (!openNewVolume()) {
                return false;
            }

            if (!addPathToZip(zipFile, sourcePath, std::filesystem::path(sourcePath).filename().string(), password,
                              compressionLevel, currentVolumeSize, splitSize, openNewVolume)) {
                return false;
            }

            if (zipFile) {
                zipClose(zipFile, nullptr);
            }

            return true;
        }
        else {
            uint64_t currentVolumeSize = 0; // Initialize currentVolumeSize for single file compression
            auto openNewVolume = []() -> bool { return true; };
            // Define an empty openNewVolume function for single file compression
            zipFile zipFile = zipOpen(destPath.c_str(), APPEND_STATUS_CREATE);
            if (zipFile == nullptr) {
                std::cerr << "Failed to open output zip file: " << destPath << std::endl;
                return false;
            }

            if (!addPathToZip(zipFile, sourcePath, "", password, compressionLevel, currentVolumeSize, splitSize,
                              openNewVolume)) {
                zipClose(zipFile, nullptr);
                return false;
            }

            zipClose(zipFile, nullptr);
            return true;
        }
    }

    bool zip::ExtractSelectedFile(const std::string&filePath, const std::string&selectedFile,
                                  const std::string&outputPath,
                                  const std::string&pwd) {
        unzFile zipFile = unzOpen(filePath.c_str());
        if (zipFile == nullptr) {
            std::cerr << "Failed to open zip file: " << filePath << std::endl;
            return false;
        }

        if (unzGoToFirstFile(zipFile) != UNZ_OK) {
            std::cerr << "Failed to go to first file in zip" << std::endl;
            unzClose(zipFile);
            return false;
        }

        bool found = false;
        do {
            unz_file_info64 fileInfo;
            char fileName[256];
            if (unzGetCurrentFileInfo64(zipFile, &fileInfo, fileName, sizeof(fileName), nullptr, 0, nullptr, 0) !=
                UNZ_OK) {
                std::cerr << "Failed to get current file info" << std::endl;
                unzClose(zipFile);
                return false;
            }

            if (std::string(fileName) == selectedFile) {
                found = true;
                break;
            }
        }
        while (unzGoToNextFile(zipFile) == UNZ_OK);

        if (!found) {
            std::cerr << "Selected file not found in zip: " << selectedFile << std::endl;
            unzClose(zipFile);
            return false;
        }

        std::string fullFilePath = outputPath + "/" + selectedFile;
        std::string directoryPath = fullFilePath.substr(0, fullFilePath.find_last_of('/'));
        if (!Utils::Directory::Exists(directoryPath)) {
            if (Utils::Directory::Create(directoryPath)) {
                if (!Utils::Directory::Exists(directoryPath)) {
                    std::cerr << "Failed to create directory: " << directoryPath << std::endl;
                    unzClose(zipFile);
                    return false;
                }
            }
        }

        if (unzOpenCurrentFilePassword(zipFile, pwd.c_str()) != UNZ_OK) {
            std::cerr << "Failed to open current file with pwd: " << selectedFile << std::endl;
            unzClose(zipFile);
            return false;
        }

        std::ofstream outputFile(fullFilePath, std::ios::binary);
        if (!outputFile.is_open()) {
            std::cerr << "Failed to open output file: " << fullFilePath << std::endl;
            unzCloseCurrentFile(zipFile);
            unzClose(zipFile);
            return false;
        }

        char buffer[4096];
        int bytesRead;
        while ((bytesRead = unzReadCurrentFile(zipFile, buffer, sizeof(buffer))) > 0) {
            outputFile.write(buffer, bytesRead);
        }

        outputFile.close();
        unzCloseCurrentFile(zipFile);
        unzClose(zipFile);

        return true;
    }

    bool zip::MergeParts(const std::string&outputZipPath, const std::string&destPath, const std::string&pwd) {
        std::string extension = ".zip";
        std::string baseName = outputZipPath.substr(0, outputZipPath.find_last_of('.'));
        int volumeNumber = 0;
        zipFile outputZipFile = zipOpen(outputZipPath.c_str(), APPEND_STATUS_CREATE);
        if (outputZipFile == nullptr) {
            std::cerr << "Failed to open output zip file: " << outputZipPath << std::endl;
            return false;
        }

        bool continueMerging = true;
        while (continueMerging) {
            std::string volumePath = baseName + "." + std::to_string(volumeNumber++) + extension;
            unzFile inputZipFile = unzOpen(volumePath.c_str());
            if (inputZipFile == nullptr) {
                if (volumeNumber == 1) {
                    std::cerr << "Failed to open input zip file: " << volumePath << std::endl;
                    zipClose(outputZipFile, nullptr);
                    return false;
                }
                continueMerging = false;
                break;
            }

            if (unzGoToFirstFile(inputZipFile) != UNZ_OK) {
                std::cerr << "Failed to go to first file in zip: " << volumePath << std::endl;
                unzClose(inputZipFile);
                zipClose(outputZipFile, nullptr);
                return false;
            }

            do {
                unz_file_info64 fileInfo;
                char fileName[256];
                if (unzGetCurrentFileInfo64(inputZipFile, &fileInfo, fileName, sizeof(fileName), nullptr, 0,
                                            nullptr, 0) != UNZ_OK) {
                    std::cerr << "Failed to get current file info in zip: " << volumePath << std::endl;
                    unzClose(inputZipFile);
                    zipClose(outputZipFile, nullptr);
                    return false;
                }

                if (unzOpenCurrentFilePassword(inputZipFile, pwd.c_str()) != UNZ_OK) {
                    std::cerr << "Failed to open current file with password in zip: " << fileName << std::endl;
                    unzClose(inputZipFile);
                    zipClose(outputZipFile, nullptr);
                    return false;
                }

                zip_fileinfo zipInfo = {};
                if (zipOpenNewFileInZip3(outputZipFile, fileName, &zipInfo, nullptr, 0, nullptr, 0, nullptr,
                                         Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0, -MAX_WBITS, DEF_MEM_LEVEL,
                                         Z_DEFAULT_STRATEGY, pwd.c_str(), 0) != ZIP_OK) {
                    std::cerr << "Failed to open new file in output zip: " << fileName << std::endl;
                    unzCloseCurrentFile(inputZipFile);
                    unzClose(inputZipFile);
                    zipClose(outputZipFile, nullptr);
                    return false;
                }

                char buffer[4096];
                int bytesRead;
                while ((bytesRead = unzReadCurrentFile(inputZipFile, buffer, sizeof(buffer))) > 0) {
                    if (zipWriteInFileInZip(outputZipFile, buffer, bytesRead) != ZIP_OK) {
                        std::cerr << "Failed to write to output zip file: " << fileName << std::endl;
                        unzCloseCurrentFile(inputZipFile);
                        unzClose(inputZipFile);
                        zipCloseFileInZip(outputZipFile);
                        zipClose(outputZipFile, nullptr);
                        return false;
                    }
                }

                unzCloseCurrentFile(inputZipFile);
                zipCloseFileInZip(outputZipFile);
            }
            while (unzGoToNextFile(inputZipFile) == UNZ_OK);

            unzClose(inputZipFile);
        }

        zipClose(outputZipFile, nullptr);
        if (!destPath.empty()) {
            Utils::Directory::Create(Utils::File::GetParentDirectory(destPath));
            Utils::File::Move(outputZipPath, destPath);
        }
        return true;
    }


    bool zip::tryPwd(const std::string&filePath, const std::string&pwd) {
        unzFile zipFile = unzOpen(filePath.c_str());
        if (zipFile == nullptr) {
            std::cerr << "zip open error: " << filePath << std::endl;
        }
        if (unzGoToFirstFile(zipFile) != UNZ_OK) {
            std::cerr << "zip open error: " << filePath << std::endl;
        }
        unz_file_info64 fileInfo;
        char fileName[256];
        if (unzGetCurrentFileInfo64(zipFile, &fileInfo, fileName, sizeof(fileName), nullptr, 0, nullptr, 0) !=
            UNZ_OK) {
            std::cerr << "zip open error: " << filePath << std::endl;
        }
        if (unzOpenCurrentFilePassword(zipFile, pwd.c_str()) == UNZ_OK) {
            unzCloseCurrentFile(zipFile);
            unzClose(zipFile);
            return true;
        }
        std::cerr << "zip open error: Incorrect password! " << std::endl;
        return false;
    }

    bool zip::zipExtractSingle(const std::string&filePath, const std::string&outputPath, const std::string&pwd) {
        unzFile zipFile = unzOpen(filePath.c_str());

        if (unzGoToFirstFile(zipFile) != UNZ_OK) {
            std::cerr << "zip open error: " << filePath << std::endl;
            unzClose(zipFile);
            return false;
        }

        while (true) {
            unz_file_info64 fileInfo;
            char fileName[256];
            if (unzGetCurrentFileInfo64(zipFile, &fileInfo, fileName, sizeof(fileName), nullptr, 0, nullptr, 0) !=
                UNZ_OK) {
                std::cerr << "zip open error: " << filePath << std::endl;
                unzClose(zipFile);
                return false;
            }

            if (!pwd.empty()) {
                if (tryPwd(filePath, pwd)) {
                    if (unzOpenCurrentFilePassword(zipFile, pwd.c_str()) != UNZ_OK) {
                        std::cerr << "Failed to open current file with pwd: " << fileName << std::endl;
                        unzClose(zipFile);
                        return false;
                    }
                }
            }
            else {
                if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
                    std::cerr << "Failed to open current file: " << fileName << std::endl;
                    unzClose(zipFile);
                    return false;
                }
            }

            std::string outputFilePath = outputPath + "/" + fileName;
            if (Utils::Directory::IsDirectory(outputFilePath)) {
                if (!Utils::Directory::Exists(outputFilePath)) {
                    Utils::Directory::Create(outputFilePath);
                    if (!Utils::Directory::Exists(outputFilePath)) {
                        std::cerr << "Failed to create directory: " << outputFilePath << std::endl;
                        unzCloseCurrentFile(zipFile);
                        unzClose(zipFile);
                        return false;
                    }
                }
            }
            else {
                std::ofstream outputFile(outputFilePath, std::ios::binary);
                if (!outputFile.is_open()) {
                    std::cerr << "Failed to open output file: " << outputFilePath << std::endl;
                    unzCloseCurrentFile(zipFile);
                    unzClose(zipFile);
                    return false;
                }

                char buffer[4096];
                int readSize = 0;
                while ((readSize = unzReadCurrentFile(zipFile, buffer, sizeof(buffer))) > 0) {
                    outputFile.write(buffer, readSize);
                }

                outputFile.close();
            }

            int r = unzGoToNextFile(zipFile);
            if (r == UNZ_END_OF_LIST_OF_FILE) {
                unzClose(zipFile);
                return true;
            }
            else if (r != UNZ_OK) {
                std::cerr << "zip open error: " << filePath << std::endl;
                unzCloseCurrentFile(zipFile);
                unzClose(zipFile);
                return false;
            }

            unzCloseCurrentFile(zipFile);
        }

        return false;
    }

    bool zip::zipExtractMulti(const std::string&zipPath, const std::string&outputPath, const std::string&pwd) {
        std::string baseName = zipPath.substr(0, zipPath.find_last_of('.'));
        std::string extension = zipPath.substr(zipPath.find_last_of('.'));
        int volumeNumber = 0;
        unzFile zipFile = nullptr;

        auto openNextVolume = [&]() -> bool {
            if (zipFile) {
                unzClose(zipFile);
            }
            std::string volumePath = baseName + "." + std::to_string(volumeNumber++) + extension;
            zipFile = unzOpen(volumePath.c_str());
            if (zipFile == nullptr) {
                std::cerr << "Failed to open zip file: " << volumePath << std::endl;
                return false;
            }
            return true;
        };

        if (!openNextVolume()) {
            return false;
        }

        if (unzGoToFirstFile(zipFile) != UNZ_OK) {
            std::cerr << "Failed to go to first file in zip" << std::endl;
            unzClose(zipFile);
            return false;
        }

        do {
            unz_file_info64 fileInfo;
            char fileName[256];
            if (unzGetCurrentFileInfo64(zipFile, &fileInfo, fileName, sizeof(fileName), nullptr, 0, nullptr, 0) !=
                UNZ_OK) {
                std::cerr << "Failed to get current file info" << std::endl;
                unzClose(zipFile);
                return false;
            }

            std::string fullFilePath = outputPath + "/" + fileName;

            // Check if the current file is a directory
            if (Utils::Directory::IsDirectory(fullFilePath)) {
                if (!Utils::Directory::Exists(fullFilePath)) {
                    Utils::Directory::Create(fullFilePath);
                    if (!Utils::Directory::Exists(fullFilePath)) {
                        std::cerr << "Failed to create directory: " << fullFilePath << std::endl;
                        unzCloseCurrentFile(zipFile);
                        unzClose(zipFile);
                        return false;
                    }
                }
            }
            else {
                if (unzOpenCurrentFilePassword(zipFile, pwd.c_str()) != UNZ_OK) {
                    std::cerr << "Failed to open current file with pwd: " << fileName << std::endl;
                    unzClose(zipFile);
                    return false;
                }

                std::ofstream outputFile(fullFilePath, std::ios::binary);
                if (!outputFile.is_open()) {
                    std::cerr << "Failed to open output file: " << fullFilePath << std::endl;
                    unzCloseCurrentFile(zipFile);
                    unzClose(zipFile);
                    return false;
                }

                char buffer[4096];
                int bytesRead;
                while ((bytesRead = unzReadCurrentFile(zipFile, buffer, sizeof(buffer))) > 0) {
                    outputFile.write(buffer, bytesRead);
                }

                outputFile.close();
            }


            int r = unzGoToNextFile(zipFile);
            if (r == UNZ_END_OF_LIST_OF_FILE) {
                if (!openNextVolume()) {
                    unzClose(zipFile);
                    return false;
                }
                if (unzGoToFirstFile(zipFile) != UNZ_OK) {
                    std::cerr << "Failed to go to first file in next volume" << std::endl;
                    unzClose(zipFile);
                    return false;
                }
                return true;
            }
            else if (r != UNZ_OK) {
                std::cerr << "Failed to go to next file in zip" << std::endl;
                unzClose(zipFile);
                return false;
            }
            unzCloseCurrentFile(zipFile);
        }
        while (true);

        if (zipFile) {
            unzClose(zipFile);
        }

        return true;
    }

    bool zip::addFileToZip(zipFile zipFile, const std::string&filePath, const std::string&relativePath,
                           const std::string&password, int compressionLevel, uint64_t&currentVolumeSize, long splitSize,
                           std::function<bool()> openNewVolume) {
        std::ifstream inputFile(filePath, std::ios::binary);
        if (!inputFile.is_open()) {
            std::cerr << "Failed to open input file: " << filePath << std::endl;
            return false;
        }

        inputFile.seekg(0, std::ios::end);
        uint64_t fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);

        while (fileSize > 0) {
            uint64_t remainingSize = splitSize - currentVolumeSize;
            uint64_t bytesToWrite = min(fileSize, remainingSize);

            zip_fileinfo fileInfo = {};
            if (zipOpenNewFileInZip3(zipFile, relativePath.c_str(), &fileInfo, nullptr, 0, nullptr, 0, nullptr,
                                     Z_DEFLATED, compressionLevel, 0, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                                     password.c_str(), 0) != ZIP_OK) {
                std::cerr << "Failed to open new file in zip: " << relativePath << std::endl;
                inputFile.close();
                return false;
            }

            char* buffer = new char[bytesToWrite];
            inputFile.read(buffer, bytesToWrite);
            std::streamsize bytesRead = inputFile.gcount();
            if (bytesRead > 0) {
                if (zipWriteInFileInZip(zipFile, buffer, static_cast<unsigned int>(bytesRead)) != ZIP_OK) {
                    std::cerr << "Failed to write to zip file: " << relativePath << std::endl;
                    zipCloseFileInZip(zipFile);
                    inputFile.close();
                    delete[] buffer;
                    return false;
                }
                currentVolumeSize += bytesRead;
            }
            delete[] buffer;

            zipCloseFileInZip(zipFile);

            if (currentVolumeSize >= splitSize) {
                if (!openNewVolume()) {
                    inputFile.close();
                    return false;
                }
            }

            fileSize -= bytesRead;
        }

        inputFile.close();
        return true;
    }

    bool zip::addDirectoryToZip(zipFile zipFile, const std::string&directoryPath, const std::string&relativePath,
                                const std::string&password, int compressionLevel) {
        zip_fileinfo fileInfo = {};
        if (zipOpenNewFileInZip3(zipFile, (relativePath + "/").c_str(), &fileInfo, nullptr, 0, nullptr, 0, nullptr,
                                 Z_DEFLATED, compressionLevel, 0, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                                 password.c_str(), 0) != ZIP_OK) {
            std::cerr << "Failed to open new directory in zip: " << relativePath << std::endl;
            return false;
        }
        zipCloseFileInZip(zipFile);
        return true;
    }

    bool zip::addPathToZip(zipFile zipFile, const std::string&path, const std::string&relativePath,
                           const std::string&password, int compressionLevel, uint64_t&currentVolumeSize, long splitSize,
                           std::function<bool()> openNewVolume) {
        if (std::filesystem::is_directory(path)) {
            if (!addDirectoryToZip(zipFile, path, relativePath, password, compressionLevel)) {
                return false;
            }
            for (const auto&entry: std::filesystem::directory_iterator(path)) {
                std::string newRelativePath = relativePath.empty()
                                                  ? entry.path().filename().string()
                                                  : relativePath + "/" + entry.path().filename().string();
                if (!addPathToZip(zipFile, entry.path().string(), newRelativePath, password, compressionLevel,
                                  currentVolumeSize, splitSize, openNewVolume)) {
                    return false;
                }
            }
        }
        else {
            if (!addFileToZip(zipFile, path, relativePath, password, compressionLevel, currentVolumeSize, splitSize,
                              openNewVolume)) {
                return false;
            }
        }
        return true;
    }
}
