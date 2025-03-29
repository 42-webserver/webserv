#ifndef CGIKILLEVENT_HPP
# define CGIKILLEVENT_HPP

#include <Event/EventBase/Event.hpp>
#include <Event/SpecialEvent/CgiKillEventHandler.hpp>
#if defined(__linux__)
  #include <sys/timerfd.h>
#endif
class CgiKillEvent: public Event {
    public:
        CgiKillEvent(pid_t cgiPid);
        virtual	~CgiKillEvent(void);

    private:
        CgiKillEvent(const CgiKillEvent &ref);
        CgiKillEvent	&operator=(const CgiKillEvent &rhs);
    public:
        virtual void	callEventHandler(void);
        virtual void	onboardQueue(void);
        virtual void	offboardQueue(void);
        pid_t getCgiPid(void);

    private:
        pid_t _cgiPid;
    #if defined(__linux__)
        int _timerFd; // Linux 전용: timerfd를 저장하기 위한 멤버
    #endif
};

#endif