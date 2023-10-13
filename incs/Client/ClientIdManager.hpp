#ifndef CLIENTIDMANAGER_HPP
# define CLIENTIDMANAGER_HPP

# include <set>
# include <Client/ClientLimitExceededException.hpp>
# include "../../libs/shared_ptr/shared_ptr.hpp"

typedef int clinet_id_t;

class Client_id {
private:
    clinet_id_t _id;
    bool _isAvailable;
public:
    Client_id(clinet_id_t id, bool isAvailable);
    ~Client_id();
    clinet_id_t getId() const;
    void release();
};

class ClientIdManager {
private:
    std::set<clinet_id_t> _availableIds;
    clinet_id_t _nextId;
    static ClientIdManager *_instance;
private:
    ClientIdManager();
    ClientIdManager(const ClientIdManager &src);
    ClientIdManager &operator=(const ClientIdManager &rhs);
    ~ClientIdManager();
public:
    static ClientIdManager &getInstance();

public:
    ft::shared_ptr<Client_id> allocateId();
    void releaseId(ft::shared_ptr<Client_id> id);
};
#endif