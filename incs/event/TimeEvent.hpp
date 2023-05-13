#ifndef TIMEEVENT_HPP
# define TIMEEVENT_HPP

# include "Event.hpp"

class TimeEvent: public Event {
	public:
		TimeEvent(void);
		TimeEvent(const TimeEvent &ref);
		virtual	~TimeEvent(void);
		TimeEvent	&operator=(const TimeEvent &rhs);
};

#endif