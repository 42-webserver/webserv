#ifndef LOGEVENT_HPP
# define LOGEVENT_HPP
#if defined(__linux__)
#include <sys/timerfd.h>
#endif
#include <Event/EventBase/Event.hpp>
#include <Event/SpecialEvent/LogEventHandler.hpp>
class LogEvent: public Event {
    public:
        LogEvent(void);
        virtual	~LogEvent(void);

    private:
        LogEvent(const LogEvent &ref);
        LogEvent	&operator=(const LogEvent &rhs);
    public:
        virtual void	callEventHandler(void);
        virtual void	onboardQueue(void);
        virtual void	offboardQueue(void);
    #if defined(__linux__)
    private:
            int _timerFd; // Linux에서 timerfd를 사용하기 위한 멤버 변수
    #endif
};
#endif