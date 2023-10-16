#ifndef SYNCRO_HPP
# define SYNCRO_HPP

typedef enum {
    Waiting,
    OkNext,
    InvalidArgument,
    deletedFile
}   e_file_msg;


typedef enum{
    Init,
    InCache,
    InsertingCache,
    UpdatingCache,
    ReadingFile,
    WritingFile,
    InBuffer,
    BufferFlushed,
    eraseProcessing,
    sharedProcessing,
    deleted  
}   e_file_content_syncro;

#endif