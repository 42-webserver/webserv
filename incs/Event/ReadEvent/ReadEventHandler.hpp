#ifndef READEVENTHANDLER_HPP
# define READEVENTHANDLER_HPP

# include "../EventBase/EventHandler.hpp"

class ReadEventHandler: public EventHandler {
	protected:
		ReadEventHandler(void);
	public:
		virtual	~ReadEventHandler(void);
		virtual void	handleEvent(Event &event) = 0;
};

#endif