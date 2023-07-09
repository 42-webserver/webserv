#ifndef FILEUPLOADER_HPP
#define FILEUPLOADER_HPP

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <vector>
#include "../Exception/BadRequestException.hpp"

class FileUploader
{
public:
class FileUploadException : public BadRequestException {
    public:
            virtual const char* what() const throw();
};

static void fileUpload(std::vector<char> &reqBuffer, std::string path) throw(FileUploader::FileUploadException);
static void checkFileExists(const std::string& filepath) throw(FileUploader::FileUploadException);
};

#endif