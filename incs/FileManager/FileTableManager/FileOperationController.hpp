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
public:
    e_file_msg loadFileContent(ft::shared_ptr<Client_id> currentClientId);
    e_file_msg sendFileContent(ft::shared_ptr<Client_id> currentClientId);
    e_file_msg deleteFileContent(ft::shared_ptr<Client_id> currentClientId);
    e_file_msg updateFileContent(ft::shared_ptr<Client_id> currentClientId);
private:
    e_file_msg _invalidArgument(void);
    e_file_msg _loadIsOk(ft::shared_ptr<Client_id> currentClientId);
};
#endif