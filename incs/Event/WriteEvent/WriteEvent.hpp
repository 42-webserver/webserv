#ifndef WRITEEVENT_HPP
# define WRITEEVENT_HPP

# include "../EventBase/Event.hpp"
# include "WriteEventHandler.hpp"
# include <vector>

class WriteEvent: public Event {
	private:
		//Todo Buffer IoReadWriter
	protected:
		WriteEvent(WriteEventHandler* writeEventHandler);

	public:	
		virtual	~WriteEvent(void);
		
	private:
		WriteEvent(const WriteEvent &ref);
		WriteEvent	&operator=(const WriteEvent &rhs);
	public:
		virtual void	callEventHandler(void) = 0;
		virtual void	onboardQueue(void)  = 0;
		virtual void	offboardQueue(void)  = 0;
	protected:
		// const std::vector<char>	&getBuffer(void);
		// std::vector<char>	&getBufferRef(void);
};

#endif