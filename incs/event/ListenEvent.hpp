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
		virtual void	handleEvent(Event &event);
};

#endif