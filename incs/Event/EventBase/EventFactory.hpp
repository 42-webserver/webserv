#ifndef EVENTFACTORY_HPP
# define EVENTFACTORY_HPP

# include "../EventDto/EventDto.hpp"
# include "./Event.hpp"

class EventFactory {
	public:
		EventFactory(void);
		virtual ~EventFactory(void);

	private:
		EventFactory(const EventFactory &ref);
		EventFactory	&operator=(const EventFactory &rhs);
	
	public:
		virtual Event	*createEvent(const EventDto &EventDto) const = 0;
};
#endif