#include <FileManager/FileTableManager/FileTableManager.hpp>

FileTableManager::AcessKey::AcessKey(void) {}
FileTableManager::AcessKey::~AcessKey(void) {}
FileTableManager *FileTableManager::_instance = NULL;
FileTableManager::FileTableManager(void) {
}
FileTableManager::~FileTableManager(void) {}
FileTableManager& FileTableManager::getInstance(const AcessKey &acessKey) {
    (void)acessKey;
    if (FileTableManager::_instance == NULL) {
        FileTableManager::_instance = new FileTableManager();
    }
    return (*FileTableManager::_instance);
}

bool FileTableManager::_fileExist(const std::string &path) {
    return (access(path.c_str(), F_OK) != -1);
}
void FileTableManager::deleteInstance(const AcessKey &acessKey) {
    (void)acessKey;
    if (FileTableManager::_instance != NULL) {
        delete FileTableManager::_instance;
        FileTableManager::_instance = NULL;
    }
}

e_file_content_syncro FileTableManager::getFileState(const std::string &path) {
    if (this->_fileTable.find(path) == this->_fileTable.end()) {
        this->_fileTable[path] = ft::shared_ptr<FileOperationController>
        (new FileOperationController(path));
    }
    return (this->_fileTable[path]->getFileState());
}

bool FileTableManager::hit(const std::string &path) {
    if (this->_fileTable.find(path) == this->_fileTable.end()) {
        if (this->_fileExist(path) == false) {
            return (false);
        }
    }
    return (true);
}

bool FileTableManager::syncTo(const std::string &path, ReadEventFromFile *readEventFromFile,
e_file_content_syncro targetState) {
    if (this->_fileTable.find(path) == this->_fileTable.end()) {
        return (false);
    }
    this->_fileTable[path]->syncTo(readEventFromFile, targetState);
    return (true);
}

bool FileTableManager::syncTo(const std::string &path, WriteEventToFile *writeEventToFile,
e_file_content_syncro targetState) {
    if (this->_fileTable.find(path) == this->_fileTable.end()) {
        return (false);
    }
    this->_fileTable[path]->syncTo(writeEventToFile, targetState);
    return (true);
}