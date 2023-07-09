#include "../../../incs/Http/Utils/FileUploader.hpp"

const char* FileUploader::FileUploadException::what() const throw() { return ("File upload failed"); }

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
        throw FileUploader::FileUploadException();
}