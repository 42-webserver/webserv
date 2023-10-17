#ifndef WRITEEVENTTOFILE_HPP
# define WRITEEVENTTOFILE_HPP

# include "WriteEvent.hpp"
# include <Channel/SingleStreamable.hpp>
# include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
# include <Channel/FileStream.hpp>
# include <Event/Exception/KqueueError.hpp>
# include <Event/WriteEvent/WriteEventToFileHandler.hpp>

class WriteEventToFile: public WriteEvent, public SingleStreamable{
friend class FileIdent;   
    public:
        WriteEventToFile(
            ft::shared_ptr<IoReadAndWriteBuffer> buffer,
            const std::string &path, std::string mode);
        virtual	~WriteEventToFile(void);

    public:
        virtual void	callEventHandler(void);
        virtual void	onboardQueue(void);
        virtual void	offboardQueue(void);
    private:
        //To do syncroFileDataAndWriter
};

#endif