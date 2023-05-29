#ifndef READEVENT_HPP
# define READEVENT_HPP

# include "./Event.hpp"
# include <vector>

class ReadEvent: public Event {
	public:
		static const int	BUF_SIZE = (16 * 1024);

	protected:
		std::vector<char>	_buffer;

	public:
		ReadEvent(int fd, EventHandler *ReadEventHandler);
		virtual	~ReadEvent(void);
		virtual void	callEventHandler(void) = 0;
	public:
		//  const std::vector<char>	&getBuffer(void);
	
	private:
		ReadEvent(const ReadEvent &ref);
		ReadEvent	&operator=(const ReadEvent &rhs);
};

class ReadEventHandler: public EventHandler {
	public:
		virtual	~ReadEventHandler(void);
		virtual void	handleEvent(Event &event) = 0;
};




class ReadEvClient: public ReadEvent {
	public:
		ReadEvClient(int fd, EventHandler *ReadEvClientHandler);
		virtual	~ReadEvClient(void);
	public:
		virtual void	callEventHandler(void);
};

class ReadEvClientHandler: public ReadEventHandler {
	public:
		ReadEvClientHandler();
		virtual	~ReadEvClientHandler(void);
	public:
		virtual void	handleEvent(Event &event);
};
class ReadEvCgi: public ReadEvent {
	public:
		ReadEvCgi();
		virtual	~ReadEvCgi(void);
	public:
		virtual void	callEventHandler(void);
};
#endif