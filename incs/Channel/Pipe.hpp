#ifndef PIPE_HPP
# define PIPE_HPP

#include "Channel.hpp"
#include <unistd.h>
#include "../../libs/shared_ptr/shared_ptr.hpp"
//Todo : 상황에 맞게 구현해야됨 
class CgiChannel{
public:
	typedef enum e_pipe{
		READ = 0,
		WRITE = 1,
		Count = 2
	}				e_pipe;
private:
	ft::shared_ptr<Channel>		_channel[Count];
public:
	int getFd(e_pipe pipe) const;
	void Build(void);
	void Run(void);
	void Close(e_pipe pipe);
};

#endif