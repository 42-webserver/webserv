#ifndef IOONLYREADBUFFER_HPP
# define IOONLYREADBUFFER_HPP

#include "BaseBuffer.hpp"
#include "../Node/LargeNode.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../../libs/Library/Optional.hpp"
#include <Channel/Channel.hpp>
#include <vector>
#include "IoAble.hpp"


class IoOnlyReadBuffer : public BaseBuffer, IoReadable {
//for iterator
public: 
    typedef std::vector<char>::iterator iterator;
    typedef std::vector<char>::const_iterator const_iterator;
//Constructor => singleton
private:
    IoOnlyReadBuffer();
//Copy, Assign => delete
//Destructor => singleton
private:
    virtual ~IoOnlyReadBuffer();
    IoOnlyReadBuffer(const IoOnlyReadBuffer& other);
    IoOnlyReadBuffer& operator=(const IoOnlyReadBuffer& other);

private:
	ft::Optional<ft::shared_ptr<LargeNode> >_head;
	static IoOnlyReadBuffer* _instance;

//IoReadable interface
public:
	virtual ssize_t	ioRead(int fd);
//Buffer Inteface
public:
	virtual ssize_t size();
//IoOnlyReadBuffer interface
public:	
	void	_allocate();
	void	recycleInstance();
public:
	static IoOnlyReadBuffer& getInstance();

//Iterator


//S
public:
    iterator begin() const;
    iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

};

#endif