#ifndef FILEUPLOADER_HPP
#define FILEUPLOADER_HPP

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include "../request/MultipartRequest.hpp"
#include "../exception/BadRequestException.hpp"

static const std::string        _FILE_UPLOAD_DIR = "/Users/leehyunkyu/Desktop/webserv/uploads/";

class FileUploader
{
public:
class FileUploadException : public BadRequestException {
    public:
            virtual const char* what() const throw();
};

static void fileUpload(std::vector<MultipartRequest> &multipartRequests) throw(FileUploader::FileUploadException);
static void uploadStart(std::string &filepath, std::vector<char> &memoryBody, std::string &bodyDataFilename) throw(FileUploadException);
static void checkFileExists(const std::string& filepath) throw(FileUploader::FileUploadException);
};

#endif