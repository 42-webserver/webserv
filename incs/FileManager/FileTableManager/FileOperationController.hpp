#ifndef FILEOPERATIONCONTROLLER_HPP
# define FILEOPERATIONCONTROLLER_HPP

# include <FileManager/FileTableManager/FileBufferManager.hpp>
# include <FileManager/FileTableManager/FileIdent.hpp>  

class FileOperationController
{
private:
    FileBufferManager _fileBufferManager;
    FileIdent _fileIdent;
private:
    FileOperationController(void);
    FileOperationController(const FileOperationController &ref);
    FileOperationController &operator=(const FileOperationController &rhs);
public:
    FileOperationController(std::string path);
    ~FileOperationController(void);
//fileIdent
public:
    e_file_content_syncro getFileState(void);
    long getFileSize(void);
    void syncTo(ReadEventFromFile *readEventFromFile, e_file_content_syncro targetState);
    void syncTo(WriteEventToFile *writeEventToFile, e_file_content_syncro targetState);
//fileBufferManager
public:
};
#endif