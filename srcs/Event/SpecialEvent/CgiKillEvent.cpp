#include <Event/SpecialEvent/CgiKillEvent.hpp>
#include <Event/EventQueue/EventQueue.hpp>
#include <Event/Exception/KqueueError.hpp>

CgiKillEvent::CgiKillEvent(pid_t cgiPid): Event(new CgiKillEventHandler()), _cgiPid(cgiPid) {}
CgiKillEvent::~CgiKillEvent(void) {}

pid_t CgiKillEvent::getCgiPid(void) {
    return (this->_cgiPid);
}

void CgiKillEvent::callEventHandler(void) {
    this->_event_handler->handleEvent(*this);
}

void CgiKillEvent::onboardQueue(void) {
    EventQueue& eventQueue = EventQueue::getInstance();
    Event *event = this;
        
        EV_SET(
            eventQueue.getEventSetElementPtr(),
            1,
            EVFILT_TIMER,
            EV_ADD | EV_ENABLE,
            0,
            3 * 60 * 1000,
            // 3 * 1000,
            event
        );
        if (kevent(eventQueue.getEventQueueFd(),
            eventQueue.getEventSet(),
            1, NULL, 0, NULL) == -1) {
            throw (KqueueError());
        }
}

void CgiKillEvent::offboardQueue(void) {
    EventQueue& eventQueue = EventQueue::getInstance();
    Event *event = this;
        // EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 3 * 60 * 1000, 0); // 3분 = 3 * 60 * 1000ms
        EV_SET(
            eventQueue.getEventSetElementPtr(),
            1,
            EVFILT_TIMER,
            EV_DELETE,
            0,
            // 3 * 60 * 1000,
            3 * 1000,
            event
        );
        if (kevent(eventQueue.getEventQueueFd(),
            eventQueue.getEventSet(),
            1, NULL, 0, NULL) == -1) {
            throw (KqueueError());
        }
        delete event;
}