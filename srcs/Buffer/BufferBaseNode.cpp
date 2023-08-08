#include "../../incs/Buffer/BufferBaseNode.hpp"

BaseNode::~BaseNode() {}
BaseNode::BaseNode(size_t capacity) : _partialPoint(0), _currentPos(0), _size(0), _buffer(capacity), 
_capacity(capacity), _setToDelete(false) {}
const size_t& BaseNode::getPartialPoint() const { return this->_partialPoint; }
void BaseNode::setPartialPoint(size_t n) { this->_partialPoint = n; }
const size_t& BaseNode::getCurrentPos() const { return this->_currentPos; }
void BaseNode::setCurrentPos(size_t n) { this->_currentPos = n; }
size_t BaseNode::erase(std::vector<char>::iterator start, std::vector<char>::iterator end) {
	size_t n = end - start;

	if (_size < n) {
		size_t tmp = _size;
		_setToDelete = true;
		_size = 0;
		return tmp;
	}
	_partialPoint += n;
	_currentPos = _partialPoint;
	_size -= n;
	return n;
}

size_t BaseNode::insert(std::vector<char>::iterator start, std::vector<char>::iterator end) {
	if (start == end)
		return 0;
	size_t n = end - start;
	if (_capacity - _size - _partialPoint < n) {
		n = _capacity - _size - _partialPoint;
	}
	for (size_t i = 0; i < n; i++) {
		_buffer[_size + i] = *(start + i);
	}
	return n;
}

bool BaseNode::canDelete() const { return _setToDelete; }