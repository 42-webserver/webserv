#include "../../../incs/Buffer/Node/LargeNode.hpp"

const size_t LargeNode::_sizeLarge = 64 * 1024;
LargeNode::LargeNode() : BaseNode(LargeNode::_sizeLarge){}
LargeNode::~LargeNode() {}
bool LargeNode::isFull(void) const {
    return size() == LargeNode::_sizeLarge;
}