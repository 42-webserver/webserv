#ifndef READEVENT_HPP
# define READEVENT_HPP

# include "../EventBase/Event.hpp"
# include "../EventDto/EventDto.hpp"
# include "../../Log/Logger.hpp"
# include "./ReadEventHandler.hpp"
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

};



#endif