#include "../../../incs/Http/Utils/FileUploader.hpp"

FileUploader::FileUploadException::FileUploadException(const char *message) :
    BadRequestException(BAD_REQUEST), _message(message) {}

const char* FileUploader::FileUploadException::what() const throw() { return _message; }

void FileUploader::fileUpload(std::vector<char> &reqBuffer, std::string path) throw(FileUploader::FileUploadException){
   std::ofstream file(path, std::ios::app);
	if (!file.is_open())
		throw std::runtime_error("Error: can't open file");
	file.write(reqBuffer.data(), reqBuffer.size());
	file.close();
}

void FileUploader::checkFileExists(const std::string& filepath) throw(FileUploader::FileUploadException){
    std::ifstream file(filepath.c_str());
    if(file.good())
        throw FileUploader::FileUploadException("Duplicate File Error");
}

bool FileUploader::isFileExists(const std::string& filename){
    std::ifstream file(filename.c_str());
    return file.good();
}

long FileUploader::getFileSize(const std::string& filename) {
    struct stat fileStat;
    if (stat(filename.c_str(), &fileStat) == 0) {
        return fileStat.st_size;
    }
    throw ServerErrorException(INTERNAL_SERVER_ERROR);
}