#ifndef READEVENT_HPP
# define READEVENT_HPP

# include "../EventBase/Event.hpp"
# include "../EventDto/EventDto.hpp"
# include "../../Log/Logger.hpp"
# include <vector>
# include "./ReadEventHandler.hpp"
class ReadEvent: public Event {
	private:
		std::vector<char>	_buffer;

	protected:
		ReadEvent(ft::shared_ptr<Channel> channel, ReadEventHandler* readEventHandler);
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
		const std::vector<char>	&getBuffer(void);
		std::vector<char>	&getBufferRef(void);
};



#endif