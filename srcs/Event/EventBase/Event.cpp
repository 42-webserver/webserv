#include "../../../incs/Event/EventBase/Event.hpp"

Event::Event(ft::shared_ptr<Channel> channel, 
EventHandler* eventHandler): _channel(channel), _event_handler(ft::shared_ptr<EventHandler>(eventHandler)) {}
Event::~Event(void) {}

const ft::shared_ptr<Channel>	&Event::getChannel(void) { return (this->_channel); }
int	Event::getFd(void) const { return (this->_channel->getFd()); }

//Exception
const char	*Event::FailToOnboardException::what(void) const throw() { return ("Fail to onboard"); }
const char	*Event::FailToOffboardException::what(void) const throw() { return ("Fail to offboard"); }