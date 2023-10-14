#include <Client/ClientIdManager.hpp>


Client_id::Client_id(clinet_id_t id, bool isAvailable, 
e_client_file_action fileAction) : _id(id), _isAvailable(isAvailable), _fileAction(fileAction) {}
Client_id::~Client_id() {}
clinet_id_t Client_id::getId() const {
    return _id;
}
bool Client_id::isAvailable() const {
    return _isAvailable;
}
e_client_file_action Client_id::getfileAction() const {
    return _fileAction;
}
void Client_id::release() {
    _isAvailable = false;
}
ClientIdManager *ClientIdManager::_instance = NULL;
ClientIdManager::~ClientIdManager() {}
ClientIdManager::AcessKey::AcessKey(void) {}
ClientIdManager::AcessKey::~AcessKey(void) {}
ClientIdManager &ClientIdManager::getInstance(const AcessKey &acessKey) {
    (void)acessKey;
    if (_instance == NULL) {
        _instance = new ClientIdManager();
    }
    return *_instance;
}
ClientIdManager::ClientIdManager(void) : _nextId(0) {}

// //check this
// ft::shared_ptr<Client_id> ClientIdManager::allocateId() {
//     if (_nextId == INT_MAX) {
//         if (_availableIds.empty()) {
//             throw ClientLimitExceededException();
//         } else {
//             clinet_id_t id = *_availableIds.begin();
            
//             _availableIds.erase(_availableIds.begin());
//             return ft::shared_ptr<Client_id>(new Client_id(id, true));
//         }
//     } else if (_availableIds.empty()) {
//         return ft::shared_ptr<Client_id>(new Client_id(_nextId++, true));
//     } else {
//         clinet_id_t id = *_availableIds.begin();
//         _availableIds.erase(_availableIds.begin());
//         return ft::shared_ptr<Client_id>(new Client_id(id, true));
//     }
// }
ft::shared_ptr<Client_id> ClientIdManager::allocateId() {
    if (_nextId == INT_MAX) {
        if (_availableIds.empty()) {
            throw ClientLimitExceededException();
        } else {
            clinet_id_t id = *_availableIds.begin();
            
            _availableIds.erase(_availableIds.begin());
            return ft::shared_ptr<Client_id>(new Client_id(id, true, ActionNone));
        }
    } else if (_availableIds.empty()) {
        return ft::shared_ptr<Client_id>(new Client_id(_nextId++, true, ActionNone));
    } else {
        clinet_id_t id = *_availableIds.begin();
        _availableIds.erase(_availableIds.begin());
        return ft::shared_ptr<Client_id>(new Client_id(id, true, ActionNone));
    }
}
void ClientIdManager::releaseId(ft::shared_ptr<Client_id> id) {
    if (id->getId() == _nextId - 1) {
        _nextId--;
    } else {
        _availableIds.insert(id->getId());
    }
    id->release();
}