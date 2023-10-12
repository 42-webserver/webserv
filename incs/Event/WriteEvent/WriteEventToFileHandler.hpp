#ifndef WRITEEVENTTOFILEHANDLER_HPP
# define WRITEEVENTTOFILEHANDLER_HPP

# include "WriteEventHandler.hpp"
# include <FileManager/Syncro.hpp>
# include "../../../libs/shared_ptr/shared_ptr.hpp"
# include <Buffer/Exception/DisconnectionException.hpp>


class WriteEventToFileHandler: public WriteEventHandler{
    public:
        WriteEventToFileHandler(IoReadAndWriteBuffer &buffer);
            virtual	~WriteEventToFileHandler(void);

    public:
        virtual void   handleEvent(Event &event);
    private:
        IoReadAndWriteBuffer      &_buffer;
};
#endif