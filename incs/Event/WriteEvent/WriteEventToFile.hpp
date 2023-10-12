#ifndef WRITEEVENTTOFILE_HPP
# define WRITEEVENTTOFILE_HPP

# include "WriteEvent.hpp"
# include <Channel/SingleStreamable.hpp>
# include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
# include <Channel/FileStream.hpp>
# include <Event/Exception/KqueueError.hpp>
# include <Event/WriteEvent/WriteEventToFileHandler.hpp>

class WriteEventToFile: public WriteEvent, public SingleStreamable{
    public:
        WriteEventToFile(
            IoReadAndWriteBuffer &buffer,
            const std::string &path, std::string mode, e_syncro_state *state);
        virtual	~WriteEventToFile(void);

    public:
        virtual void	callEventHandler(void);
        virtual void	onboardQueue(void);
        virtual void	offboardQueue(void);
    private:
        e_syncro_state *_state;
        void _notifyFinish(void);
};

#endif