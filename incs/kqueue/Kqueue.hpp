#ifndef KQUEUE_HPP
# define KQUEUE_HPP

# include <exception>
# include <sys/event.h>

class Kqueue {
	private:
		static const int	MAX_EVENTS = 10;

	private:
		int				_kq_fd;
		struct kevent	_ev_set[2];
		struct kevent	_ev_list[MAX_EVENTS];

	// Constructor & Destructor
	public:
		Kqueue(void);
		Kqueue(int server_fd);
		~Kqueue(void);
		Kqueue(const Kqueue &ref);
		Kqueue	&operator=(const Kqueue &rhs);

	// Setters
	bool	setEvent(int fd, int filter, int flags, void *udata) throw(std::exception);

	// Util
	public:
		int	getEventCount(void);
		int	getEventFd(int idx) const throw(std::exception);
	
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

		class InvalidParameterException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class FailToGetEventCountException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class TimeoutException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class OutOfRangeException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

#endif