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
		~Socket(void);
		Socket(const Socket &ref);
		Socket	&operator=(const Socket &rhs);

	// Getters
	public:
		int		getFd(void) const;

	// Utils
	public:
		bool	init(int port) throw(std::exception);
		int		accept(void) throw(std::exception);

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

		class FailToAcceptException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class FailToControlException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

#endif