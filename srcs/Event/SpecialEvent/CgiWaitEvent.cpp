#include <Event/SpecialEvent/CgiWaitEvent.hpp>

CgiWaitEvent::CgiWaitEvent(pid_t cgiPid): Event(new CgiWaitEventHandler()), _cgiPid(cgiPid) {}
CgiWaitEvent::~CgiWaitEvent(void) {}

pid_t CgiWaitEvent::getCgiPid(void)  { return this->_cgiPid; }

void CgiWaitEvent::callEventHandler(void) {
    this->_event_handler->handleEvent(*this);
}
void CgiWaitEvent::onboardQueue(void) { 

    EventQueue& eventQueue = EventQueue::getInstance();
    Event *event = this;
    EV_SET(
        eventQueue.getEventSetElementPtr(),
        _cgiPid,
        EVFILT_PROC,
        EV_ADD | EV_ENABLE,
        NOTE_EXIT,
        0,
        event
    );   

    if (kevent(eventQueue.getEventQueueFd(),
    eventQueue.getEventSet(),
    1, NULL, 0, NULL) == -1) {
        throw (KqueueError());
    }
}

void CgiWaitEvent::offboardQueue(void) {
    Event *event = this;

    delete event;
}