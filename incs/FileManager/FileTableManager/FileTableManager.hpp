#ifndef FILETABLEMANAGER_HPP
# define FILETABLEMANAGER_HPP

# include <FileManager/FileState.hpp>
# include <iostream>
# include <map>
# include <Event/ReadEvent/ReadEventFromFile.hpp>
# include <Event/WriteEvent/WriteEventToFile.hpp>
# include <FileManager/FileTableManager/FileOperationController.hpp>
# include <unistd.h>
class FileManager;
class FileProcessor;
class FileTableManager
{
public:
    class AcessKey
    {
    private:
        friend class FileManager;
        friend class FileProcessor;
        AcessKey(void);
        ~AcessKey(void);
    };
private:
    std::map<std::string, ft::shared_ptr<FileOperationController> > _fileTable;
private:
    FileTableManager(void);
    ~FileTableManager(void);
    FileTableManager(const FileTableManager &ref);
    FileTableManager &operator=(const FileTableManager &rhs);
private:
    bool _fileExist(const std::string &path);
public:
    FileTableManager &getInstance(const AcessKey &acessKey);
    void deleteInstance(const AcessKey &acessKey);
    e_file_content_syncro getFileState(const std::string &path);
    bool hit(const std::string &path);
    bool syncTo(const std::string &path, ReadEventFromFile *readEventFromFile,
    e_file_content_syncro targetState);
    bool syncTo(const std::string &path, WriteEventToFile *writeEventToFile,
    e_file_content_syncro targetState);
private:
    static FileTableManager *_instance;
};
#endif