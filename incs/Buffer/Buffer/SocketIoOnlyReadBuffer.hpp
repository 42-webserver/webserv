#ifndef READSOCKETBUFFER_HPP
# define READSOCKETBUFFER_HPP

#include "BaseBuffer.hpp"
#include "../Node/LargeNode.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../Channel/Socket.hpp"
#include <vector>

class SocketIoOnlyReadBuffer : public BaseBuffer {
private:
	ft::shared_ptr<LargeNode>	_head;
	ft::shared_ptr<Socket>		_socket;
public:
	size_t	ioRead();
	const std::vector<char> &getBufferVectorForm();
};

#endif