#include "../../../incs/Event/ReadEvent/ReadEvent.hpp"
#if defined(__linux__)
  #include <sys/epoll.h>
#endif
#include "../../../incs/Log/Logger.hpp"
#include <stdexcept>
#include <unistd.h>

ReadEvent::ReadEvent(ReadEventHandler *readEventHandler)
    : Event(readEventHandler) {}

ReadEvent::~ReadEvent(void) {}

void ReadEvent::_onboardRead(Event *event, int fd) {
    EventQueue &event_queue = EventQueue::getInstance();

#if defined(__APPLE__) || defined(__FreeBSD__)
    // kqueue 방식
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
#elif defined(__linux__)
    // epoll 방식
    struct epoll_event ev;
    ev.events = EPOLLIN;  // 읽기 이벤트
    ev.data.ptr = event;
    if (epoll_ctl(event_queue.getEventQueueFd(), EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw (KqueueError());
    }
#endif
}

void ReadEvent::_offboardRead(Event *event, int fd) {
    EventQueue &event_queue = EventQueue::getInstance();

#if defined(__APPLE__) || defined(__FreeBSD__)
    // kqueue 방식
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
#elif defined(__linux__)
    // epoll 방식
    if (epoll_ctl(event_queue.getEventQueueFd(), EPOLL_CTL_DEL, fd, NULL) == -1) {
        throw (KqueueError());
    }
    delete event;
#endif
}
