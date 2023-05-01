#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <exception>
# include <netinet/in.h>

class Socket {
	private:
		int					_fd;
		struct sockaddr_in	_addr;

	// Constructor & Destructor
	public:
		Socket(void);
		Socket(int port);
		~Socket(void);
		Socket(const Socket &ref);
		Socket	&operator=(const Socket &rhs);

	// Getters
	public:
		int		getFd(void) const;

	// Utils
	public:
		bool	bind(void);
		bool	listen(void);

	// Exception
	public:
		class SocketCreationErrorException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class SocketBindErrorException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class SocketListenErrorException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

#endif