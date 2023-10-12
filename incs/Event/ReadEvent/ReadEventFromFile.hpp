#ifndef READEVENFROMFILE_HPP
# define READEVENFROMFILE_HPP

# include "ReadEvent.hpp"
# include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
# include <Channel/SingleStreamable.hpp>
# include <Channel/FileStream.hpp>
# include <Event/Exception/KqueueError.hpp>
# include <Event/ReadEvent/ReadEventFromFileHandler.hpp>

class ReadEventFromFile: public ReadEvent, public SingleStreamable{
    public:
        ReadEventFromFile(
            IoReadAndWriteBuffer &buffer,
            const std::string &path, std::string mode, e_syncro_state *state);
        virtual	~ReadEventFromFile(void);

    public:
        virtual void	callEventHandler(void);
        virtual void	onboardQueue(void);
        virtual void	offboardQueue(void);
    private:
        e_syncro_state *_state;
        void _notifyFinish(void);
};

#endif