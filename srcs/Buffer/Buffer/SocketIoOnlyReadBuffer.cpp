#include "../../../incs/Buffer/Buffer/SocketIoOnlyReadBuffer.hpp"
SocketIoOnlyReadBuffer* SocketIoOnlyReadBuffer::_instance = NULL;
SocketIoOnlyReadBuffer::SocketIoOnlyReadBuffer() :
_head(ft::Optional<ft::shared_ptr<LargeNode> >()) {}
SocketIoOnlyReadBuffer& SocketIoOnlyReadBuffer::getInstance() {
    if (_instance == NULL) {
        try {
        _instance = new SocketIoOnlyReadBuffer();
        }
        catch (const std::bad_alloc& e) {
            throw AllocationException();
        }
        catch (const std::exception& e) {
            throw;
        }
    }
    return *_instance;
}

SocketIoOnlyReadBuffer::~SocketIoOnlyReadBuffer(){}
size_t SocketIoOnlyReadBuffer::size(){
	ft::Assert::_assert((!_head.has_value()),
	"SocketIoOnlyReadBuffer:: size Invarint Error");
	return _head.value_or(0);
}
void	SocketIoOnlyReadBuffer::recycleInstance(){
	ft::Assert::_assert((!_head.has_value()),
	"SocketIoOnlyReadBuffer:: recycleInstance Invarint Error");
	if (_head.has_value()) 
		(*_head)->reset();
}
void	SocketIoOnlyReadBuffer::_allocate() {
    try {
        _head = ft::shared_ptr<LargeNode>(new LargeNode());
    }
    catch (const std::bad_alloc& e) {
        throw AllocationException();
    }
    catch (const std::exception& e) {
        throw;
    }
}

size_t	SocketIoOnlyReadBuffer::ioRead(int fd) {
    //if No alloc->alloc
    try {
        if (!_head.has_value()) _allocate();
    }
    catch (const std::exception& e) {
        throw;
    }

    return _head.value()->ioRead(fd);
}
std::vector<int> SocketIoOnlyReadBuffer::_getPartialMatch(const std::string &N) {
	int m = N.size();
	int begin = 1;
	int matched = 0;
	std::vector<int> pi(m, 0);
	while (begin + matched < m) {
		if (N[begin + matched] == N[matched]){
			matched++;
			pi[begin+matched-1] = matched;
		}
		else {
			if (matched == 0)
				begin++;
			else {
				begin += matched - pi[matched-1];
				matched = pi[matched-1];
			}
		}
	}
	return pi;
}

std::vector<SocketIoOnlyReadBuffer::iterator> SocketIoOnlyReadBuffer::find(const std::string& str) {
    ft::Assert::_assert((!_head.has_value()),
    "SocketIoOnlyReadBuffer:: find Invarint Error");
    if (str.empty()) return {};
    
    std::vector<int> pi = _getPartialMatch(str);
    std::vector<SocketIoOnlyReadBuffer::iterator> result;

    SocketIoOnlyReadBuffer::iterator haystack_begin = this->begin();
    SocketIoOnlyReadBuffer::iterator haystack_end = this->end();
    int m = str.size();
    int begin = 0, matched = 0;

    while (haystack_begin + (begin + m - 1) != haystack_end) {
        if (matched < m && *(haystack_begin + (begin + matched)) == str[matched]) {
            matched++;
            if (matched == m) {
                result.push_back(haystack_begin + begin);
            }
        } else {
            if (matched == 0) {
                begin++;
            } else {
                begin += matched - pi[matched - 1];
                matched = pi[matched-1];
            }
        }
    }
    return result;
}

typename SocketIoOnlyReadBuffer::iterator SocketIoOnlyReadBuffer::findFirst(const std::string& str) {
    ft::Assert::_assert((!_head.has_value()),
    "SocketIoOnlyReadBuffer:: findFirst Invarint Error");
    if (str.empty()) return this->end(); // 빈 문자열이 주어지면 end() 이터레이터 반환
    
    std::vector<int> pi = _getPartialMatch(str);

    SocketIoOnlyReadBuffer::iterator haystack_begin = this->begin();
    SocketIoOnlyReadBuffer::iterator haystack_end = this->end();
    int m = str.size();
    int begin = 0, matched = 0;

    while (haystack_begin + (begin + m - 1) != haystack_end) {
        if (matched < m && *(haystack_begin + (begin + matched)) == str[matched]) {
            matched++;
            if (matched == m) {
                return haystack_begin + begin; // 첫 번째 일치하는 위치의 이터레이터 반환
            }
        } else {
            if (matched == 0) {
                begin++;
            } else {
                begin += matched - pi[matched - 1];
                matched = pi[matched-1];
            }
        }
    }
    return haystack_end; // 일치하는 항목을 찾지 못하면 end() 이터레이터 반환
}
size_t SocketIoOnlyReadBuffer::copyToVectorBack(std::vector<char>& dest) const {
    ft::Assert::_assert((!this->_head.has_value()),
    "SocketIoOnlyReadBuffer:: copyToVectorBack Invarint Error");
    std::vector<char> source = this->_head.has_value() ? this->_head.value()->getBuffer() : std::vector<char>();

    size_t oldSize = dest.size();
    dest.reserve(source.size() + oldSize); 

    std::copy(source.begin(), source.end(), std::back_inserter(dest));
    return dest.size() - oldSize;
}

size_t SocketBufferCopyToVectorBack(std::vector<char>& dest, 
SocketIoOnlyReadBuffer::iterator start, SocketIoOnlyReadBuffer::iterator end)  {
    SocketIoOnlyReadBuffer& buf = SocketIoOnlyReadBuffer::getInstance();
    ft::Assert::_assert((!buf._head.has_value()),
    "SocketIoOnlyReadBuffer:: copyToVectorBack Invarint Error");
    std::vector<char> source = buf._head.has_value() ? buf._head.value()->getBuffer() : std::vector<char>();

    size_t oldSize = dest.size();
    dest.reserve(source.size() + oldSize); 

    std::copy(start, end, std::back_inserter(dest)); 
    return dest.size() - oldSize;
}