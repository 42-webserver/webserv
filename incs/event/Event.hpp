#ifndef EVENT_HPP
# define EVENT_HPP

# include "../../libs/shared_ptr/incs/shared_ptr.hpp"
# include "../../libs/unique_ptr/unique_ptr.hpp"
# include "../socket/Socket.hpp"
# include "../../libs/HandlerChain/HandlerChain.hpp"
# include "../../incs/file/file.hpp"

class Event;
class EventHandler {
public:
    virtual ~EventHandler();  // Virtual destructor
    virtual void handleEvent(Event& event) = 0;
};
class Event{
protected:
    ft::shared_ptr<FdClass> _connection;
    ft::unique_ptr<EventHandler> _eventHandler;

public:
    Event(int fd, EventHandler* eventHandler);

    virtual ~Event();// Virtual destructor

    virtual void callEventHandler() = 0;

private:
    Event(const Event&);
    Event& operator=(const Event&);
};


class EventFactory {
public:
    virtual ft::unique_ptr<Event> createEvent() = 0;
};


#endif