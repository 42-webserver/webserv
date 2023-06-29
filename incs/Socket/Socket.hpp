#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <exception>
# include <netinet/in.h>
# include <string>

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
		~Socket(void);
		Socket(const Socket &ref);
		Socket	&operator=(const Socket &rhs);

	// Utils
	public:
		bool	build(const int port, const std::string &ip) throw(std::exception);
		bool	run(void) throw(std::exception);
		int		getFd(void) const;
	
	private:
		static uint32_t	stringToNetworkByteOrder(const std::string &ip);

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

	friend std::ostream	&operator<<(std::ostream &os, const Socket &socket);
};

#endif