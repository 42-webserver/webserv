#include "../../../incs/Event/WriteEvent/WriteEvent.hpp"

// To do: implement WriteEvent

WriteEvent::WriteEvent(WriteEventHandler* writeEventHandler): Event(writeEventHandler) {}
WriteEvent::~WriteEvent(void) {}
void WriteEvent::_onboardWrite(Event *event, int fd) {
    EventQueue &event_queue = EventQueue::getInstance();

    EV_SET(
        event_queue.getEventSetElementPtr(),
        fd,
        EVFILT_WRITE,
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
void WriteEvent::_offboardWrite(Event *event, int fd) {
    EventQueue &event_queue = EventQueue::getInstance();

    EV_SET(
        event_queue.getEventSetElementPtr(),
        fd,
        EVFILT_WRITE,
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
