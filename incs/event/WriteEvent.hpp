#ifndef WRITEEVENT_HPP
# define WRITEEVENT_HPP

# include "Event.hpp"
# include <vector>

class WriteEvent: public Event {
	private:
		static const int	MAX_SIZE = (16 * 1024);

	private:
		std::vector<char>	_buffer;

	public:
		WriteEvent(void);
		WriteEvent(int fd);
		WriteEvent(const WriteEvent &ref);
		virtual	~WriteEvent(void);
		WriteEvent	&operator=(const WriteEvent &rhs);
};

#endif