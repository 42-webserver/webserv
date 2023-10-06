// #include "../../../incs/Buffer/Buffer/IoReadAndWriteBuffer.hpp"
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>

// Function pointer array
IoReadAndWriteBuffer::_sizeFunc IoReadAndWriteBuffer::_sizeFuncs[StateSize] = {
    &IoReadAndWriteBuffer::_initSize,
    &IoReadAndWriteBuffer::_AppendSize,
    &IoReadAndWriteBuffer::_EraseSize
};

IoReadAndWriteBuffer::IoReadAndWriteBuffer() : 
BaseBuffer(), IoAble(), Modifiable(), 
_state(InitState), _lst(std::list<ft::shared_ptr<BaseNode> >()) {}

IoReadAndWriteBuffer::~IoReadAndWriteBuffer() {
}

size_t IoReadAndWriteBuffer::size() {
    return (this->*_sizeFuncs[_state])();
}

size_t IoReadAndWriteBuffer::_initSize() {
    return 0;
}


size_t IoReadAndWriteBuffer::_AppendSize() {
   return ((_lst.size() == 1) ? _lst.front()->size() : 
    NormalNode::_sizeNormal +
    ((_lst.size() == 2) ? _lst.back()->size() : 
    (_lst.size() - 2) * LargeNode::_sizeLarge + _lst.back()->size()));
}

size_t IoReadAndWriteBuffer::_EraseSize() {
    size_t size = 0;

    for (std::list<ft::shared_ptr<BaseNode> >::iterator it = _lst.begin(); it != _lst.end(); ++it) {
        size += (*it)->size();
    }
    return size;
}

void IoReadAndWriteBuffer::_allocate() {
    try {
        if (_state == InitState) {
            _lst.push_back(ft::shared_ptr<BaseNode>(new NormalNode()));
            _state = AppendState;
        }
        else if (_state == AppendState) {
            _lst.push_back(ft::shared_ptr<BaseNode>(new LargeNode()));
        }
        else {
            ft::Assert::_assert((false), "IoReadAndWriteBuffer::_allocate Invarint Error");
        }
    }
    catch (const std::exception& e) {
        throw;
    }
    catch (...) {
        throw;
    }
}
void IoReadAndWriteBuffer::_deallocate() {
        ft::Assert::_assert((_state == EraseState), "IoReadAndWriteBuffer::_deallocate Invarint Error");
        _lst.pop_front();        
}
size_t IoReadAndWriteBuffer::ioRead(int fd) {
    size_t size = 0;

    try {
        if (_lst.empty()) _allocate();

        size = _lst.back()->ioRead(fd);

        if (_lst.back()->isFull()) _allocate();
    }
    catch (const std::exception& e) {
        throw;
    }
    catch (...) {
        throw;
    }
    return size;
}

size_t IoReadAndWriteBuffer::ioWrite(int fd) {
    size_t size = 0;

    try {
        if (_lst.empty()) return 0;

        size = _lst.front()->ioWrite(fd);
        _lst.front()->erase(size);
        if (_state != EraseState) _state = EraseState;
    
        if (_lst.front()->isEmpty()) _deallocate();
    }
    catch (const std::exception& e) {
        throw;
    }
    catch (...) {
        throw;
    }
    return size;
}

size_t IoReadAndWriteBuffer::append(std::vector<char>::iterator begin, std::vector<char>::iterator end) {
    size_t size = 0;

    try {
        if (_lst.empty()) _allocate();

        size = _lst.back()->insert(begin, end);

        if (_lst.back()->isFull()) _allocate();
    }
    catch (const std::exception& e) {
        throw;
    }
    catch (...) {
        throw;
    }
    return size;
}

size_t IoReadAndWriteBuffer::append(std::vector<char>::iterator begin, size_t size) {
    size_t ret = 0;

    try {
        if (_lst.empty()) _allocate();

        ret = _lst.back()->insert(begin, size);

        if (_lst.back()->isFull()) _allocate();
    }
    catch (const std::exception& e) {
        throw;
    }
    catch (...) {
        throw;
    }
    return ret;
}

size_t IoReadAndWriteBuffer::eraseFront(size_t size) {
    size_t ret = 0;

    try {
        if (_lst.empty()) return 0;

        ret = _lst.front()->erase(size);
        if (_state != EraseState) _state = EraseState;

        if (_lst.front()->isEmpty()) _deallocate();
    }
    catch (const std::exception& e) {
        throw;
    }
    catch (...) {
        throw;
    }
    return ret;
}