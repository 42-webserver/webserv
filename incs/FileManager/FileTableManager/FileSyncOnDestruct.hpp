#ifndef FILESYNCRO_HPP
# define FILESYNCRO_HPP

#include <FileManager/FileState.hpp>
#include <iostream>

class WriteEventToFile;
class ReadEventFromFile;
class FileSyncOnDestruct
{
friend class WriteEventToFile;
friend class ReadEventFromFile;
private:
    e_file_content_syncro *_state;
    bool *_haveToUpdate;
    e_file_content_syncro _targetState;
public:  
    FileSyncOnDestruct(void);
    ~FileSyncOnDestruct(void);
private:
    FileSyncOnDestruct(const FileSyncOnDestruct &ref);
    FileSyncOnDestruct &operator=(const FileSyncOnDestruct &ref);
    void Sync(e_file_content_syncro *_origin,
    e_file_content_syncro targetState, bool *_haveToUpdate);
};
#endif