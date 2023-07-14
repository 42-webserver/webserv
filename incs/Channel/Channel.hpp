#include <unistd.h>
#include <iostream>

#ifndef FILE_HPP
# define FILE_HPP

# include <fcntl.h>
# include "../Log/Logger.hpp"
// file class
class Channel {
	private:
		int _ChannelFd;


	public:
		void setNonBlocking(void) throw (std::exception);
	protected:
		Channel(void);
		virtual void inJectChannelFd(int fd) = 0;
		void setChannelFd(int fd);
		void destroyChannelFd(void);
	private:
		Channel(const Channel &ref);
		Channel	&operator=(const Channel &rhs);
	public:
		virtual ~Channel(void);
		int	getFd() const;
	public:
		class FailToNonBlockException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

#endif