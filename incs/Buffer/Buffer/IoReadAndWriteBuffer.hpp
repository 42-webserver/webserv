#ifndef IOREADANDWRITEBUFFER_HPP
# define IOREADANDWRITEBUFFER_HPP

#include "BaseBuffer.hpp"
#include "../Node/LargeNode.hpp"
#include "../Node/NormalNode.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../../libs/Library/Optional.hpp"
#include "../../Channel/Socket.hpp"
#include "../../../libs/Library/Assert.hpp"
#include <list>
#include "../Exception/AllocationException.hpp"
#include "IoAble.hpp"
#include "Modifiable.hpp"
class IoReadAndWriteBuffer : public BaseBuffer, IoAble, Modifiable{
public:
    typedef enum{
        InitState,
        NormalState,
        LargeState,
        EraseState,
        StateSize
    }   State;
private:
    State _state;
    ft::Optional<ft::shared_ptr<NormalNode> >_head;
    std::list<ft::shared_ptr<LargeNode> > _lst;
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
    size_t _normalSize();
    size_t _largeSize();
    size_t _eraseSize();
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
    virtual size_t append(IoOnlyReadBuffer::iterator begin, IoOnlyReadBuffer::iterator end);
    virtual size_t append(IoOnlyReadBuffer::iterator begin, size_t size);
    virtual size_t eraseFront(size_t size);
//IoReadAndWriteBuffer interface
public:
    void	_allocate();
    void	_deallocate();
};

#endif