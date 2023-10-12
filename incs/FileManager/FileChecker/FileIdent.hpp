#ifndef FILEIDENT_HPP
# define FILEIDENT_HPP

#include <FileManager/FileState.hpp>
#include <iostream>
#include <stdio.h>
#include <Event/ReadEvent/ReadEventFromFile.hpp>
#include <Event/WriteEvent/WriteEventToFile.hpp>
// 상속받아서 해제 할떄 해당 state를 그 생성한 방식으로 변경할 수 있는것 구현 
class FileIdent
{
private:
    long _fileSize;
    std::string _path;
    e_file_content_syncro _state;
    bool    _haveToUpdate;
public:
    FileIdent(const std::string &path);
    ~FileIdent(void);
    const e_file_content_syncro &getState(void) const;
    long getFileSize(void);
    void syncTo(ReadEventFromFile *readEventFromFile, e_file_content_syncro targetState);
    void syncTo(WriteEventToFile *writeEventToFile,  e_file_content_syncro targetState);
private:
    void _updateFileSize();
    void _queryUpdateFileSize();
    bool _queryUpdateNeeded();
class FailToOpenException: public std::exception {
    public:
        virtual const char *what() const throw();
};
};

#endif