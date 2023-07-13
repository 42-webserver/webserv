#ifndef WriteEventToClientHandler_HPP
# define WriteEventToClientHandler_HPP

# include "WriteEventHandler.hpp"

class WriteEventToClientHandler: public WriteEventHandler {

	public:
		WriteEventToClientHandler(void);
		virtual	~WriteEventToClientHandler(void);
		virtual void	handleEvent(Event &event);
};
#endif