#ifndef SYNCRO_HPP
# define SYNCRO_HPP


//왜 있는지 모르겠는데 일단 넣어둠
typedef enum {
    SyncroReady,
    SyncroProcessing,
    SyncroFinish
}   e_local_syncro_state;


typedef enum{
    Init,
    InCache,
    InsertingCache,
    UpdatingCache,
    NotUsed,
    ReadingFile,
    WritingFile,
    InBuffer,
    eraseProcessing,
    sharedProcessing,
    deleted  
}   e_file_content_syncro;

#endif