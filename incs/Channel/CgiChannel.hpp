#ifndef CGICHANNEL_HPP
# define CGICHANNEL_HPP

#include "Channel.hpp"
#include <unistd.h>
#include "../../libs/shared_ptr/shared_ptr.hpp"
#include "ByteStream.hpp"
#include <unistd.h>

class CgiChannel{
public:
	typedef enum {
		Read = 0,
		Write,
	}			e_std;
	typedef enum e_pipe{
		SERVER_READ = 0,
		CGI_WRITE,
		CGI_READ,
		SERVER_WRITE,
		Count
	}				e_pipe;
private:
	ft::shared_ptr<Channel>		_channel[Count];
private:
	void _destroy(e_pipe type);
public:
	void _dupFdInCgiProcess();
	void _closeServerSideFd();
	void _closeCgiSideFd();
	// void _close
public:
	CgiChannel(void);
	~CgiChannel(void);
private:
	CgiChannel(const CgiChannel &ref);
	CgiChannel	&operator=(const CgiChannel &rhs);
public:
	int getFd(e_pipe pipe) const;
	void build(void);
class FailToCreateException: public std::exception {
	public:
		virtual const char *what() const throw();
};
class FailToDupException: public std::exception {
	public:
		virtual const char *what() const throw();
};
};

#endif