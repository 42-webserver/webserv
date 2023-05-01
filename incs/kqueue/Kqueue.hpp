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
	
	// Exception
	public:
		class KqueueCreationErrorException: public std::exception {
			public:
				virtual const char *what() const throw();
		};

		class KqueueControlErrorException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

#endif