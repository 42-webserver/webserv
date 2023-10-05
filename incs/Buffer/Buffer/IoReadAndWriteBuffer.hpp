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
class IoReadAndWriteBuffer : public BaseBuffer, IoAble, Modifiable<IoReadAndWriteBuffer> {
public:
    typedef enum{
        InitState,
        NormalState,
        LargeState,
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
//Size
private:
    size_t _initSize();
    size_t _normalSize();
    size_t _largeSize();
    typedef size_t (IoReadAndWriteBuffer::*_sizeFunc)();
    static _sizeFunc _sizeFuncs[StateSize];

//IoAble interface

public:
    virtual size_t	ioRead(int fd);
    //IoRead
private:
    size_t _ioReadInit(int fd);
    size_t _ioReadNormal(int fd);
    size_t _ioReadLarge(int fd);
    typedef size_t (IoReadAndWriteBuffer::*_ioReadFunc)(int fd);
    static _ioReadFunc _ioReadFuncs[StateSize];

public:
    virtual size_t	ioWrite(int fd);
    //IoWrite
private:
    size_t _ioWriteInit(int fd);
    size_t _ioWriteNormal(int fd);
    size_t _ioWriteLarge(int fd);
    typedef size_t (IoReadAndWriteBuffer::*_ioWriteFunc)(int fd);
    static _ioWriteFunc _ioWriteFuncs[StateSize];   


//Modifiable interface

public:
    size_t appendImpl(const char* data, size_t size);
private:
    size_t _appendInit(const char* data, size_t size);
    size_t _appendNormal(const char* data, size_t size);
    size_t _appendLarge(const char* data, size_t size);
    typedef size_t (IoReadAndWriteBuffer::*_appendFunc)(const char* data, size_t size);
    static _appendFunc _appendFuncs[StateSize];

public:
    size_t popFrontImpl(size_t size);
private:
    size_t _popFrontInit(size_t size);
    size_t _popFrontNormal(size_t size);
    size_t _popFrontLarge(size_t size);
    typedef size_t (IoReadAndWriteBuffer::*_popFrontFunc)(size_t size);
    static _popFrontFunc _popFrontFuncs[StateSize];
//IoReadAndWriteBuffer interface
public:
    void	_allocate();
    void	_deallocate();
private:
    void _allocateInit();
    void _allocateNormal();
    void _allocateLarge();
    typedef void (IoReadAndWriteBuffer::*_allocateFunc)();
    static _allocateFunc _allocateFuncs[StateSize];

    void _deallocateInit();
    void _deallocateNormal();
    void _deallocateLarge();
    typedef void (IoReadAndWriteBuffer::*_deallocateFunc)();
    static _deallocateFunc _deallocateFuncs[StateSize];
};

#endif