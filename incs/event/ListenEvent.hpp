#ifndef LISTENEVENT_HPP
# define LISTENEVENT_HPP

# include "./Event.hpp"

class ListenEvent: public Event {
	public:
		ListenEvent(int fd, EventHandler *ReadEventHandler);
		virtual	~ListenEvent(void);
		virtual void	callEventHandler(void);


	private:
		ListenEvent	&operator=(const ListenEvent &rhs);
		int	accept(void) const throw(std::exception);
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
		
	public:
		virtual void	handleEvent(Event &event)  throw(std::exception);
	private:
		class FailToAcceptException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class FailToControlException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};
int		ListenEvent::accept(void) const throw(std::exception) {
	int					client_fd;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len;
	int					flags;

	client_addr_len = sizeof(client_addr);
	if ((client_fd = ::accept(this->_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
		throw (FailToAcceptException());
	}
	if ((flags = fcntl(client_fd, F_GETFL, 0)) == -1) {
		close(client_fd);
		throw (FailToControlException());
	}
	if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		close(client_fd);
		throw (FailToControlException());
	}
	return (client_fd);
}
#endif