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
		ReadEvent(int fd, EventHandler *ReadEventHandler, ft::shared_ptr<Kqueue> kqueue);
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

class ReadEventFactory : public EventFactory {
protected:
	ReadEventFactory() : EventFactory() {}

public:
	virtual ~ReadEventFactory() {}
    virtual Event* createEvent(int fd, ft::shared_ptr<Kqueue> kqueue) = 0;

private:
    ReadEventFactory(const ReadEventFactory&);
    ReadEventFactory& operator=(const ReadEventFactory&);
};


class ReadEvClient: public ReadEvent {
	public:
		ReadEvClient(int fd, EventHandler *ReadEvClientHandler, ft::shared_ptr<Kqueue> kqueue);
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

class ReadEvClientFactory : public ReadEventFactory {
public:
    static ReadEvClientFactory& getInstance() {
        static ReadEvClientFactory instance;
        return instance;
    }

    Event* createEvent(int fd, ft::shared_ptr<Kqueue> kqueue) {
        return new ReadEvClient(fd, new ReadEvClientHandler(), kqueue);
    }

private:
    ReadEvClientFactory() : ReadEventFactory() {}
    ReadEvClientFactory(const ReadEvClientFactory&);
    ReadEvClientFactory& operator=(const ReadEvClientFactory&);
};
class ReadEvCgi: public ReadEvent {
	public:
		ReadEvCgi();
		virtual	~ReadEvCgi(void);
	public:
		virtual void	callEventHandler(void);
};
#endif