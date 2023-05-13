#ifndef READEVENT_HPP
# define READEVENT_HPP

# include "Event.hpp"
# include <vector>

class ReadEvent: public Event {
	public:
		static const int	MAX_SIZE = (16 * 1024);

	private:
		std::vector<char>	_buffer;

	public:
		ReadEvent(void);
		ReadEvent(int fd);
		ReadEvent(const ReadEvent &ref);
		virtual	~ReadEvent(void);
		ReadEvent	&operator=(const ReadEvent &rhs);

	public:
		const std::vector<char>	&getBuffer(void) const;
	
	public:
		bool	append(const char *buf);
};

#endif