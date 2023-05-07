#ifndef KQUEUE_HPP
# define KQUEUE_HPP

# include <exception>
# include <sys/event.h>

class Kqueue {
	private:
		static const int	MAX_EVENTS = 16;

	private:
		int				_fd;
		struct kevent	_ev_set[2];
		struct kevent	_ev_list[MAX_EVENTS];

	// Constructor & Destructor
	public:
		Kqueue(void);
		~Kqueue(void);
		Kqueue(const Kqueue &ref);
		Kqueue	&operator=(const Kqueue &rhs);

	// Util
	public:
		bool	init(int server_fd) throw(std::exception);
		int		length(void);
		bool	addEvent(int client_fd);
		bool	deleteEvent(int client_fd);
		int		getEventFd(int idx) const throw(std::exception);
	
	// Exception
	public:
		class FailToCreateException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class FailToControlException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class FailToGetEventException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class TimeoutException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

#endif