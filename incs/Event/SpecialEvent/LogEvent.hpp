#ifndef LOGEVENT_HPP
# define LOGEVENT_HPP

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
};
#endif