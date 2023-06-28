#include "../../../incs/http/utils/FileUploader.hpp"

const char* FileUploader::FileUploadException::what() const throw(){
    return "File upload failed";
}

void FileUploader::fileUpload(std::vector<MultipartRequest> &multipartRequests) throw(FileUploader::FileUploadException){
    for (std::vector<MultipartRequest>::iterator it = multipartRequests.begin(); it != multipartRequests.end(); it++){
        std::multimap<std::string, std::string> _headers = it->getHeaders();
        std::multimap<std::string, std::string>::iterator mapIt = _headers.find("Content-Disposition");
        if (mapIt == _headers.end())
            throw FileUploader::FileUploadException();
        while(mapIt != _headers.end()){
            if (mapIt->second.find("filename=") == std::string::npos){
                mapIt++;
                continue;
            }
            std::string filename = mapIt->second.substr(mapIt->second.find("filename=") + 9);
            filename = std::string(filename.begin() + 1, filename.end() - 1);
            if (filename.empty())
                throw FileUploader::FileUploadException();
            std::string filepath = _FILE_UPLOAD_DIR + filename;
            uploadStart(filepath, it->getMemoryBody(), it->getBodyDataFilename());
            break;
        }
    }
}

void FileUploader::uploadStart(std::string &filepath, std::vector<char> &memoryBody, std::string &bodyDataFilename) throw(FileUploadException){
    checkFileExists(filepath);
    if (memoryBody.empty()){
        if (std::rename(bodyDataFilename.c_str(), filepath.c_str()) != 0)
            throw std::runtime_error("Error: can't rename file on FileUploader::uploadStart");
    }else if (!memoryBody.empty()){
        std::ofstream file(filepath.c_str(), std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Error: can't open file");
        }
        file.write(&memoryBody[0], memoryBody.size());
        file.close();
    } else
        throw FileUploader::FileUploadException();
}

void FileUploader::checkFileExists(const std::string& filepath) throw(FileUploader::FileUploadException){
    std::ifstream file(filepath.c_str());
    if(file.good())
        throw FileUploader::FileUploadException();
}