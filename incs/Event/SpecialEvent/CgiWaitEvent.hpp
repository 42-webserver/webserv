#ifndef CGIWAITEVENT_HPP
# define CGIWAITEVENT_HPP

#include <Event/EventBase/Event.hpp>
#include <unistd.h>
#include <Event/SpecialEvent/CgiWaitEventHandler.hpp>
#include <Event/EventQueue/EventQueue.hpp>
# include <Event/Exception/KqueueError.hpp>

class CgiWaitEvent: public Event {
    public:
        CgiWaitEvent(pid_t cgiPid);
        virtual	~CgiWaitEvent(void);

    private:
        CgiWaitEvent(const CgiWaitEvent &ref);
        CgiWaitEvent	&operator=(const CgiWaitEvent &rhs);
    public:
        pid_t getCgiPid(void) const;
    private:
        pid_t _cgiPid;
    public:
        virtual void	callEventHandler(void);
        virtual void	onboardQueue(void);
        virtual void	offboardQueue(void);
};
#endif