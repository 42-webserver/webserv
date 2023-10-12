#ifndef CLIENTIDMANAGER_HPP
# define CLIENTIDMANAGER_HPP

# include <set>
# include <Client/ClientLimitExceededException.hpp>
class ClientIdManager {
private:
    std::set<int> _availableIds;
    int _nextId;
    static ClientIdManager *_instance;
private:
    ClientIdManager();
    ClientIdManager(const ClientIdManager &src);
    ClientIdManager &operator=(const ClientIdManager &rhs);
    ~ClientIdManager();
public:
    static ClientIdManager &getInstance();

public:
    int allocateId();
    void releaseId(int id);
};
#endif