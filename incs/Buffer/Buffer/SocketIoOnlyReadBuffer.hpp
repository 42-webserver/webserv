#ifndef READSOCKETBUFFER_HPP
# define READSOCKETBUFFER_HPP

#include "BaseBuffer.hpp"
#include "../Node/LargeNode.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../../libs/Library/Optional.hpp"
#include "../../Channel/Socket.hpp"
#include "../../../libs/Library/Assert.hpp"
#include <vector>
#include "../Exception/AllocationException.hpp"
#include "IoAble.hpp"
//Todo: 1)check RequestParser Pattern(어떤 매서드 지원해야되는지)
//Todo: 2)memory Exception
//Todo: 3)iterator
class SocketIoOnlyReadBuffer : public BaseBuffer, IoReadable {
protected:
	ft::Optional<ft::shared_ptr<LargeNode> >_head;
private:
	static SocketIoOnlyReadBuffer* _instance;
//IoReadable interface
public:
	virtual size_t	ioRead(int fd);
	ft::Optional<ft::shared_ptr<LargeNode> >	getHead();
//debug
public:
	size_t	_debugPrint(int fd = 0);
//Buffer Inteface
public:
	virtual size_t size();
	virtual ~SocketIoOnlyReadBuffer();
//SocketIoOnlyReadBuffer interface
public:	
	void	_allocate();
	void	recycleInstance();
private:
	SocketIoOnlyReadBuffer();
public:
	static SocketIoOnlyReadBuffer& getInstance();

//Iterator
class iterator {
public:
    typedef char value_type;
    typedef std::ptrdiff_t difference_type;
    typedef char* pointer;
    typedef char& reference;
    typedef std::random_access_iterator_tag iterator_category;

private:
    SocketIoOnlyReadBuffer& _buffer; // 버퍼 참조를 유지
    size_t _index; // 현재 위치를 나타내는 인덱스

public:
      iterator(size_t index)
            : _buffer(SocketIoOnlyReadBuffer::getInstance()), _index(index) {}

        reference operator*() {
            checkRange();
            return _buffer._head.value()->getBuffer()[_index];
        }

    iterator& operator++() {
        checkRange();
        ++_index;
        return *this;
    }

    iterator operator++(int) {
        checkRange();
        iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    iterator operator+(difference_type diff) const {
        return iterator(_index + diff);
    }

    bool operator==(const iterator& other) const {
        return &_buffer == &other._buffer && _index == other._index;
    }

    bool operator!=(const iterator& other) const {
        return !(*this == other);
    }

private:
    void checkRange() const {
        if (!_buffer._head.has_value() || _index >= _buffer._head.value()->size()) {
            throw std::out_of_range("Index out of range");
        }
    }
};

// In your SocketIoOnlyReadBuffer class
public:
    iterator begin() {
        return iterator(0);
    }

    iterator end() {
        return iterator(size()); // size()를 통해 끝 인덱스 얻기
    }
public:
	std::vector<SocketIoOnlyReadBuffer::iterator> find(const std::string& str);
	SocketIoOnlyReadBuffer::iterator findFirst(const std::string& str);
	void copyToVector(std::vector<char>& dest);
private:
	std::vector<int> _getPartialMatch(const std::string &N);
};

#endif