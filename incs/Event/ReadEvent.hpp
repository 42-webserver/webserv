#ifndef READEVENT_HPP
# define READEVENT_HPP

# include "Event.hpp"
# include "../EventDto/EventDto.hpp"
# include "../Log/Logger.hpp"
#include "../http/parser/HttpRequestParser.hpp"
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

class ReadEventHandler: public EventHandler {
	public:
		virtual	~ReadEventHandler(void);
		virtual void	handleEvent(Event &event) = 0;
};

class ReadEventFactory: public EventFactory {
	protected:
		ReadEventFactory(void);

	public:
		virtual ~ReadEventFactory(void);

	private:
		ReadEventFactory(const ReadEventFactory&);
		ReadEventFactory& operator=(const ReadEventFactory&);

	public:
		virtual Event	*createEvent(const EventDto &EventDto) const = 0;
};


class ReadEventClient: public ReadEvent {
	public:
		ReadEventClient(int fd, EventHandler *read_event_client_handler, ft::shared_ptr<VirtualServerManager> physicalServer);
		virtual	~ReadEventClient(void);

	public:
		ft::shared_ptr<VirtualServerManager>	getPhysicalServer(void) const;

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void) throw (std::exception);
		virtual void	offboardQueue(void) throw (std::exception);
	
	private:
		ft::shared_ptr<VirtualServerManager>	_physical_server;

};

class ReadEventClientHandler: public ReadEventHandler {
	public:
		ReadEventClientHandler(void);
		virtual	~ReadEventClientHandler(void);

	public:
		ft::unique_ptr<HttpRequestParser>	&getHttpRequestParser(void);

	public:
		virtual void	handleEvent(Event &event);

	private:
		ft::unique_ptr<HttpRequestParser>	_http_request_parser;
};

class ReadEventClientFactory: public EventFactory {
	public:
		static ReadEventClientFactory	&getInstance(void);

	public:
		Event	*createEvent(const EventDto &EventDto) const;

	private:
		ReadEventClientFactory(void);
		ReadEventClientFactory(const ReadEventClientFactory &ref);
		ReadEventClientFactory	&operator=(const ReadEventClientFactory &rhs);
};

class ReadEventCgi: public ReadEvent {
	public:
		ReadEventCgi(void);
		virtual	~ReadEventCgi(void);

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void) throw (std::exception);
		virtual void	offboardQueue(void) throw (std::exception);
};
#endif