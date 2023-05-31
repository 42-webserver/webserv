#ifndef EVENT_HPP
# define EVENT_HPP

# include "../../libs/shared_ptr/incs/shared_ptr.hpp"
# include "../../libs/unique_ptr/unique_ptr.hpp"
# include "../socket/Socket.hpp"
# include "../../incs/file/file.hpp"
# include "../../incs/EventQueue/EventQueue.hpp"
class EventQueue;
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
    virtual void onboardQueue() throw (std::exception)= 0;
    virtual void offboardQueue() throw (std::exception)= 0;
    const int &getConnectionFd() const;
private:
    Event(const Event&);
    Event& operator=(const Event&);
	public:
		class FailToOnboardException: public std::exception {
			public:
				virtual const char *what() const throw();
		};
        class FailToOffboardException: public std::exception {
            public:
                virtual const char *what() const throw();
        };
};

class EventFactory {
public:
    EventFactory();
    virtual ~EventFactory(); 
    virtual Event* createEvent(int fd) = 0;

private:
    EventFactory(const EventFactory&);
    EventFactory& operator=(const EventFactory&);
};

#endif