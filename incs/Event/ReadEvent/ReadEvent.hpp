#ifndef READEVENT_HPP
# define READEVENT_HPP

# include "../EventBase/Event.hpp"
# include "../EventDto/EventDto.hpp"
# include "../../Log/Logger.hpp"
# include <vector>

class ReadEvent: public Event {
	public:
		static const int	BUF_SIZE = (4 * 1024);

	protected:
		std::vector<char>	_buffer;

	public:
		ReadEvent(int fd, EventHandler *read_event_handler);
		virtual	~ReadEvent(void);

	private:
		ReadEvent(const ReadEvent &ref);
		ReadEvent	&operator=(const ReadEvent &rhs);
	
	public:
		virtual void	callEventHandler(void) = 0;
		virtual void	onboardQueue(void) throw (std::exception) = 0;
		virtual void	offboardQueue(void) throw (std::exception) = 0;

	//public:
		//const std::vector<char>	&getBuffer(void);
};



#endif