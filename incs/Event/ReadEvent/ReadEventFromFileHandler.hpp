#ifndef READEVENTFROMFILEHANDLER_HPP
# define READEVENTFROMFILEHANDLER_HPP

# include "ReadEventHandler.hpp"
# include <FileManager/Syncro.hpp>
# include "../../../libs/shared_ptr/shared_ptr.hpp"

class ReadEventFromFileHandler: public ReadEventHandler{
    public:
        ReadEventFromFileHandler(ft::shared_ptr<e_syncro_state> state,
            IoReadAndWriteBuffer &buffer);
            virtual	~ReadEventFromFileHandler(void);

    public:
        virtual void   handleEvent(Event &event);
    private:
        ft::shared_ptr<e_syncro_state>  _state;
        IoReadAndWriteBuffer      &_buffer;
        void    _notifyFinish();
};
#endif