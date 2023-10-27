#ifndef WRITEEVENTTOCGIHANDLER_HPP
# define WRITEEVENTTOCGIHANDLER_HPP

# include "WriteEventHandler.hpp"

class WriteEventToCgi;

class WriteEventToCgiHandler : public WriteEventHandler {
    public:
        WriteEventToCgiHandler();
        virtual ~WriteEventToCgiHandler(void);
        virtual void handleEvent(Event &event);
    private:
        WriteEventToCgiHandler(const WriteEventToCgiHandler &ref);
        WriteEventToCgiHandler &operator=(const WriteEventToCgiHandler &ref);
};

#endif