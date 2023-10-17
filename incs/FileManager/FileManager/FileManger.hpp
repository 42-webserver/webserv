#ifndef FILEMANAGER_HPP
# define FILEMANAGER_HPP

#include <iostream>
#include <Http/Response/HttpResponse.hpp>
#include <Http/Request/HttpRequest.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sstream>
#include <FileManager/Cache/cache.hpp>
#include <FileManager/FileTableManager/FileTableManager.hpp>
#include <Event/EventBase/EventFactory.hpp>
#include <Event/EventDto/EventDto.hpp>
#include <Event/ReadEvent/ReadEventFromFile.hpp>
#include <Event/WriteEvent/WriteEventToFile.hpp>
#include <FileManager/Exception/FileNotExistException.hpp>
#include <FileManager/Exception/FileIsDirectoryException.hpp>
typedef enum{
    FileRequestShouldWait,
    FileRequestSuccess,
    FileRequestFail
}   e_FileRequestType;

typedef enum{
    NotExistFile,
    ExistFile,
    ExistDirectory
}   e_file_info;
class FileManager
{
private:
    FileManager(void);
    FileManager(const FileManager &ref);
    FileManager &operator=(const FileManager &rhs);
    ~FileManager(void);
private:
    static FileManager *_instance;
    void _readFile(const std::string &uri, ft::shared_ptr<HttpResponse> response);
    void _writeFile(const std::string &uri, ft::shared_ptr<HttpRequest> request);
public:
    static FileManager &getInstance(void);
    e_FileRequestType requstFileContent(const std::string &uri, ft::shared_ptr<HttpResponse> response);
    e_FileRequestType requestFileUpload(const std::string &uri, 
    ft::shared_ptr<HttpRequest> request);
    e_FileRequestType _requestFileUploadDefault(const std::string &uri,
    ft::shared_ptr<HttpRequest> request);
    e_FileRequestType _requestFileUploadMultiPart(const std::string &uri,
    ft::shared_ptr<HttpRequest> request);

//static utility
public:
    static std::string getDirectoryListing(const std::string& path);
    static e_file_info getFileInfo(const std::string &uri, struct stat &fileStat);
    static bool isInCashSize(struct stat &fileStat);
    static bool isInCashSize(size_t size);
    static size_t getFileSize(const std::string &uri);
};


#endif