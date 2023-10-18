#ifndef FILEDATA_HPP
# define FILEDATA_HPP

#include "../../../libs/shared_ptr/shared_ptr.hpp"

typedef enum{
    NoneProcessing,
    ReadingProcessing,
    WritingProcessing
}   e_FileProcessingType;

class FileData;

class SyncroFileDataAndReader{
public:
    SyncroFileDataAndReader(FileData &fileData);
    ~SyncroFileDataAndReader();
private:
    FileData &_fileData;
};

class SyncroFileDataAndWriter{
public:
    SyncroFileDataAndWriter(FileData &fileData);
    ~SyncroFileDataAndWriter();
private:
    FileData &_fileData;
};
class FileData{
private:
    e_FileProcessingType _fileProcessingType;
    int _readerCount;
public:
    FileData(void);
    FileData(const FileData &ref);
    FileData &operator=(const FileData &rhs);
    ~FileData(void);
public:
    void decreaseReaderCount(void);
    void increaseReaderCount(void);
    void onWriting(void);
    void offWriting(void);
    ft::shared_ptr<SyncroFileDataAndReader> buildSyncroFileDataAndReader(void);
    ft::shared_ptr<SyncroFileDataAndWriter> buildSyncroFileDataAndWriter(void);
    e_FileProcessingType getFileProcessingType(void);
};

#endif