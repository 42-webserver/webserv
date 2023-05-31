#ifndef LISTENEVENT_HPP
# define LISTENEVENT_HPP

# include "./Event.hpp"
# include "../log/logger.hpp"
class ListenEvent: public Event {
	public:
		ListenEvent(int fd, EventHandler *ReadEventHandler);
		virtual	~ListenEvent(void);
		virtual void	callEventHandler(void);
		virtual void	onboardQueue() throw (std::exception);
		virtual void	offboardQueue() throw (std::exception);
	private:
		ListenEvent	&operator=(const ListenEvent &rhs);
		ListenEvent(const ListenEvent &ref);

	public:
		class FailToAcceptException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class FailToControlException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};

class ListenEvHandler: public EventHandler {
	public:
		ListenEvHandler();
		virtual	~ListenEvHandler(void);
	private:
		int	connectClient(int SocketFd) const throw(std::exception);
	public:
		virtual void	handleEvent(Event &event)  throw(std::exception);
	private:
		class FailToAcceptException: public std::exception {
			public:
				const char *what() const throw() {
					return "Failed to accept client connection";
				}
		};

		class FailToControlException: public std::exception {
			public:
				const char *what() const throw() {
					return "Failed to control the event";
				}
		};

};

class ListenEvFactory : public EventFactory {
public:
    static ListenEvFactory& getInstance() {
        static ListenEvFactory instance;
        return instance;
    }

    Event* createEvent(int fd) {
        return new ListenEvent(fd, new ListenEvHandler());
    }

private:
    ListenEvFactory() : EventFactory() {}
    ListenEvFactory(const ListenEvFactory&);
    ListenEvFactory& operator=(const ListenEvFactory&);
};

#endif