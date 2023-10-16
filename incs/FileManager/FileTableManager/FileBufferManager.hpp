#ifndef FILEBUFFERMANAGER_HPP
# define FILEBUFFERMANAGER_HPP

#include <FileManager/FileState.hpp>
#include <iostream>
#include <deque>
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
#include <Client/ClientIdManager.hpp>
#include <map>
#include <../../libs/Library/Optional.hpp>

class CurrentSharedWriteClient
{
private:
    size_t _currentWritePosition;
    ft::shared_ptr<Client_id > _client_id;
public:
    CurrentSharedWriteClient(const ft::shared_ptr<Client_id> &client_id);
    CurrentSharedWriteClient(void);
    ~CurrentSharedWriteClient(void);
    CurrentSharedWriteClient(const CurrentSharedWriteClient &ref);
    CurrentSharedWriteClient &operator=(const CurrentSharedWriteClient &rhs);
public:
    void setCurrentWritePosition(size_t currentWritePosition);
    size_t getCurrentWritePosition(void) const;
    bool isAlive(void);
    bool operator==(const ft::shared_ptr<Client_id> &client_id) const;
};
class FileBufferManager
{
public:
    typedef typename std::deque<ft::shared_ptr<Client_id> >::iterator clientQueueIterator;
public:
    FileBufferManager(void);
    ~FileBufferManager(void);
public:
    void registerClientWaitingForFile(ft::shared_ptr<Client_id> &client_id);
private:
    FileBufferManager(const FileBufferManager &ref);
    FileBufferManager &operator=(const FileBufferManager &rhs);
    // size_t &getClientWritePostion(const ft::shared_ptr<Client_id> &client_id);
    // bool checkClientEquality(const ft::shared_ptr<Client_id> &client_id);
    e_file_msg ioWriteUsingSharedBuffer(const ft::shared_ptr<Client_id> &client_id);
    bool   isClientRegistered(const ft::shared_ptr<Client_id> &client_id);
private:
    std::deque<ft::shared_ptr<Client_id> > _clientWaitingQueue;
    std::map<clinet_id_t, clientQueueIterator> _clientWaitingQueueTable;
    ft::shared_ptr<IoReadAndWriteBuffer> _readFromFileToClientBuffer;
    std::map<clinet_id_t, CurrentSharedWriteClient > _currentWriteClients;
    ft::Optional<size_t> _bufferFinalSize;
};

#endif