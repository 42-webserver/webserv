#include "../../../incs/Event/EventQueue/EventQueue.hpp"
#include <new>
#include <stddef.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include "../../../incs/Log/Logger.hpp"
#include <fcntl.h>
#if defined(__linux__)
#include <sys/epoll.h>
#endif

// 정적 멤버 초기화
EventQueue *EventQueue::_instance = NULL;

EventQueue::EventQueue(void) {
#if defined(__APPLE__) || defined(__FreeBSD__)
    // kqueue 기반 구현 (원래 코드)
    this->_fd = kqueue();
    if (this->_fd == -1) {
        throw (FailToCreateException());
    }
#elif defined(__linux__)
    // epoll 기반 구현
    this->_fd = epoll_create1(0);
    if (this->_fd == -1) {
        throw (FailToCreateException());
    }
#endif
}

EventQueue::~EventQueue(void) {
    if (this->_fd != -1)
        close(this->_fd);
}

EventQueue &EventQueue::getInstance(void) {
    if (EventQueue::_instance == NULL) {
        EventQueue::_instance = new EventQueue();
    }
    return (*EventQueue::_instance);
}

void EventQueue::deleteInstance(void) {
    if (EventQueue::_instance) {
        close(EventQueue::_instance->_fd);
        delete EventQueue::_instance;
        EventQueue::_instance = NULL;
    }
}

int EventQueue::getEventFd(int idx) const {
#if defined(__APPLE__) || defined(__FreeBSD__)
    return (this->_ev_list[idx].ident);
#elif defined(__linux__)
    // epoll_event: 파일 디스크립터는 data.fd에 저장됨
    return (this->_ev_list[idx].data.fd);
#endif
}

int EventQueue::getEventQueueFd(void) const {
    return (this->_fd);
}

Event *EventQueue::getEventData(int idx) const {
#if defined(__APPLE__) || defined(__FreeBSD__)
    return static_cast<Event *>(this->_ev_list[idx].udata);
#elif defined(__linux__)
    // 사용자 데이터는 data.ptr에 저장됨
    return static_cast<Event *>(this->_ev_list[idx].data.ptr);
#endif
}

event_t *EventQueue::getEventList(void) {
    return (this->_ev_list);
}

event_t *EventQueue::getEventSet(void) {
    return (&this->_ev_set);
}

event_t *EventQueue::getEventSetElementPtr(void) {
    return (&this->_ev_set);
}

int EventQueue::pullEvents(void) {
    int ret = 0;
#if defined(__APPLE__) || defined(__FreeBSD__)
    ret = kevent(this->_fd, NULL, 0, this->_ev_list, MAX_EVENTS, NULL);
#elif defined(__linux__)
    ret = epoll_wait(this->_fd, this->_ev_list, MAX_EVENTS, -1);
#endif
    if (ret == -1) {
        throw (FailToGetEventException());
    }
    return ret;
}

bool EventQueue::pushEvent(Event *event) {
    try {
        event->onboardQueue();
    } catch (const std::exception &e) {
        Logger::getInstance().error(e.what());
        return false;
    }
    return true;
}

bool EventQueue::popEvent(Event *event) {
    try {
        event->offboardQueue();
    } catch (const std::exception &e) {
        Logger::getInstance().error(e.what());
        return false;
    }
    return true;
}

// Exception 구현
const char *EventQueue::FailToCreateException::what(void) const throw() {
    return ("EventQueue: Fail to create");
}

const char *EventQueue::FailToControlException::what(void) const throw() {
    return ("EventQueue: Fail to control");
}

const char *EventQueue::FailToGetEventException::what(void) const throw() {
    return ("EventQueue: Fail to get event");
}

const char *EventQueue::TimeoutException::what(void) const throw() {
    return ("EventQueue: Timeout");
}
