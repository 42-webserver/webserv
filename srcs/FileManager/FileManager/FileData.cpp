#include <FileManager/FileManager/FileData.hpp>

SyncroFileDataAndReader::SyncroFileDataAndReader(FileData &fileData) : _fileData(fileData)
{
    this->_fileData.increaseReaderCount();
}

SyncroFileDataAndReader::~SyncroFileDataAndReader()
{
    this->_fileData.decreaseReaderCount();
}

FileData::FileData(void) : _fileProcessingType(NoneProcessing), _readerCount(0) {}

FileData::FileData(const FileData &ref) : _fileProcessingType(ref._fileProcessingType), _readerCount(ref._readerCount) {}

FileData &FileData::operator=(const FileData &rhs)
{
    if (this != &rhs)
    {
        this->_fileProcessingType = rhs._fileProcessingType;
        this->_readerCount = rhs._readerCount;
    }
    return (*this);
}

FileData::~FileData(void) {}

void FileData::decreaseReaderCount(void)
{
    if (this->_readerCount > 0)
        --this->_readerCount;
    if (this->_readerCount == 0)
        this->_fileProcessingType = NoneProcessing;
}

void FileData::increaseReaderCount(void)
{
    if (this->_readerCount == 0)
        this->_fileProcessingType = ReadingProcessing;
    ++this->_readerCount;
}

ft::shared_ptr<SyncroFileDataAndReader> FileData::buildSyncroFileDataAndReader(void)
{
    return (ft::shared_ptr<SyncroFileDataAndReader>(new SyncroFileDataAndReader(*this)));
}

e_FileProcessingType FileData::getFileProcessingType(void)
{
    return (this->_fileProcessingType);
}