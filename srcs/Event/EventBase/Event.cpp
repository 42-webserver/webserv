#include "../../../incs/Event/EventBase/Event.hpp"

Event::Event(EventHandler* eventHandler): _event_handler(ft::shared_ptr<EventHandler>(eventHandler)) {}
Event::~Event(void) {}

//Exception
const char	*Event::FailToOnboardException::what(void) const throw() { return ("Fail to onboard"); }
const char	*Event::FailToOffboardException::what(void) const throw() { return ("Fail to offboard"); }