#ifndef EVENT_HPP
# define EVENT_HPP

# include "../../libs/shared_ptr/incs/shared_ptr.hpp"
# include "../../libs/unique_ptr/unique_ptr.hpp"
# include "../socket/Socket.hpp"
# include "../../libs/HandlerChain/HandlerChain.hpp"
# include "../../incs/file/file.hpp"
# include "../../incs/kqueue/Kqueue.hpp"
class Kqueue;
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
    ft::shared_ptr<Kqueue> _kqueue;
public:
    Event(int fd, EventHandler* eventHandler, ft::shared_ptr<Kqueue> kqueue);

    virtual ~Event();// Virtual destructor

    virtual void callEventHandler() = 0;

private:
    Event(const Event&);
    Event& operator=(const Event&);
};

class EventFactory {
public:
    EventFactory();
    virtual ~EventFactory(); 
    virtual Event* createEvent(int fd, ft::shared_ptr<Kqueue> kqueue) = 0;

private:
    EventFactory(const EventFactory&);
    EventFactory& operator=(const EventFactory&);
};

#endif