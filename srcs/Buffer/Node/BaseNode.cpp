#include "../../../incs/Buffer/Node/BaseNode.hpp"
#include "../../../incs/Buffer/Exception/DisconnectionException.hpp"
#include <algorithm>
#include "../../../libs/Library/Assert.hpp"
#include <unistd.h>


BaseNode::~BaseNode() {}
BaseNode::BaseNode(size_t capacity) : _size(0), _buffer(capacity)
, _capacity(capacity), _mode(), _eraseSize(0) {}

size_t BaseNode::erase(size_t n) {
	static Mode _assertEraseMode(false, false, false, false, true);
	
	ft::Assert::_assert(!_mode.checkMode(_assertEraseMode), "Buffer Node Invariant is destroyed (erase is assertion)");
	_mode.setEraseMode();

	if (_size < n) {
		size_t tmp = _size;
		_mode.setCanDeleteMode();
		_size = 0;
		return tmp;
	}
	_size -= n;
	_eraseSize += n;
	return n;
}

size_t BaseNode::insert(std::vector<char>::iterator start, std::vector<char>::iterator end) {
	static Mode _assertInsertMode(true, true, false, true, true);
	
	ft::Assert::_assert(!_mode.checkMode(_assertInsertMode), "Buffer Node Invariant is destroyed (insert is exists after erase)");
	_mode.setInsertMode();

	size_t n = std::distance(start, end);
	
	if (n == 0) return n;
	if (n + _size <= _capacity)
		n = _capacity - _size;
	std::copy(start, start+n, _buffer.begin() + _size);
	_size += n;
	return n;
}

size_t BaseNode::insert(std::vector<char>::iterator start, size_t size) {
	static Mode _assertInsertMode(true, true, false, true, true);
	
	ft::Assert::_assert(!_mode.checkMode(_assertInsertMode), "Buffer Node Invariant is destroyed (insert is exists after erase)");
	_mode.setInsertMode();

	if (size == 0) return size;
	if (size + _size <= _capacity)
		size = _capacity - _size;
	std::copy(start, start+size, _buffer.begin() + _size);
	_size += size;
	return size;
}
size_t BaseNode::size() const {return _size;}
bool BaseNode::canDelete() const { return _mode.isCanDeleteMode(); }
void BaseNode::reset() {
	_buffer.clear();
	_size = 0;
	_mode.setMode(false, false, false, false, false);
}
size_t BaseNode::ioRead(int fd){
	static Mode _assertReadMode(false, true, true, true, true);

	ft::Assert::_assert(!_mode.checkMode(_assertReadMode), "Buffer Node Invariant is destroyed (read has created with assert)");
	_mode.setReadMode();

	size_t n = read(fd, _buffer.data() + _size, _capacity - _size);
	_size += n;
	return _size;
}

static bool isSIGPIPE = false;
//because of name mangling
extern "C" {
    void handleSIGPIPE(int) {
        isSIGPIPE = true;
    }
}

size_t BaseNode::ioWrite(int fd) {
    static Mode _assertWriteMode(false, false, false, false, true); 

    static bool handlerSet = false;
    if (!handlerSet) {
        signal(SIGPIPE, handleSIGPIPE);
        handlerSet = true;
    }

    ft::Assert::_assert(!_mode.checkMode(_assertWriteMode), "Buffer Node Invariant is destroyed (write has created with assert)");
    _mode.setWriteMode();

    size_t n = write(fd, _buffer.data() + _eraseSize, _size);

    if (isSIGPIPE) {
        isSIGPIPE = false; // 플래그 재설정
        throw DisconnectionException();
    }

    return n;
}



const std::vector<char>& BaseNode::getBuffer() const {
	return _buffer;
}
std::vector<char>& BaseNode::getBuffer() {
	return _buffer;
}

BaseNode::iterator BaseNode::begin() {
	return _buffer.begin();
}

BaseNode::iterator BaseNode::end() {
	return _buffer.end();
}

BaseNode::const_iterator BaseNode::begin() const {
	return _buffer.begin();
}

BaseNode::const_iterator BaseNode::end() const {
	return _buffer.end();
}

bool BaseNode::isEmpty() const {
	return _size == 0;
}

Mode::Mode() : mode(0) {}

inline void Mode::setMode(bool read, bool write, bool insert, bool erase, bool canDelete) {
    mode = (read ? 1 : 0) | (write ? 1 : 0) << 1 | (insert ? 1 : 0) << 2 | (erase ? 1 : 0) << 3 | (canDelete ? 1 : 0) << 4;
}

inline void Mode::setMode(unsigned char newMode) {
    mode = newMode;
}

inline void Mode::setReadMode() {
	mode |= 1;
}

inline void Mode::setWriteMode() {
	mode |= 2;
}

inline void Mode::setInsertMode() {
	mode |= 4;
}

inline void Mode::setEraseMode() {
	mode |= 8;
}

inline void Mode::setCanDeleteMode() {
	mode |= 16;
}
inline bool Mode::checkMode(Mode newMode) const {
    return newMode.mode & mode;
}

inline bool Mode::isReadMode() const {
    return mode & 1;
}

inline bool Mode::isWriteMode() const {
    return mode & 2;
}

inline bool Mode::isInsertMode() const {
    return mode & 4;
}

inline bool Mode::isEraseMode() const {
    return mode & 8;
}

inline bool Mode::isCanDeleteMode() const {
    return mode & 16;
}

