#include "../../../incs/Buffer/Node/BaseNode.hpp"
#include <Log/Logger.hpp>

// //Mode
// Mode::Mode() : mode(0) {}
// Mode::Mode(bool read, bool write, bool insert, bool erase, bool canDelete) {
// 	setMode(read, write, insert, erase, canDelete);
// }
// inline void Mode::setMode(bool read, bool write, bool insert, bool erase, bool canDelete) {
//     mode = (read ? 1 : 0) | (write ? 1 : 0) << 1 | (insert ? 1 : 0) << 2 | (erase ? 1 : 0) << 3 | (canDelete ? 1 : 0) << 4;
// }

// inline void Mode::setMode(unsigned char newMode) {
//     mode = newMode;
// }

// inline void Mode::setReadMode() {
// 	mode |= 1;
// }

// inline void Mode::setWriteMode() {
// 	mode |= 2;
// }

// inline void Mode::setInsertMode() {
// 	mode |= 4;
// }

// inline void Mode::setEraseMode() {
// 	mode |= 8;
// }

// inline void Mode::setCanDeleteMode() {
// 	mode |= 16;
// }
// inline bool Mode::checkMode(Mode newMode) const {
//     return newMode.mode & mode;
// }

// inline bool Mode::isReadMode() const {
//     return mode & 1;
// }

// inline bool Mode::isWriteMode() const {
//     return mode & 2;
// }

// inline bool Mode::isInsertMode() const {
//     return mode & 4;
// }

// inline bool Mode::isEraseMode() const {
//     return mode & 8;
// }

// inline bool Mode::isCanDeleteMode() const {
//     return mode & 16;
// }


//Mode

//debug
void BaseNode::_printBuffer() {
	std::cerr << "buffer: ";
	for (std::vector<char>::iterator it = _buffer.begin(); it != _buffer.end(); it++)
		std::cerr << *it;
	std::cerr << std::endl;
}

BaseNode::AccessKey::AccessKey() {}
BaseNode::AccessKey::~AccessKey() {}

BaseNode::~BaseNode() {}


BaseNode::BaseNode(ssize_t capacity) : 
// _mode(), 
_size(0), _capacity(capacity), _eraseSize(0), _buffer(capacity) {}
ssize_t BaseNode::erase(ssize_t n) {
	// static Mode _assertEraseMode(false, false, false, false, true);
	
	// ft::Assert::_assert(!_mode.checkMode(_assertEraseMode), "Buffer Node Invariant is destroyed (erase is assertion)");
	// _mode.setEraseMode();

	if (_size < n) {
		ssize_t tmp = _size;
		// _mode.setCanDeleteMode();
		_size = 0;
		return tmp;
	}
	_size -= n;
	_eraseSize += n;
	return n;
}

ssize_t BaseNode::insert(std::vector<char>::iterator start, std::vector<char>::iterator end) {
	// static Mode _assertInsertMode(true, true, false, true, true);
	
	// ft::Assert::_assert(!_mode.checkMode(_assertInsertMode), "Buffer Node Invariant is destroyed (insert is exists after erase)");
	// _mode.setInsertMode();

	ssize_t n = std::distance(start, end);
	
	if (n == 0) return n;
	if (n + _size > _capacity)
		n = _capacity - _size;
	std::copy(start, start+n, _buffer.begin() + _size);
	_size += n;
	return n;
}

// ssize_t BaseNode::insertString(const std::string& str) {
// 	// static Mode _assertInsertMode(true, true, false, true, true);
	
// 	// ft::Assert::_assert(!_mode.checkMode(_assertInsertMode), "Buffer Node Invariant is destroyed (insert is exists after erase)");
// 	// _mode.setInsertMode();

// 	ssize_t n = str.size();
	
// 	std::cout << "insertString: " << n << std::endl;
// 	if (n == 0) return n;
// 	if (n + _size <= _capacity)
// 		n = _capacity - _size;
// 	std::copy(str.begin(), str.begin()+n, _buffer.begin() + _size);
// 	_size += n;
// 	return n;
// }
ssize_t BaseNode::insertString(std::string::const_iterator start, ssize_t size) {
	// static Mode _assertInsertMode(true, true, false, true, true);
	
	// ft::Assert::_assert(!_mode.checkMode(_assertInsertMode), "Buffer Node Invariant is destroyed (insert is exists after erase)");
	// _mode.setInsertMode();

	if (size == 0) return size;
	if (size + _size > _capacity)
		size = _capacity - _size;
	std::copy(start, start+size, _buffer.begin());
	_size += size;
	return size;
}

