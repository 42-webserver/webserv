#ifndef CGIKILLEVENT_HPP
# define CGIKILLEVENT_HPP

#include <Event/EventBase/Event.hpp>
#include <Event/SpecialEvent/CgiKillEventHandler.hpp>

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
};

#endif