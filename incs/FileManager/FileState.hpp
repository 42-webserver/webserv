#ifndef SYNCRO_HPP
# define SYNCRO_HPP


//왜 있는지 모르겠는데 일단 넣어둠
typedef enum {
    OkGood,
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