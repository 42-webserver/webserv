#ifndef IOONLYREADBUFFER_HPP
# define IOONLYREADBUFFER_HPP

#include "BaseBuffer.hpp"
#include "../Node/LargeNode.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../../libs/Library/Optional.hpp"
#include "../../Channel/Socket.hpp"
#include "../../../libs/Library/Assert.hpp"
#include <vector>
#include "../Exception/AllocationException.hpp"
#include "IoAble.hpp"
#include "../../../libs/Library/Container/_iterator.hpp"
//Todo : Refactoring iterator
//Todo: 1)check RequestParser Pattern(어떤 매서드 지원해야되는지)
//Todo: 2)memory Exception
//Todo: 3)iterator
class IoOnlyReadBuffer : public BaseBuffer, IoReadable {
//for iterator
public: 
    typedef std::vector<char>::iterator iterator;
    typedef std::vector<char>::const_iterator const_iterator;
//Constructor
public:
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
	virtual size_t	ioRead(int fd);
//Buffer Inteface
public:
	virtual size_t size();
//IoOnlyReadBuffer interface
public:	
	void	_allocate();
	void	recycleInstance();
private:
	IoOnlyReadBuffer();
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