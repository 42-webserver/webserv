#ifndef READEVENTFROMFILEHANDLER_HPP
# define READEVENTFROMFILEHANDLER_HPP

# include "ReadEventHandler.hpp"
# include <FileManager/FileState.hpp>
# include "../../../libs/shared_ptr/shared_ptr.hpp"

class ReadEventFromFileHandler: public ReadEventHandler{
    public:
        ReadEventFromFileHandler(IoReadAndWriteBuffer &buffer);
            virtual	~ReadEventFromFileHandler(void);

    public:
        virtual void   handleEvent(Event &event);
    private:
        IoReadAndWriteBuffer      &_buffer;
};
#endif