#ifndef READFROMCACHEHANDLER_HPP
# define READFROMCACHEHANDLER_HPP

# include "ReadEventHandler.hpp"

class ReadEventFromCacheHandler: public ReadEventHandler{
    public:
        ReadEventFromCacheHandler(std::vector<char> &content);
            virtual	~ReadEventFromCacheHandler(void);

    public:
        virtual void   handleEvent(Event &event);
    private:
        std::vector<char> &_content;
        size_t _offset;
};
#endif