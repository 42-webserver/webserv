#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <exception>
# include <netinet/in.h>

class Socket {
	public:
		static const int	MAX_SIZE = 1024;

	protected:
		int					_fd;
	
	// Constructor & Destructor
	public:
		Socket(void);
		Socket(int fd);
		Socket(const Socket &ref);
		virtual	~Socket(void);
		Socket	&operator=(const Socket &rhs);

	// Getters
	public:
		int		getFd(void) const;

	// Exception
	public:
		class FailToCreateException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

#endif