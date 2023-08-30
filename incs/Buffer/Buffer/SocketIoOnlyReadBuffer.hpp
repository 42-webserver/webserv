#ifndef READSOCKETBUFFER_HPP
# define READSOCKETBUFFER_HPP

#include "BaseBuffer.hpp"
#include "../Node/LargeNode.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../Channel/Socket.hpp"
#include <vector>

//Todo: 1)check RequestParser Pattern(어떤 매서드 지원해야되는지)
//Todo: 2)memory Exception
//Todo: 3)iterator
class SocketIoOnlyReadBuffer : public BaseBuffer {
private:
	ft::shared_ptr<LargeNode>	_head;
	ft::shared_ptr<Socket>		_socket;
public:
	size_t	ioRead();
	size_t	_debugRead(int fd);
	size_t	_debugPrint(int fd = 0);
	
};

#endif