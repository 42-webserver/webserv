#include <Event/WriteEvent/WriteEventToFileHandler.hpp>
#include <Event/WriteEvent/WriteEventToFile.hpp>

WriteEventToFileHandler::WriteEventToFileHandler(
ft::shared_ptr<IoReadAndWriteBuffer> buffer) : _buffer(buffer) {}
WriteEventToFileHandler::~WriteEventToFileHandler(void) {}

// Todo: 보내고 있는 와중에 클라이언트가 연결 끊으면, 파일 unlink
void WriteEventToFileHandler::handleEvent(Event &event) {
    WriteEventToFile *writeEventToFile = static_cast<WriteEventToFile *>(&event);
    size_t n;
    try {
     n = _buffer->ioWrite(writeEventToFile->getFd());
    }
    catch (const DisconnectionException &e) {
        throw ;
    }
    catch (...) {
        throw ;
    }
    if (n == 0) {
        writeEventToFile->offboardQueue();
    }
}