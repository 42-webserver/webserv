#ifndef WRITEEVENT_HPP
# define WRITEEVENT_HPP

# include "../EventBase/Event.hpp"
# include "WriteEventHandler.hpp"
# include <vector>
# include <Event/EventQueue/EventQueue.hpp>
# include <Event/Exception/KqueueError.hpp>

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
		void _onboardWrite(Event *event, int fd);
		void _offboardWrite(Event *event, int fd);
};

#endif