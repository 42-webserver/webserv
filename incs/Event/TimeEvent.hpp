#ifndef TIMEEVENT_HPP
# define TIMEEVENT_HPP

# include "Event.hpp"

class TimeEvent: public Event {
	public:
		TimeEvent(void);
		virtual	~TimeEvent(void);
	
	private:
		TimeEvent(const TimeEvent &ref);
		TimeEvent	&operator=(const TimeEvent &rhs);
};

#endif