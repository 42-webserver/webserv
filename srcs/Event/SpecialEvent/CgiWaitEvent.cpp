#include <Event/SpecialEvent/CgiWaitEvent.hpp>
#if defined(__APPLE__) || defined(__FreeBSD__)
  #include <sys/event.h>
#elif defined(__linux__)
  #include <sys/wait.h>
  #include <signal.h>
  #include <map>
  #include <errno.h>
  #include <cstdio>
  #include <cstdlib>
#endif
#include "../../../incs/Log/Logger.hpp"
#include <unistd.h>
#include <stdexcept>

// Linux 전용: CGI 이벤트를 추적할 전역 맵과 SIGCHLD 핸들러 설치 여부
#if defined(__linux__)
static std::map<pid_t, CgiWaitEvent*> s_cgiEventMap;
static bool s_sigchld_handler_installed = false;

static void sigchld_handler(int signo) {
    (void)signo;
    int saved_errno = errno;
    pid_t pid;
    int status;
    // 모든 종료된 자식 프로세스를 재검사
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
         std::map<pid_t, CgiWaitEvent*>::iterator it = s_cgiEventMap.find(pid);
         if (it != s_cgiEventMap.end()) {
              CgiWaitEvent* event = it->second;
              event->callEventHandler();
              s_cgiEventMap.erase(it);
         }
    }
    errno = saved_errno;
}
#endif

CgiWaitEvent::CgiWaitEvent(pid_t cgiPid)
    : Event(new CgiWaitEventHandler()), _cgiPid(cgiPid)
{}

CgiWaitEvent::~CgiWaitEvent(void)
{}

pid_t CgiWaitEvent::getCgiPid(void) {
    return this->_cgiPid;
}

void CgiWaitEvent::callEventHandler(void) {
    this->_event_handler->handleEvent(*this);
}

void CgiWaitEvent::onboardQueue(void) {
#if defined(__APPLE__) || defined(__FreeBSD__)
    // kqueue 기반 구현
    EventQueue &eventQueue = EventQueue::getInstance();
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
#elif defined(__linux__)
    // Linux에서는 epoll에서 프로세스 이벤트를 지원하지 않으므로 SIGCHLD를 사용합니다.
    // SIGCHLD 핸들러를 설치 (한 번만 설치)
    if (!s_sigchld_handler_installed) {
         struct sigaction sa;
         sa.sa_handler = sigchld_handler;
         sigemptyset(&sa.sa_mask);
         sa.sa_flags = SA_RESTART;
         if (sigaction(SIGCHLD, &sa, NULL) == -1) {
             throw std::runtime_error("Failed to install SIGCHLD handler");
         }
         s_sigchld_handler_installed = true;
    }
    // 이 이벤트를 전역 맵에 등록하여 SIGCHLD 핸들러가 처리할 수 있도록 함
    s_cgiEventMap[this->_cgiPid] = this;
#endif
}

void CgiWaitEvent::offboardQueue(void) {
#if defined(__APPLE__) || defined(__FreeBSD__)
    EventQueue &eventQueue = EventQueue::getInstance();
    Event *event = this;
    EV_SET(
        eventQueue.getEventSetElementPtr(),
        _cgiPid,
        EVFILT_PROC,
        EV_DELETE,
        0,
        0,
        event
    );
    if (kevent(eventQueue.getEventQueueFd(),
               eventQueue.getEventSet(),
               1, NULL, 0, NULL) == -1) {
        throw (KqueueError());
    }
    delete this;
#elif defined(__linux__)
    // Linux: 이벤트를 전역 맵에서 제거하고 메모리 해제
    std::map<pid_t, CgiWaitEvent*>::iterator it = s_cgiEventMap.find(this->_cgiPid);
    if (it != s_cgiEventMap.end()) {
         s_cgiEventMap.erase(it);
    }
    delete this;
#endif
}
