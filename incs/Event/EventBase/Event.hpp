#ifndef EVENT_HPP
# define EVENT_HPP

# include "../../../libs/shared_ptr/shared_ptr.hpp"
# include "../EventDto/EventDto.hpp"
# include "EventHandler.hpp"

class EventHandler;
class Event {
	protected:
		ft::shared_ptr<EventHandler>	_event_handler;
		Event(EventHandler* event_handler);
	public:
		virtual ~Event(void); // Virtual destructor
	
	private:
		Event(void);
		Event(const Event &ref);
		Event	&operator=(const Event &rhs);

	public:
		virtual void	callEventHandler(void) = 0;
		virtual void	onboardQueue(void) = 0;
		virtual void	offboardQueue(void) = 0;

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

#endif