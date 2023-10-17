#ifndef WRITEEVENTTOCACHEHANDLER_HPP
# define WRITEEVENTTOCACHEHANDLER_HPP

# include "WriteEventHandler.hpp"
# include "../../../libs/shared_ptr/shared_ptr.hpp"

class WriteEventToCacheHandler: public WriteEventHandler{
    public:
        WriteEventToCacheHandler(std::vector<char> &content);
            virtual	~WriteEventToCacheHandler(void);

    public:
        virtual void   handleEvent(Event &event);
    private:
        std::vector<char> &_content;
        size_t _offset;
};
#endif