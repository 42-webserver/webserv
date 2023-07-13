#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include "Event.hpp"

class Event;
class EventHandler {
	public:
		virtual ~EventHandler(void);
	private:
		EventHandler(const EventHandler &ref);
		EventHandler	&operator=(const EventHandler &rhs);
	protected:
		EventHandler(void);
	public:
		virtual void	handleEvent(Event &event) = 0;
};

#endif