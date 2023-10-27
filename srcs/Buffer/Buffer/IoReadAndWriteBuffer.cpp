// #include "../../../incs/Buffer/Buffer/IoReadAndWriteBuffer.hpp"
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>

// Function pointer array
IoReadAndWriteBuffer::_sizeFunc IoReadAndWriteBuffer::_sizeFuncs[StateSize] = {
    &IoReadAndWriteBuffer::_initSize,
    &IoReadAndWriteBuffer::_AppendSize,
    &IoReadAndWriteBuffer::_EraseSize
};

// IoReadAndWriteBuffer::IoReadAndWriteBuffer() : 
// (), IoAble(), Modifiable(), 
// _state(InitState), _lst(std::list<ft::shared_ptr<BaseNode> >()) {}

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
        if (_state == InitState) {
            _lst.push_back(ft::shared_ptr<BaseNode>(new NormalNode()));
            _state = AppendState;
        }
        else if (_state == AppendState) {
            _lst.push_back(ft::shared_ptr<BaseNode>(new LargeNode()));
        }
        else {
            // ft::Assert::_assert((false), "IoReadAndWriteBuffer::_allocate Invarint Error");
        }

}
void IoReadAndWriteBuffer::_deallocate() {
        // ft::Assert::_assert((_state == EraseState), "IoReadAndWriteBuffer::_deallocate Invarint Error");
        _lst.pop_front();        
}
size_t IoReadAndWriteBuffer::ioRead(int fd) {
    size_t size = 0;

    try {
        if (_lst.empty()) _allocate();

        size = _lst.back()->ioRead(fd);
        if (size <= 0) return size;
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
        if (size <= 0) return size;
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

size_t IoReadAndWriteBuffer::ioReadToRemainigSize(int fd, size_t remainingSize) {
    size_t size = 0;

    try {
        if (_lst.empty()) _allocate();

        size = _lst.back()->ioReadToRemainigSize(fd, remainingSize);
        if (size <= 0) return size;
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


size_t IoReadAndWriteBuffer::appendString(const std::string& str) {
    size_t size = 0;
    size_t appendSize = str.size();
    try {
        if (_lst.empty()) _allocate();

        while (size == appendSize) {
            size = _lst.back()->insertString(str);
            if (_lst.back()->isFull()) _allocate();
            appendSize -= size;
        }
    }
    catch (const std::exception& e) {
        throw;
    }
    catch (...) {
        throw;
    }
    return size;
}
size_t IoReadAndWriteBuffer::ioSaveWrite(int fd, size_t start) {
    std::list<ft::shared_ptr<BaseNode> >::iterator it = _lst.begin();
    size_t accumulatesize = 0;
    size_t writtensize = 0;
    try {
        if (_lst.empty()) return 0;

        // size = _lst.front()->ioSaveWrite(fd, start);
        // if (size <= 0) return size;
        // _lst.front()->erase(size);
        // if (_state != EraseState) _state = EraseState;
        for (; it != _lst.end(); ++it) {
            if (start < accumulatesize + (*it)->size()) {
                writtensize = (*it)->ioSaveWrite(fd, start - accumulatesize);
                break;
            }
            accumulatesize += (*it)->size();
        }

    }
    catch (const std::exception& e) {
        throw;
    }
    catch (...) {
        throw;
    }
    return writtensize >= 0 ? writtensize + start : start;
}

size_t IoReadAndWriteBuffer::copyHeadTo(std::vector<char>& dest) {
    if (_lst.empty()) return 0;

    return _lst.front()->copyTo(dest);
}
size_t IoReadAndWriteBuffer::append(std::vector<char>::iterator begin, std::vector<char>::iterator end) {
    size_t size = 0;
    size_t appendSize = std::distance(begin, end);
    try {
        if (_lst.empty()) _allocate();
        while (size == appendSize) {
            size = _lst.back()->insert(begin, end);
            if (_lst.back()->isFull()) _allocate();
            begin += size;
            appendSize -= size;
        }
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
    size_t appendSize = size;
    try {
        if (_lst.empty()) _allocate();

        while (ret == appendSize) {
            ret = _lst.back()->insert(begin, size);
            if (_lst.back()->isFull()) _allocate();
            begin += ret;
            appendSize -= ret;
        }
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
