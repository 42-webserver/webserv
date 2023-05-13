#ifndef LISTENEVENT_HPP
# define LISTENEVENT_HPP

# include "Event.hpp"

class ListenEvent: public Event {
	public:
		ListenEvent(void);
		ListenEvent(int fd);
		ListenEvent(const ListenEvent &ref);
		virtual	~ListenEvent(void);
		ListenEvent	&operator=(const ListenEvent &rhs);

	public:
		int	accept(void) const throw(std::exception);

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

#endif