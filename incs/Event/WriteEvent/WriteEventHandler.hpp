#ifndef WRITEEVENTHANDLER_HPP
# define WRITEEVENTHANDLER_HPP

# include "../EventBase/EventHandler.hpp"

class WriteEventHandler: public EventHandler {
	protected:
		WriteEventHandler(void);
	public:
		virtual	~WriteEventHandler(void);
		virtual void	handleEvent(Event &event) = 0;
};
#endif