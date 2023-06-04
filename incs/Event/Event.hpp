#ifndef EVENT_HPP
# define EVENT_HPP

# include "../../libs/shared_ptr/shared_ptr.hpp"
# include "../../libs/unique_ptr/unique_ptr.hpp"
# include "../FileDescriptor/FileDescriptor.hpp"
# include "../EventQueue/EventQueue.hpp"
# include "../Socket/Socket.hpp"

class EventQueue;
class EventHandler;

class Event {
	protected:
		ft::shared_ptr<FileDescriptor>	_fd;
		ft::unique_ptr<EventHandler>	_event_handler;

	public:
		Event(int fd, EventHandler *event_handler);
		virtual ~Event(void); // Virtual destructor
	
	private:
		Event(void);
		Event(const Event &ref);
		Event	&operator=(const Event &rhs);

	public:
		virtual void callEventHandler(void) = 0;
		virtual void onboardQueue(void) throw (std::exception) = 0;
		virtual void offboardQueue(void) throw (std::exception) = 0;

	public:
		int	getFd(void) const;

	public:
		class FailToOnboardException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class FailToOffboardException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

class EventHandler {
	public:
		EventHandler(void);
		virtual ~EventHandler(void);  // Virtual destructor
	
	private:
		EventHandler(const EventHandler &ref);
		EventHandler	&operator=(const EventHandler &rhs);

	public:
		virtual void handleEvent(Event &event) = 0;
};

class EventFactory {
	public:
		EventFactory(void);
		virtual ~EventFactory(void);

	private:
		EventFactory(const EventFactory &ref);
		EventFactory	&operator=(const EventFactory &rhs);
	
	public:
		virtual Event	*createEvent(int fd) const = 0;
};

#endif