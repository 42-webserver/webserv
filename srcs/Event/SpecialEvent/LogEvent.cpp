#include <Event/SpecialEvent/LogEvent.hpp>
#include <Log/Logger.hpp>
#include <Event/EventQueue/EventQueue.hpp>
#include <Event/Exception/KqueueError.hpp>

LogEvent::LogEvent(void): Event(new LogEventHandler()) {}
LogEvent::~LogEvent(void) {}

void LogEvent::callEventHandler(void) { 

    this->_event_handler->handleEvent(*this);
}

void LogEvent::onboardQueue(void) {
    EventQueue& eventQueue = EventQueue::getInstance();
    Event *event = this;
        // EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 3 * 60 * 1000, 0); // 3분 = 3 * 60 * 1000ms
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

void LogEvent::offboardQueue(void) {
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