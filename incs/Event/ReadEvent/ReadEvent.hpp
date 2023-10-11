#ifndef READEVENT_HPP
# define READEVENT_HPP

# include "../EventBase/Event.hpp"
# include "../EventDto/EventDto.hpp"
# include "../../Log/Logger.hpp"
# include "./ReadEventHandler.hpp"
# include <Event/EventQueue/EventQueue.hpp>
# include <Event/Exception/KqueueError.hpp>
class ReadEvent: public Event {
	protected:
		ReadEvent(ReadEventHandler* readEventHandler);
	public:	
		virtual	~ReadEvent(void);

	private:
		ReadEvent(const ReadEvent &ref);
		ReadEvent	&operator=(const ReadEvent &rhs);
	
	public:
		virtual void	callEventHandler(void) = 0;
		virtual void	onboardQueue(void)  = 0;
		virtual void	offboardQueue(void)  = 0;

	protected:
		void	_onboardRead(Event *event, int fd);
		void	_offboardRead(Event *event, int fd);
};



#endif