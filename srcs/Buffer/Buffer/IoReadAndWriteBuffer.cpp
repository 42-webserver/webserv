// #include "../../../incs/Buffer/Buffer/IoReadAndWriteBuffer.hpp"
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>

// Function pointer array
IoReadAndWriteBuffer::_sizeFunc IoReadAndWriteBuffer::_sizeFuncs[StateSize] = {
    &IoReadAndWriteBuffer::_initSize,
    &IoReadAndWriteBuffer::_normalSize,
    &IoReadAndWriteBuffer::_largeSize,
    &IoReadAndWriteBuffer::_eraseSize
};

IoReadAndWriteBuffer::IoReadAndWriteBuffer() : 
BaseBuffer(), IoAble(), Modifiable(), 
_state(InitState), 
_head(ft::Optional<ft::shared_ptr<NormalNode> >()), 
_lst(std::list<ft::shared_ptr<LargeNode> >()) {}
IoReadAndWriteBuffer::~IoReadAndWriteBuffer() {
}

size_t IoReadAndWriteBuffer::size() {
    return (this->*_sizeFuncs[_state])();
}

size_t IoReadAndWriteBuffer::_initSize() {
    return 0;
}

size_t IoReadAndWriteBuffer::_normalSize() {
    return (*_head)->size();
}

size_t IoReadAndWriteBuffer::_largeSize() {
    size_t size = 
}


