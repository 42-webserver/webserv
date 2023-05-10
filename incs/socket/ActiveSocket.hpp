#ifndef ACTIVESOCKET_HPP
# define ACTIVESOCKET_HPP

# include "Socket.hpp"

class ActiveSocket: public Socket {
	public:
		ActiveSocket(void);
		ActiveSocket(int fd);
		virtual	~ActiveSocket(void);
};

#endif