#ifndef CGIWAITEVENTHANDLER_HPP
# define CGIWAITEVENTHANDLER_HPP

#include <Event/EventBase/EventHandler.hpp>
#include <unistd.h>

class CgiWaitEventHandler: public EventHandler {
    public:
        CgiWaitEventHandler(void);
        virtual	~CgiWaitEventHandler(void);

    private:
        CgiWaitEventHandler(const CgiWaitEventHandler &ref);
        CgiWaitEventHandler	&operator=(const CgiWaitEventHandler &rhs);

    public:
        virtual void	handleEvent(Event &event);
};

#endif