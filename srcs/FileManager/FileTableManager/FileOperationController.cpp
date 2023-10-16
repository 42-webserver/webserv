#include <FileManager/FileTableManager/FileOperationController.hpp>

FileOperationController::FileOperationController(std::string path) :
_fileBufferManager(),
_fileIdent(path) {}

FileOperationController::~FileOperationController(void) {}

e_file_content_syncro FileOperationController::getFileState(void) {
    return (this->_fileIdent.getState());
}

void FileOperationController::syncTo(ReadEventFromFile *readEventFromFile,
e_file_content_syncro targetState) {
        this->_fileIdent.syncTo(readEventFromFile, targetState);
}

void FileOperationController::syncTo(WriteEventToFile *writeEventToFile,
e_file_content_syncro targetState) {
        this->_fileIdent.syncTo(writeEventToFile, targetState);
}

long FileOperationController::getFileSize(void) {
    return (this->_fileIdent.getFileSize());
}