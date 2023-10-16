#include <Client/ClientIdManager.hpp>

Client_id::fileActionKey::fileActionKey(void) {}
Client_id::fileActionKey::~fileActionKey(void) {}
Client_id::Client_id(clinet_id_t id, bool isAvailable, 
e_client_file_action fileAction, ft::shared_ptr<Channel> socket) : _id(id), _isAvailable(isAvailable), _fileAction(fileAction),
_socket(socket) {}
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
void Client_id::setFileAction(e_client_file_action fileAction,
const fileActionKey &fileActionKey) {
    (void)fileActionKey;
    _fileAction = fileAction;
}
bool Client_id::operator==(const Client_id &rhs) const {
    return _id == rhs._id && _isAvailable && rhs._isAvailable;
}

bool Client_id::operator!=(const Client_id &rhs) const {
    return !(*this == rhs);
}
//ClientIdManager
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
//Todo: check this
ft::shared_ptr<Client_id> ClientIdManager::allocateId(
ft::shared_ptr<Channel> socket) {
    if (_nextId == INT_MAX) {
        if (_availableIds.empty()) {
            throw ClientLimitExceededException();
        } else {
            clinet_id_t id = *_availableIds.begin();
            
            _availableIds.erase(_availableIds.begin());
            return ft::shared_ptr<Client_id>(new Client_id(id, true, ActionNone, socket));
        }
    } else if (_availableIds.empty()) {
        return ft::shared_ptr<Client_id>(new Client_id(_nextId++, true, ActionNone, socket));
    } else {
        clinet_id_t id = *_availableIds.begin();
        _availableIds.erase(_availableIds.begin());
        return ft::shared_ptr<Client_id>(new Client_id(id, true, ActionNone, socket));
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