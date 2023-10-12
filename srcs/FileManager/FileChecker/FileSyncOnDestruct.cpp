#include <FileManager/FileChecker/FileSyncOnDestruct.hpp>

FileSyncOnDestruct::FileSyncOnDestruct(void) : _state(NULL), _haveToUpdate(NULL), _targetState(Init) {}
FileSyncOnDestruct::~FileSyncOnDestruct(void) {
    if (_state != NULL)
        *_state = _targetState;
    if (_haveToUpdate != NULL)
        *_haveToUpdate = true;
}
void FileSyncOnDestruct::Sync(e_file_content_syncro *origin, e_file_content_syncro targetState, bool *haveToUpdate) {
    _state = origin;
    _targetState = targetState;
    _haveToUpdate = haveToUpdate;
}