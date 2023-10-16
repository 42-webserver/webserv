#ifndef FILEIDENT_HPP
# define FILEIDENT_HPP

#include <FileManager/FileState.hpp>
#include <iostream>
#include <stdio.h>
#include <Event/ReadEvent/ReadEventFromFile.hpp>
#include <Event/WriteEvent/WriteEventToFile.hpp>
#include <unistd.h>
#include <sys/stat.h>

typedef enum{
    normal,
    directory
}   e_file_type;
class FileIdent
{
private:
    long _fileSize;
    std::string _path;
    e_file_content_syncro _state;
    bool    _haveToUpdate;
    e_file_type _fileType;
public:
    FileIdent(const std::string &path);
    FileIdent(const FileIdent &ref);
    FileIdent &operator=(const FileIdent &rhs);
    FileIdent(void);
    ~FileIdent(void);
    const e_file_content_syncro &getState(void) const;
    long getFileSize(void);
    const e_file_type &getFileType(void) const;
    void syncTo(ReadEventFromFile *readEventFromFile, e_file_content_syncro targetState);
    void syncTo(WriteEventToFile *writeEventToFile,  e_file_content_syncro targetState);
private:
    void _updateFileSize();
    void _queryUpdateFileSize();
    bool _queryUpdateNeeded();
    e_file_type _queryFileType();
class FailToOpenException: public std::exception {
    public:
        virtual const char *what() const throw();
};
};

#endif