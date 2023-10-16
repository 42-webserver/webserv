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
    return (OkNext);
}


e_file_msg FileOperationController::loadFileContent(ft::shared_ptr<Client_id> currentClientId) {
    // e_file_content_syncro fileState = this->_fileIdent.getState();
    const e_client_file_action clientAction = currentClientId->getfileAction();

    switch (clientAction) {
        case ActionRead:
            break;
        default:
            return (this->_invalidArgument());
    }
    if (!this->_fileBufferManager.isClientRegistered(currentClientId))
        this->_fileBufferManager.registerClientWaitingForFile(currentClientId);
    //Todo
    return (OkNext);
}

e_file_msg FileOperationController::sendFileContent(ft::shared_ptr<Client_id> currentClientId) {
    (void)currentClientId;
    //TODO: send file content
    return (OkNext);
}

e_file_msg FileOperationController::deleteFileContent(ft::shared_ptr<Client_id> currentClientId) {
    (void)currentClientId;
    //TODO: delete file content
    return (OkNext);
}

e_file_msg FileOperationController::updateFileContent(ft::shared_ptr<Client_id> currentClientId) {
    (void)currentClientId;
    //TODO: update file content
    return (OkNext);
}

