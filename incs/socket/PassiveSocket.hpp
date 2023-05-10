#ifndef PASSIVESOCKET_HPP
# define PASSIVESOCKET_HPP

# include "Socket.hpp"

class PassiveSocket: public Socket {
	private:
		int					_enable;
		struct sockaddr_in	_addr;
	
	// Constructor & Destructor
	public:
		PassiveSocket(void);
		PassiveSocket(int port);
		PassiveSocket(const PassiveSocket &ref);
		virtual	~PassiveSocket(void);
		PassiveSocket	&operator=(const PassiveSocket &rhs);

	// Utils
	public:
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