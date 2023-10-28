#ifndef LOGEVENTHANDLER_HPP
# define LOGEVENTHANDLER_HPP

#include <Event/EventBase/EventHandler.hpp>

class LogEventHandler: public EventHandler {
    public:
        LogEventHandler(void);
        virtual	~LogEventHandler(void);

    private:
        LogEventHandler(const LogEventHandler &ref);
        LogEventHandler	&operator=(const LogEventHandler &rhs);
    public:
        virtual void	handleEvent(Event &event);
};
#endif