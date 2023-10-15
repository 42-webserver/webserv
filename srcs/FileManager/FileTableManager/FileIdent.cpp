#include <FileManager/FileTableManager/FileIdent.hpp>

FileIdent::~FileIdent(void) {}
FileIdent::FileIdent(const std::string &path) : _path(path), _state(Init), _haveToUpdate(true) {
    try {
        this->_fileType = _queryFileType();
    } catch (const std::exception &e) {
        throw ;
    }
}
FileIdent::FileIdent(void) : _path(""), _state(Init), _haveToUpdate(true) {
}
FileIdent::FileIdent(const FileIdent &ref) {
    *this = ref;
}
FileIdent &FileIdent::operator=(const FileIdent &rhs) {
    if (this != &rhs) {
        this->_fileSize = rhs._fileSize;
        this->_path = rhs._path;
        this->_state = rhs._state;
        this->_haveToUpdate = rhs._haveToUpdate;
    }
    return (*this);
}

void FileIdent::_updateFileSize() {
    struct stat fileInfo;

    if (stat(this->_path.c_str(), &fileInfo) != 0) {
        throw FileIdent::FailToOpenException();
    } else {
        this->_fileSize = fileInfo.st_size;
    }
}
bool FileIdent::_queryUpdateNeeded() {
    bool ret = false;
    if (_haveToUpdate) {
        ret = true;
        _haveToUpdate = false;
    }
    return (ret);
}
e_file_type FileIdent::_queryFileType() {
    struct stat fileInfo;

    if (stat(this->_path.c_str(), &fileInfo) != 0) {
        throw FileIdent::FailToOpenException();
    } else {
        if (S_ISDIR(fileInfo.st_mode)) {
            return (directory);
        } else {
            return (normal);
        }
    }
}
void FileIdent::_queryUpdateFileSize() {
    if (_queryUpdateNeeded()) {
        try {
            _updateFileSize();
        } catch (const std::exception &e) {
            throw ;
        }
    }
}

const e_file_type& FileIdent::getFileType(void) const {
    return (this->_fileType);
}
const e_file_content_syncro &FileIdent::getState(void) const {
    return (this->_state);
}

void FileIdent::syncTo(ReadEventFromFile *readEventFromFile,
e_file_content_syncro targetState) {
    readEventFromFile->_sync(&(this->_state), targetState, &(this->_haveToUpdate));
}
void FileIdent::syncTo(WriteEventToFile *writeEventToFile,
e_file_content_syncro targetState) {
    writeEventToFile->_sync(&(this->_state), targetState, &(this->_haveToUpdate));
}

long FileIdent::getFileSize(void) {
    _queryUpdateFileSize();
    return (this->_fileSize);
}

const char	*FileIdent::FailToOpenException::what(void) const throw() { return ("FileIdent: Fail to open"); }