#ifndef FILETABLEMANAGER_HPP
# define FILETABLEMANAGER_HPP

# include <FileManager/FileTableManager/FileIdent.hpp>
# include <FileManager/FileState.hpp>
# include <iostream>
# include <map>
# include <Event/ReadEvent/ReadEventFromFile.hpp>
# include <Event/WriteEvent/WriteEventToFile.hpp>

class FileTableManager
{
private:
    std::map<std::string, FileIdent> _fileTable;
private:
    FileTableManager(void);
    ~FileTableManager(void);
    FileTableManager(const FileTableManager &ref);
    FileTableManager &operator=(const FileTableManager &rhs);
public:
    FileTableManager &getInstance(void);
    void deleteInstance(void);
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