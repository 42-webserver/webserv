#ifndef READEVENT_HPP
# define READEVENT_HPP

# include "../EventBase/EventHandler.hpp"

class ReadEventHandler: public EventHandler {
	public:
		virtual	~ReadEventHandler(void);
		virtual void	handleEvent(Event &event) = 0;
};

#endif