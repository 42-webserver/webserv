#include <Client/ClientIdManager.hpp>

ClientIdManager *ClientIdManager::_instance = NULL;
ClientIdManager::~ClientIdManager() {}
void ClientIdManager::releaseId(int id) {
    if (id == _nextId - 1) {
        _nextId--;
    } else {
        _availableIds.insert(id);
    }
}
ClientIdManager &ClientIdManager::getInstance() {
    if (_instance == NULL) {
        _instance = new ClientIdManager();
    }
    return *_instance;
}
ClientIdManager::ClientIdManager(void) : _nextId(0) {}
int ClientIdManager::allocateId() {
    if (_nextId == INT_MAX) {
        if (_availableIds.empty()) {
            throw ClientLimitExceededException();
        } else {
            int id = *_availableIds.begin();
            
            _availableIds.erase(_availableIds.begin());
            return id;
        }
    } else if (_availableIds.empty()) {
        return _nextId++;
    } else {
        int id = *_availableIds.begin();
        _availableIds.erase(_availableIds.begin());
        return id;
    }
}
