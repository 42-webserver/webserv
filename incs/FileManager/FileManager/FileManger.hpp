#ifndef FILEMANAGER_HPP
# define FILEMANAGER_HPP

#include <iostream>
#include <Http/Response/HttpResponse.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sstream>
#include <FileManager/Cache/cache.hpp>
#include <FileManager/FileTableManager/FileTableManager.hpp>
#include <Event/EventBase/EventFactory.hpp>
#include <Event/EventDto/EventDto.hpp>
#include <Event/ReadEvent/ReadEventFromFile.hpp>

typedef enum{
    FileRequestShouldWait,
    FileRequestSuccess,
    FileRequestFail
}   e_FileRequestType;


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
public:
    static FileManager &getInstance(void);
    e_FileRequestType requstFileContent(const std::string &uri, ft::shared_ptr<HttpResponse> response);

};


#endif