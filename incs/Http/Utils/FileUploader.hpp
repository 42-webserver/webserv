#ifndef FILEUPLOADER_HPP
#define FILEUPLOADER_HPP

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <vector>
#include "../Exception/BadRequestException.hpp"
#include "../Exception/ServerErrorException.hpp"
#include <sys/stat.h>

class FileUploader
{

public:
class FileUploadException : public BadRequestException {
    private:
            const char *_message;
    public:
            virtual const char* what() const throw();
            FileUploadException(const char *message);
};

static void fileUpload(std::vector<char> &reqBuffer, std::string path) throw(FileUploader::FileUploadException);
static void checkFileExists(const std::string& filepath) throw(FileUploader::FileUploadException);
static bool isFileExists(const std::string& filename);
static bool isDirectory(const std::string& filename);
static long getFileSize(const std::string& filename);
};

#endif