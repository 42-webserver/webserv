#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <exception>
# include <netinet/in.h>

class Socket {
	public:
		static const int	MAX_SIZE = 1024;

	private:
		int					_fd;
		int					_enable;
		struct sockaddr_in	_addr;
	
	// Constructor & Destructor
	public:
		Socket(void);
		Socket(int port);
		Socket(const Socket &ref);
		virtual	~Socket(void);
		Socket	&operator=(const Socket &rhs);

	// Utils
	public:
		int		getFd(void) const;

	// Exception
	public:
		class FailToCreateException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class FailToBindException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class FailToListenException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

#endif