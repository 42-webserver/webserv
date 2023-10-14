#ifndef CLIENTIDMANAGER_HPP
# define CLIENTIDMANAGER_HPP

# include <set>
# include <Client/ClientLimitExceededException.hpp>
# include "../../libs/shared_ptr/shared_ptr.hpp"

typedef int clinet_id_t;
typedef enum {
    Reader,
    Writer,
    Deleter
}   e_client_role;
class Client_id {
private:
    clinet_id_t _id;
    bool _isAvailable;
    e_client_role _role;
public:
    Client_id(clinet_id_t id, bool isAvailable, e_client_role role);
    ~Client_id();
    clinet_id_t getId() const;
    bool isAvailable() const;
    e_client_role getRole() const;
    void release();
};

class ClientIdManager {
public:
    class AcessKey {
    private:
        friend class Client;
        AcessKey(void);
        ~AcessKey(void);
    };
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
    static ClientIdManager &getInstance(const AcessKey &acessKey);

public:
    ft::shared_ptr<Client_id> allocateId(e_client_role role);
    void releaseId(ft::shared_ptr<Client_id> id);
};
#endif