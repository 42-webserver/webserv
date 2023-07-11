#ifndef EVENTQUEUE_HPP
# define EVENTQUEUE_HPP

# include <exception>
# include <sys/event.h>

enum EventSetIndex {
	READ_SET,
	WRITE_SET
};

class EventQueue {
	private:
		static const int	MAX_EVENTS = 16;
		static EventQueue	*_instance;

	private:
		int				_fd;
		struct kevent	_ev_set[2];
		struct kevent	_ev_list[MAX_EVENTS];

	private:
		EventQueue(void);

	public:
		~EventQueue(void);

	public:
		static EventQueue	&getInstance(void);

	public:
		int		pullEvents(void);
		bool	pushEvent(Event *event);
		bool	popEvent(Event *event);

	public:
		int				getEventFd(int idx) const;
		int				getEventQueueFd(void) const;
		Event			*getEventData(int idx) const;
		struct kevent	*getEventList(void);
		struct kevent 	*getEventSet(void);
		struct kevent	*getEventSetElementPtr(EventSetIndex index);

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