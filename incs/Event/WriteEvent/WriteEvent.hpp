#ifndef WRITEEVENT_HPP
# define WRITEEVENT_HPP

# include "../EventBase/Event.hpp"
# include "WriteEventHandler.hpp"
# include <vector>

class WriteEvent: public Event {
	private:
		std::vector<char>	_buffer;

	protected:
		WriteEvent(ft::shared_ptr<FileDescriptor> fd, WriteEventHandler* writeEventHandler);

	public:	
		virtual	~WriteEvent(void);
		
	private:
		WriteEvent(const WriteEvent &ref);
		WriteEvent	&operator=(const WriteEvent &rhs);
	public:
		virtual void	callEventHandler(void) = 0;
		virtual void	onboardQueue(void) throw (std::exception) = 0;
		virtual void	offboardQueue(void) throw (std::exception) = 0;
	protected:
		const std::vector<char>	&getBuffer(void);
		std::vector<char>	&getBufferRef(void);
};

#endif