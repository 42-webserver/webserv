#include <FileManager/FileTableManager/FileIdent.hpp>

FileIdent::~FileIdent(void) {}
FileIdent::FileIdent(const std::string &path) : _path(path), _state(Init), _haveToUpdate(true) {
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
    FILE *file = fopen(this->_path.c_str(), "w+");

    if (file == NULL)
        throw FileIdent::FailToOpenException();
    long originalPos = ftell(file);
    fseek(file, 0, SEEK_END);
    this->_fileSize = ftell(file);
    fseek(file, originalPos, SEEK_SET);
    fclose(file);
}
bool FileIdent::_queryUpdateNeeded() {
    bool ret = false;
    if (_haveToUpdate) {
        ret = true;
        _haveToUpdate = false;
    }
    return (ret);
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