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
public: 
    typedef char value_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef std::random_access_iterator_tag iterator_category;
private:
	ft::Optional<ft::shared_ptr<LargeNode> >_head;
	static IoOnlyReadBuffer* _instance;

//IoReadable interface
public:
	virtual size_t	ioRead(int fd);
//Buffer Inteface
public:
	virtual size_t size();
	virtual ~IoOnlyReadBuffer();
//IoOnlyReadBuffer interface
public:	
	void	_allocate();
	void	recycleInstance();
private:
	IoOnlyReadBuffer();
public:
	static IoOnlyReadBuffer& getInstance();

//Iterator
class iterator{
public:
    typedef char value_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef std::random_access_iterator_tag iterator_category;
private:
    IoOnlyReadBuffer& _buffer; // 버퍼 참조를 유지
    size_t _index; // 현재 위치를 나타내는 인덱스

public:
      iterator(size_t index)
            : _buffer(IoOnlyReadBuffer::getInstance()), _index(index) {}

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

//S
public:
    iterator begin() {
        return iterator(0);
    }

    iterator end() {
        return iterator(size()); // size()를 통해 끝 인덱스 얻기
    }
public:
	std::vector<IoOnlyReadBuffer::iterator> find(const std::string& str);
	IoOnlyReadBuffer::iterator findFirst(const std::string& str);
	size_t copyToVectorBack(std::vector<char>& dest) const ;
private:
	std::vector<int> _getPartialMatch(const std::string &N);
    friend size_t IoReadCopyToVectorBack(std::vector<char>& dest, IoOnlyReadBuffer::iterator begin, IoOnlyReadBuffer::iterator end);
};

#endif