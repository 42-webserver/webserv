#ifndef EVENT_HPP
# define EVENT_HPP

# include "../../libs/shared_ptr/incs/shared_ptr.hpp"
# include "../socket/Socket.hpp"
# include "../../libs/HandlerChain/HandlerChain.hpp"

class ConnectionFd;
class EventHandler;

class Event{
protected:
    ConnectionFd* connection;
    EventHandler* eventHandler;

public:
    Event(ConnectionFd* conn, EventHandler* handler)
        : connection(conn), eventHandler(handler) {}

    virtual ~Event() {}  // Virtual destructor

    virtual void callEventHandler() = 0;

private:
    // Disable copy constructor and copy assignment operator
    Event(const Event&);
    Event& operator=(const Event&);
};

class EventHandler {
public:
    virtual ~EventHandler() {}  // Virtual destructor


};






#endif