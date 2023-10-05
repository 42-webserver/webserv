// #include "../../../incs/Buffer/Buffer/IoReadAndWriteBuffer.hpp"
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>

// Function pointer array
IoReadAndWriteBuffer::_sizeFunc IoReadAndWriteBuffer::_sizeFuncs[StateSize] = {
    &IoReadAndWriteBuffer::_initSize,
    &IoReadAndWriteBuffer::_normalSize,
    &IoReadAndWriteBuffer::_largeSize
};

IoReadAndWriteBuffer::IoReadAndWriteBuffer() : 
BaseBuffer(), IoAble(), Modifiable(), 
_state(InitState), 
_head(ft::Optional<ft::shared_ptr<NormalNode> >()), 
_lst(std::list<ft::shared_ptr<LargeNode> >()) {}
IoReadAndWriteBuffer::~IoReadAndWriteBuffer() {
}






