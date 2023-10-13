#ifndef SYNCRO_HPP
# define SYNCRO_HPP

typedef enum {
    SyncroReady,
    SyncroProcessing,
    SyncroFinish
}   e_local_syncro_state;


typedef enum{
    Init,
    InCache,
    InsertingCache,
    NotUsed,
    ReadingFile,
    WritingFile,
    SyncronizedReadingFile,
    
}   e_file_content_syncro;

#endif