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

//debug
public: 
    void printBuffer();


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
    virtual ssize_t size();
private:
    ssize_t _initSize();
    ssize_t _AppendSize();
    ssize_t _EraseSize();
    typedef ssize_t (IoReadAndWriteBuffer::*_sizeFunc)();
    static _sizeFunc _sizeFuncs[StateSize];
//IoAble interface

public:
    virtual ssize_t	ioRead(int fd);
    virtual ssize_t	ioWrite(int fd);
//Modifiable interface
public:
    virtual ssize_t append(std::vector<char>::iterator begin, std::vector<char>::iterator end);
    virtual ssize_t append(std::vector<char>::iterator begin, ssize_t size);
    virtual ssize_t eraseFront(ssize_t size);
//IoReadAndWriteBuffer interface
private:
    void	_allocate();
    void	_deallocate();

public:
    ssize_t ioReadToRemainigSize(int fd, ssize_t remainingSize);
    ssize_t appendString(const std::string& str);
public:
    ssize_t  ioSaveWrite(int fd, ssize_t start);
    ssize_t  copyHeadTo(std::vector<char>& dest);
};

#endif