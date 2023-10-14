#ifndef FILEBUFFERMANAGER_HPP
# define FILEBUFFERMANAGER_HPP

#include <FileManager/FileState.hpp>
#include <iostream>
#include <queue>
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
#include <Client/ClientIdManager.hpp>
class FileBufferManager
{
public:
    FileBufferManager(void);
    ~FileBufferManager(void);
private:
    FileBufferManager(const FileBufferManager &ref);
    FileBufferManager &operator=(const FileBufferManager &rhs);
private:
    std::queue<ft::shared_ptr<Client_id> > _clientWaitingQueue;
    ft::shared_ptr<IoReadAndWriteBuffer> _buffer;
};

#endif