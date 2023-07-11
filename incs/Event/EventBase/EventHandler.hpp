#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include "Event.hpp"

class EventHandler {
	public:
		EventHandler();
		virtual ~EventHandler(void);
	
	private:
		EventHandler(const EventHandler &ref);
		EventHandler	&operator=(const EventHandler &rhs);

	public:
		virtual void	handleEvent(Event &event) = 0;
};

#endif