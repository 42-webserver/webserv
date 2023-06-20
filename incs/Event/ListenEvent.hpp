#ifndef LISTENEVENT_HPP
# define LISTENEVENT_HPP

# include "Event.hpp"
# include "../Log/Logger.hpp"
# include "../Server/VirtualServerManager.hpp"

class ListenEvent: public Event {
	public:
		ListenEvent(int fd, EventHandler *listen_event_handler, ft::shared_ptr<VirtualServerManager> physical_server);
		virtual	~ListenEvent(void);

	private:
		ListenEvent(const ListenEvent &ref);
		ListenEvent	&operator=(const ListenEvent &rhs);

	private:
		ft::shared_ptr<VirtualServerManager>	_physical_server;
	
	public:
		ft::shared_ptr<VirtualServerManager>	getPhysicalServer(void) const;

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void) throw (std::exception);
		virtual void	offboardQueue(void) throw (std::exception);

	public:
		class FailToAcceptException: public std::exception {
			public:
				const char *what() const throw();
		};
		class FailToControlException: public std::exception {
			public:
				const char *what() const throw();
		};
};

class ListenEventHandler: public EventHandler {
	public:
		ListenEventHandler(void);
		virtual	~ListenEventHandler(void);

	private:
		int	connectClient(int SocketFd) const throw(std::exception);

	public:
		virtual void	handleEvent(Event &event)  throw(std::exception);

	private:
		class FailToAcceptException: public std::exception {
			public:
				const char *what() const throw();
		};
		class FailToControlException: public std::exception {
			public:
				const char *what() const throw();
		};
};

class ListenEventFactory: public EventFactory {
	public:
		static ListenEventFactory	&getInstance(void);

	public:
		Event	*createEvent(const EventDto &EventDto) const;

	private:
		ListenEventFactory(void);
		~ListenEventFactory(void);
		ListenEventFactory(const ListenEventFactory &rhs);
		ListenEventFactory	&operator=(const ListenEventFactory &ref);
};

#endif