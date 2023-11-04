#ifndef READFROMCACHEHANDLER_HPP
# define READFROMCACHEHANDLER_HPP

# include "ReadEventHandler.hpp"

class ReadEventFromCacheHandler: public ReadEventHandler{
    public:
        ReadEventFromCacheHandler(std::vector<char> &content, size_t fileSize);
            virtual	~ReadEventFromCacheHandler(void);

    public:
        virtual void   handleEvent(Event &event);
    private:
        std::vector<char> &_content;
        size_t _fileSize;
        size_t _offset;
};
#endif