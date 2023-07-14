#ifndef PIPE_HPP
# define PIPE_HPP

#include "Channel.hpp"
#include <unistd.h>

//Todo : 상황에 맞게 구현해야됨 
class Pipe : public Channel {
public:
	Pipe(void);
	virtual ~Pipe(void);
private:
	virtual void inJectChannelFd(int fd);
private:
	Pipe(const Pipe &ref);
	Pipe	&operator=(const Pipe &rhs);
};
#endif