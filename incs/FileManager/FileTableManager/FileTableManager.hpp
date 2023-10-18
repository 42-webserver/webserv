#ifndef FILETABLEMANAGER_HPP
# define FILETABLEMANAGER_HPP

#include <iostream>
#include <map>
#include <FileManager/FileManager/FileData.hpp>

class FileManager;
class FileTableManager
{
public:
    class Accesskey
    {
        friend class FileManager;
        private:
        Accesskey(void);
        ~Accesskey(void);
    };
private:
    std::map<std::string, FileData> _fileTable;
public:
    e_FileProcessingType findFileProcessingType(const std::string &fileName);
    FileData& getFileData(const std::string &fileName);
    void deleteFileData(const std::string &fileName);
private:
    FileTableManager(void);
    FileTableManager(const FileTableManager &ref);
    FileTableManager &operator=(const FileTableManager &rhs);
    ~FileTableManager(void);
public:
    static FileTableManager &getInstance(Accesskey);

private:
    static FileTableManager *_instance;
};

#endif