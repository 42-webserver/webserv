#ifndef EVENT_HPP
# define EVENT_HPP

# include "../../libs/shared_ptr/incs/shared_ptr.hpp"
# include "../socket/Socket.hpp"

class Event {
	protected:
		int	_fd;

	public:
		Event(void);
		Event(int fd);
		Event(const Event &ref);
		virtual	~Event(void);
		Event	&operator=(const Event &rhs);
	
	public:
		int	getFd(void) const;
};

#endif