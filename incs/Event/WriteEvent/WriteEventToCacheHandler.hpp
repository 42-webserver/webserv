#ifndef WRITEEVENTTOCACHEHANDLER_HPP
# define WRITEEVENTTOCACHEHANDLER_HPP

# include "WriteEventHandler.hpp"
# include "../../../libs/shared_ptr/shared_ptr.hpp"
# include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
class WriteEventToCacheHandler: public WriteEventHandler{
    public:
        WriteEventToCacheHandler(ft::shared_ptr<IoReadAndWriteBuffer> buffer);
            virtual	~WriteEventToCacheHandler(void);

    public:
        virtual void   handleEvent(Event &event);
    private:
        ft::shared_ptr<IoReadAndWriteBuffer> _buffer;
};
#endif