ssize_t BaseNode::insert(std::vector<char>::iterator start, ssize_t size) {
	// static Mode _assertInsertMode(true, true, false, true, true);
	
	// ft::Assert::_assert(!_mode.checkMode(_assertInsertMode), "Buffer Node Invariant is destroyed (insert is exists after erase)");
	// _mode.setInsertMode();

	if (size == 0) return size;
	if (size + _size > _capacity)
		size = _capacity - _size;
	std::copy(start, start+size, _buffer.begin() + _size);
	_size += size;
	return size;
}
ssize_t BaseNode::size() const {return _size;}
void BaseNode::reset(AccessKey key) {
	(void)key;
	_size = 0;
	_eraseSize = 0;
	std::cerr << "reset" << std::endl;
}
ssize_t BaseNode::ioRead(int fd){
	// static Mode _assertReadMode(false, true, true, true, true);

	// ft::Assert::_assert(!_mode.checkMode(_assertReadMode), "Buffer Node Invariant is destroyed (read has created with assert)");
	// _mode.setReadMode();
	ssize_t n = read(fd, _buffer.data() + _size, _capacity - _size);
	if (n < 0) {
		Logger::getInstance().error("read error");
		return n;
	}
	_size += n;
	return n;
}

ssize_t BaseNode::ioReadToRemainigSize(int fd, ssize_t remainingSize) {
	// static Mode _assertReadMode(false, true, true, true, true);

	// ft::Assert::_assert(!_mode.checkMode(_assertReadMode), "Buffer Node Invariant is destroyed (read has created with assert)");
	// _mode.setReadMode();
	ssize_t readSize = _capacity - _size < remainingSize ? _capacity - _size : remainingSize;
	ssize_t n = read(fd, _buffer.data() + _size, readSize);
	_size += n > 0 ? n : 0;
	return n;
}

static bool isSIGPIPE = false;
static bool isHandlerSet = false;
//because of name mangling
extern "C" {
    void handleSIGPIPE(int) {
        isSIGPIPE = true;
    }
}

ssize_t BaseNode::ioWrite(int fd) {
    // static Mode _assertWriteMode(false, false, false, false, true); 

    if (!isHandlerSet) {
        signal(SIGPIPE, handleSIGPIPE);
        isHandlerSet = true;
    }

    // ft::Assert::_assert(!_mode.checkMode(_assertWriteMode), "Buffer Node Invariant is destroyed (write has created with assert)");
    // _mode.setWriteMode();

    ssize_t n = write(fd, _buffer.data() + _eraseSize, _size);
	std::cerr << "baseNode::ioWrite: ";
	std::cerr << "n: " << n << std::endl;
	std::cerr << "eraseSize: " << _eraseSize << std::endl;
	std::cerr << "size: " << _size << std::endl;
	// for (std::vector<char>::iterator it = _buffer.begin() + _eraseSize; it != _buffer.begin() + _size + _eraseSize; it++)
	// 	std::cerr << *it;
    if (isSIGPIPE) {
        isSIGPIPE = false; // 플래그 재설정
        throw DisconnectionException();
    }

    return n;
}

ssize_t ft::_ioWrite(int fd, std::vector<char>& buffer, ssize_t start) {
	// static Mode _assertWriteMode(false, false, false, false, true); 

	if (!isHandlerSet) {
		signal(SIGPIPE, handleSIGPIPE);
		isHandlerSet = true;
	}

	// ft::Assert::_assert(!_mode.checkMode(_assertWriteMode), "Buffer Node Invariant is destroyed (write has created with assert)");
	// _mode.setWriteMode();
	
	ssize_t n = write(fd, buffer.data() + start, buffer.size() - start);
	std::vector<char>::iterator end = buffer.begin() + buffer.size() - start;
	std::cerr << "ft::_ioWrite: ";
	for (std::vector<char>::iterator it = buffer.begin() + start; it != end; it++)
		std::cerr << *it;
	if (isSIGPIPE) {
		isSIGPIPE = false; // 플래그 재설정
		throw DisconnectionException();
	}
	return n;
}



ssize_t BaseNode::ioSaveWrite(int fd, ssize_t start) {
	// static Mode _assertWriteMode(false, false, false, false, true); 

	if (!isHandlerSet) {
		signal(SIGPIPE, handleSIGPIPE);
		isHandlerSet = true;
	}

	// ft::Assert::_assert(!_mode.checkMode(_assertWriteMode), "Buffer Node Invariant is destroyed (write has created with assert)");
	// _mode.setWriteMode();

	ssize_t n = write(fd, _buffer.data() + start, _size - start);
	
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
	return _buffer.begin() + _eraseSize;
}

BaseNode::iterator BaseNode::end() {
	return _buffer.begin() + _size;
}

BaseNode::const_iterator BaseNode::begin() const {
	return _buffer.begin() + _eraseSize;
}

BaseNode::const_iterator BaseNode::end() const {
	return _buffer.begin() + _size;
}

bool BaseNode::isEmpty() const {
	return _size == 0;
}

ssize_t BaseNode::copyTo(std::vector<char>& dest) {
	ssize_t n = _size;
	dest.resize(n);
	std::copy(_buffer.begin() + _eraseSize, _buffer.begin() + _size, dest.begin());
	return n;
}
