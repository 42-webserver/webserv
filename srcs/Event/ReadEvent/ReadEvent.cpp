#include "../../../incs/Event/ReadEvent/ReadEvent.hpp"


// ReadEvent::ReadEvent(int fd, EventHandler *read_event_handler): 
// Event(fd, read_event_handler), _buffer(ft::bufSize) {}
ReadEvent::ReadEvent(ReadEventHandler *readEventHandler):
Event(readEventHandler){}
ReadEvent::~ReadEvent(void) {}
void ReadEvent::_onboardRead(Event *event, int fd) {
    EventQueue &event_queue = EventQueue::getInstance();

    EV_SET(
        event_queue.getEventSetElementPtr(),
        fd,
        EVFILT_READ,
        EV_ADD | EV_ENABLE,
        0,
        0,
        event
    );

    if (kevent(event_queue.getEventQueueFd(), 
    event_queue.getEventSet(), 
    1, NULL, 0, NULL) == -1) {
        throw (KqueueError());
    }
}

void ReadEvent::_offboardRead(Event *event, int fd) {
    EventQueue &event_queue = EventQueue::getInstance();

    EV_SET(
        event_queue.getEventSetElementPtr(),
        fd,
        EVFILT_READ,
        EV_DELETE,
        0,
        0,
        event
    );

    if (kevent(event_queue.getEventQueueFd(), 
    event_queue.getEventSet(), 
    1, NULL, 0, NULL) == -1) {
        throw (KqueueError());
    }
    delete event;
}