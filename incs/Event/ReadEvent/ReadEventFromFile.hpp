#ifndef READEVENFROMFILE_HPP
# define READEVENFROMFILE_HPP

# include "ReadEvent.hpp"
# include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
# include <Channel/SingleStreamable.hpp>
# include <Channel/FileStream.hpp>
# include <Event/Exception/KqueueError.hpp>
# include <Event/ReadEvent/ReadEventFromFileHandler.hpp>
# include <FileManager/FileChecker/FileSyncOnDestruct.hpp>

class ReadEventFromFile: public ReadEvent, public SingleStreamable{
friend class FileIdent;
    public:
        ReadEventFromFile(
            IoReadAndWriteBuffer &buffer,
            const std::string &path, std::string mode);
        virtual	~ReadEventFromFile(void);

    public:
        virtual void	callEventHandler(void);
        virtual void	onboardQueue(void);
        virtual void	offboardQueue(void);
    private:
        FileSyncOnDestruct _syncObj;
        void               _sync(e_file_content_syncro *origin,
        e_file_content_syncro targetState, bool *haveToUpdate);
};

#endif