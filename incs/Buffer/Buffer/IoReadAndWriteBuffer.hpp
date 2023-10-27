#ifndef IOREADANDWRITEBUFFER_HPP
# define IOREADANDWRITEBUFFER_HPP

#include "BaseBuffer.hpp"
#include "../Node/LargeNode.hpp"
#include "../Node/NormalNode.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../../libs/Library/Optional.hpp"
#include "../../Channel/Socket.hpp"
#include <list>
#include "IoAble.hpp"
#include "Modifiable.hpp"

//erase :: if only HeadNode, then kill buffer
//erase :: if lst -> kill LargeNode in lst
class IoReadAndWriteBuffer : public BaseBuffer, IoAble, Modifiable{
public:
    typedef enum{
        InitState,
        AppendState,
        EraseState,
        StateSize
    }   State;
private:
    State _state;
    std::list<ft::shared_ptr<BaseNode> > _lst;
//Constructor
public:
    IoReadAndWriteBuffer();
    virtual ~IoReadAndWriteBuffer();
//Copy, Assign => delete
private:
    IoReadAndWriteBuffer(const IoReadAndWriteBuffer& other);
    IoReadAndWriteBuffer& operator=(const IoReadAndWriteBuffer& other);
//Buffer Inteface
public:
    virtual size_t size();
private:
    size_t _initSize();
    size_t _AppendSize();
    size_t _EraseSize();
    typedef size_t (IoReadAndWriteBuffer::*_sizeFunc)();
    static _sizeFunc _sizeFuncs[StateSize];
//IoAble interface

public:
    virtual size_t	ioRead(int fd);
    virtual size_t	ioWrite(int fd);
//Modifiable interface
public:
    virtual size_t append(std::vector<char>::iterator begin, std::vector<char>::iterator end);
    virtual size_t append(std::vector<char>::iterator begin, size_t size);
    virtual size_t eraseFront(size_t size);
//IoReadAndWriteBuffer interface
private:
    void	_allocate();
    void	_deallocate();

public:
    size_t ioReadToRemainigSize(int fd, size_t remainingSize);
public:
    size_t  ioSaveWrite(int fd, size_t start);
    size_t  copyHeadTo(std::vector<char>& dest);
};

#endif