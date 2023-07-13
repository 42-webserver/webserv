#ifndef EVENTFACTORY_HPP
# define EVENTFACTORY_HPP

# include "../EventDto/EventDto.hpp"
# include "./Event.hpp"
# include "../../FtUtil/ft.hpp"
class EventFactory {
	
	public:
	class FailToEventCreateException: public std::exception {
		public:
			const char *what() const throw();
	};
	public:
		static EventFactory	&getInstance(void);
		void DeleteInstance(void);
	private:
		EventFactory(const EventFactory &ref);
		EventFactory	&operator=(const EventFactory &rhs);
		EventFactory(void);
		virtual ~EventFactory(void);
		static EventFactory	*_instance;
	public:
		Event	*createEvent(ft::EventType eventType, EventDto &EventDto) throw (FailToEventCreateException);
};
#endif