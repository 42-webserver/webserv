#include <FileManager/FileTableManager/FileTableManager.hpp>

CurrentSharedWriteClient::CurrentSharedWriteClient(void) :
_currentWritePosition(0), _client_id() {}
CurrentSharedWriteClient::CurrentSharedWriteClient(const ft::shared_ptr<Client_id> &client_id) :
_currentWritePosition(0), _client_id(client_id) {}
CurrentSharedWriteClient::~CurrentSharedWriteClient(void) {}
CurrentSharedWriteClient::CurrentSharedWriteClient(const CurrentSharedWriteClient &ref) :
_currentWritePosition(ref._currentWritePosition), _client_id(ref._client_id) {}
CurrentSharedWriteClient &CurrentSharedWriteClient::operator=(const CurrentSharedWriteClient &rhs) {
    if (this != &rhs) {
        this->_currentWritePosition = rhs._currentWritePosition;
        this->_client_id = rhs._client_id;
    }
    return (*this);
}

void CurrentSharedWriteClient::setCurrentWritePosition(size_t currentWritePosition) {
    this->_currentWritePosition = currentWritePosition;
}

size_t CurrentSharedWriteClient::getCurrentWritePosition(void) const {
    return (this->_currentWritePosition);
}

bool CurrentSharedWriteClient::isAlive(void) {
    return (this->_client_id->isAvailable());
}

bool CurrentSharedWriteClient::operator==(const ft::shared_ptr<Client_id> &client_id) const {
    return (*this->_client_id == *client_id);
}



///////////
FileBufferManager::FileBufferManager(void) :
_clientWaitingQueue(std::queue<ft::shared_ptr<Client_id> >()),
_readFromFileToClientBuffer(ft::shared_ptr<IoReadAndWriteBuffer>(new IoReadAndWriteBuffer())) {}

FileBufferManager::~FileBufferManager(void) {}

void FileBufferManager::registerClientWaitingForFile(ft::shared_ptr<Client_id> &client_id) {
    this->_clientWaitingQueue.push(client_id);
}

bool FileBufferManager::isClientRegistered(const ft::shared_ptr<Client_id> &client_id) {
    clientQueueIterator it = this->_clientWaitingQueueTable.find(client_id->getId());
}
e_file_msg FileBufferManager::ioWriteUsingSharedBuffer(const ft::shared_ptr<Client_id> &client_id) {
    CurrentSharedWriteClient &currentWriteClient = this->_currentWriteClients[client_id->getId()];
    return (deletedFile);
}