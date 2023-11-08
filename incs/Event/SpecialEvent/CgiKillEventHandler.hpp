#ifndef CGIKILLEVENTHANDLER_HPP
# define CGIKILLEVENTHANDLER_HPP

#include <Event/EventBase/EventHandler.hpp>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

class CgiKillEventHandler: public EventHandler {
    public:
        CgiKillEventHandler(void);
        virtual	~CgiKillEventHandler(void);

    private:
        CgiKillEventHandler(const CgiKillEventHandler &ref);
        CgiKillEventHandler	&operator=(const CgiKillEventHandler &rhs);
    public:
        virtual void	handleEvent(Event &event);
};

#endif