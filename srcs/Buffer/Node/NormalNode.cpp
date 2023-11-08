#include "../../../incs/Buffer/Node/NormalNode.hpp"

const ssize_t NormalNode::_sizeNormal = 4 * 1024;
NormalNode::NormalNode() : BaseNode(NormalNode::_sizeNormal){}
NormalNode::~NormalNode() {}
bool NormalNode::isFull() const {
    return size() == NormalNode::_sizeNormal;
}
