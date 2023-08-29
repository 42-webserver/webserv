#include "../../incs/Buffer/NormalNode.hpp"

const size_t NormalNode::_sizeNormal = 4 * 1024;
NormalNode::NormalNode() : BaseNode(NormalNode::_sizeNormal){}
~NormalNode::NormalNode() {}
