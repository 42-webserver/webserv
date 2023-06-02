#ifndef EVENTQUEUE_HPP
# define EVENTQUEUE_HPP

# include <exception>
# include <sys/event.h>
# include "../event/Event.hpp"

class Event;

enum EventType {
	LISTEN,
	READ,
	WRITE,
	Timer
};

enum EvSetIndex {
	READ_SET,
	WRITE_SET
};

class EventQueue {
	private:
		static const int	MAX_EVENTS = 16;
		static EventQueue	*_instance;
		EventQueue();
	private:
		int				_fd;
		struct kevent	_ev_set[2];
		struct kevent	_ev_list[MAX_EVENTS];

	// Constructor & Destructor
	public:
		static EventQueue	&getInstance(void) {
			if (_instance == NULL) {
				_instance = new EventQueue();
			}
			return (*_instance);
		}
		~EventQueue();
	// Util
	public:
		int		pullEvents(void);
		int		getEventFd(int idx) const;
		Event	*getEventData(int idx) const;
		bool	pushEvent(Event *event);
		bool	popEvent(Event *event);
		int		getEvQueFd(void) const;
		struct kevent	*getEventList(void);
		struct kevent 	*getEventSet(void);
		struct kevent *getEvSetElementPtr(EvSetIndex index);
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