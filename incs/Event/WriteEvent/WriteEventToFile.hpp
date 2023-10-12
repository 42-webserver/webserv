#ifndef WRITEEVENTTOFILE_HPP
# define WRITEEVENTTOFILE_HPP

# include "WriteEvent.hpp"
# include <Channel/SingleStreamable.hpp>
# include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
# include <Channel/FileStream.hpp>
# include <Event/Exception/KqueueError.hpp>
# include <Event/WriteEvent/WriteEventToFileHandler.hpp>
#include <FileManager/FileChecker/FileSyncOnDestruct.hpp>

class WriteEventToFile: public WriteEvent, public SingleStreamable{
friend class FileIdent;   
    public:
        WriteEventToFile(
            IoReadAndWriteBuffer &buffer,
            const std::string &path, std::string mode);
        virtual	~WriteEventToFile(void);

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