#ifndef PIPE_HPP
# define PIPE_HPP

#include "Channel.hpp"
#include <unistd.h>
#include "../../libs/shared_ptr/shared_ptr.hpp"
//Todo : 상황에 맞게 구현해야됨 
class CgiChannel{
public:
	typedef enum e_pipe{
		SERVER_READ = 0,
		SERVER_WRITE,
		CGI_READ,
		CGI_WRITE,
		Count
	}				e_pipe;
private:
	ft::shared_ptr<Channel>		_channel[Count];
public:
	int getFd(e_pipe pipe) const;
	void build(void);
	void run(void);
	void destroy(void);
};

#endif