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


e_file_msg FileOperationController::_invalidArgument(void) {
    return (InvalidArgument);
}

e_file_msg FileOperationController::_loadIsOk(ft::shared_ptr<Client_id> currentClientId) {
    this->_fileBufferManager.registerClientWaitingForFile(currentClientId);
    return (OkGood);
}


e_file_msg FileOperationController::loadFileContent(ft::shared_ptr<Client_id> currentClientId) {
    e_file_content_syncro fileState = this->_fileIdent.getState();

    switch (fileState) {
        case e_file_content_syncro::Init:
            return (InvalidArgument);
        case e_file_content_syncro::InCache:
            return (InvalidArgument);
        case e_file_content_syncro::InsertingCache:
            return (InvalidArgument);
        case e_file_content_syncro::UpdatingCache:
            return (InvalidArgument);
        case e_file_content_syncro::ReadingFile: {
            return (this->_loadIsOk(currentClientId));
        }
        case e_file_content_syncro::WritingFile: {
            return (this->_loadIsOk(currentClientId));
        }
        case e_file_content_syncro::InBuffer: {
            return (this->_loadIsOk(currentClientId));
        }
        case e_file_content_syncro::BufferFlushed: {
            return (InvalidArgument);
        }
        case e_file_content_syncro::eraseProcessing: {
            return (InvalidArgument);
        }
        case e_file_content_syncro::sharedProcessing: {
            return (InvalidArgument);
        }
        case e_file_content_syncro::deleted: {
            return (deletedFile);
        }
    }
}

