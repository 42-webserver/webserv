// #include "../../../incs/Buffer/Buffer/IoReadAndWriteBuffer.hpp"
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>

// Function pointer array
IoReadAndWriteBuffer::_sizeFunc IoReadAndWriteBuffer::_sizeFuncs[StateSize] = {
    &IoReadAndWriteBuffer::_initSize,
    &IoReadAndWriteBuffer::_AppendSize,
    &IoReadAndWriteBuffer::_EraseSize
};

void IoReadAndWriteBuffer::printBuffer() {
    std::cerr << "IoReadAndWriteBuffer::printBuffer" << std::endl;
    std::cerr << "IoReadAndWriteBuffer::printBuffer: _state: " << _state << std::endl;
    std::cerr << "IoReadAndWriteBuffer::printBuffer: _lst.size(): " << _lst.size() << std::endl;
    for (std::list<ft::shared_ptr<BaseNode> >::iterator it = _lst.begin(); it != _lst.end(); ++it) {
        std::cerr << "IoReadAndWriteBuffer::printBuffer: (*it)->size(): " << (*it)->size() << std::endl;
        std::cerr << std::endl;
        (*it)->_printBuffer();
    }
}
// IoReadAndWriteBuffer::IoReadAndWriteBuffer() : 
// (), IoAble(), Modifiable(), 
// _state(InitState), _lst(std::list<ft::shared_ptr<BaseNode> >()) {}

IoReadAndWriteBuffer::IoReadAndWriteBuffer() :
BaseBuffer(), IoAble(), Modifiable(),
_state(InitState), _lst(std::list<ft::shared_ptr<BaseNode> >()) {}
IoReadAndWriteBuffer::~IoReadAndWriteBuffer() {
}

ssize_t IoReadAndWriteBuffer::size() {
    return (this->*_sizeFuncs[_state])();
}

ssize_t IoReadAndWriteBuffer::_initSize() {
    return 0;
}


ssize_t IoReadAndWriteBuffer::_AppendSize() {
   ssize_t size = 0;

    for (std::list<ft::shared_ptr<BaseNode> >::iterator it = _lst.begin(); it != _lst.end(); ++it) {
        size += (*it)->size();
    }
    return size;
}

ssize_t IoReadAndWriteBuffer::_EraseSize() {
    ssize_t size = 0;

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
        // else if (_state == AppendState) {
        //     _lst.push_back(ft::shared_ptr<BaseNode>(new LargeNode()));
        // }
        //Todo: check this for log file
        else {
            _lst.push_back(ft::shared_ptr<BaseNode>(new LargeNode()));
        }

}
void IoReadAndWriteBuffer::_deallocate() {
        // ft::Assert::_assert((_state == EraseState), "IoReadAndWriteBuffer::_deallocate Invarint Error");
        _lst.pop_front();        
}
ssize_t IoReadAndWriteBuffer::ioRead(int fd) {
    ssize_t size = 0;

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

ssize_t IoReadAndWriteBuffer::ioWrite(int fd) {
    ssize_t size = 0;
    std::cerr << "IoReadAndWriteBuffer::ioWrite" << std::endl;
    try {
        if (_lst.empty()) {
            std::cerr << "IoReadAndWriteBuffer::ioWrite: _lst.empty()" << std::endl;
            return 0;
        }

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

ssize_t IoReadAndWriteBuffer::ioReadToRemainigSize(int fd, ssize_t remainingSize) {
    ssize_t size = 0;

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


ssize_t IoReadAndWriteBuffer::appendString(const std::string& str) {
    ssize_t n = 0;
    ssize_t appendSize = str.size();
    ssize_t size = 0;
    // std::cerr << "appendSize : " << appendSize << std::endl;
    std::string::const_iterator current = str.begin();
    try {
        if (_lst.empty()) _allocate();

        while (appendSize != 0) {
            n = _lst.back()->insertString(current, appendSize);
            if (_lst.back()->isFull()) _allocate();
            appendSize -= n;
            current += n;
            size += n;
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
ssize_t IoReadAndWriteBuffer::ioSaveWrite(int fd, ssize_t start) {
    std::list<ft::shared_ptr<BaseNode> >::iterator it = _lst.begin();
    ssize_t accumulatesize = 0;
    ssize_t writtensize = 0;
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

ssize_t IoReadAndWriteBuffer::copyHeadTo(std::vector<char>& dest) {
    if (_lst.empty()) return 0;

    return _lst.front()->copyTo(dest);
}
ssize_t IoReadAndWriteBuffer::append(std::vector<char>::iterator begin, std::vector<char>::iterator end) {
    ssize_t size = 0;
    ssize_t n = 0;
    std::cerr << "IoReadAndWriteBuffer::append1" << std::endl;
    ssize_t appendSize = std::distance(begin, end);
    std::cerr << "IoReadAndWriteBuffer::append1: appendSize: " << appendSize << std::endl;
    try {
        if (_lst.empty()) _allocate();
        while (begin != end) {
            n = _lst.back()->insert(begin, end);
            if (_lst.back()->isFull()) _allocate();
            begin += n;
            appendSize -= n;
            size += n;
        }
        std::cerr << "IoReadAndWriteBuffer::append1 size: " << size << std::endl;
    }
    catch (const std::exception& e) {
        throw;
    }
    catch (...) {
        throw;
    }
    return size;
}

// ssize_t IoReadAndWriteBuffer::append(std::vector<char>::iterator begin, ssize_t size) {
//     ssize_t ret = 0;
//     ssize_t appendSize = size;
//     std::cerr << "IoReadAndWriteBuffer::append2: appendSize: " << appendSize << std::endl;
//     try {
//         if (_lst.empty()) _allocate();

//         while (appendSize != 0) {
//             ret = _lst.back()->insert(begin, size);
//             if (_lst.back()->isFull()) _allocate();
//             begin += ret;
//             appendSize -= ret;
//         }
//     }
//     catch (const std::exception& e) {
//         throw;
//     }
//     catch (...) {
//         throw;
//     }
//     return ret;
// }

ssize_t IoReadAndWriteBuffer::eraseFront(ssize_t size) {
    ssize_t ret = 0;

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
