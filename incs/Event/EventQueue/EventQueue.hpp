#ifndef EVENTQUEUE_HPP
# define EVENTQUEUE_HPP

#include <exception>

#if defined(__APPLE__) || defined(__FreeBSD__)
# include <sys/event.h>
typedef struct kevent event_t;
#elif defined(__linux__)
# include <sys/epoll.h>
typedef struct epoll_event event_t;
#else
# error "Unsupported platform for event handling"
#endif

#include "../EventBase/Event.hpp"
#include <vector>

class EventQueue {
	public:
		enum EventSetIndex {
			READ_SET,
			WRITE_SET
		};

	private:
		static const int MAX_EVENTS = 1024;
		static EventQueue *_instance;

	private:
		int _fd;
		event_t _ev_set;
		event_t _ev_list[MAX_EVENTS];

	private:
		EventQueue(void);

	public:
		~EventQueue(void);

	public:
		static EventQueue &getInstance(void);
		void deleteInstance(void);

	public:
		int pullEvents(void);
		bool pushEvent(Event *event);
		bool popEvent(Event *event);

	public:
		int getEventFd(int idx) const;
		int getEventQueueFd(void) const;
		Event *getEventData(int idx) const;
		event_t *getEventList(void);
		event_t *getEventSet(void);
		event_t *getEventSetElementPtr(void);

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
