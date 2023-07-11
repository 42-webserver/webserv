#ifndef LISTENEVENTHANDLER_HPP
# define LISTENEVENTHANDLER_HPP

# include "../EventBase/EventHandler.hpp"
# include "../EventBase/Event.hpp"

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

#endif