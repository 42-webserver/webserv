#include <Event/WriteEvent/WriteEventToCgiHandler.hpp>
#include <Event/WriteEvent/WriteEventToCgi.hpp>
WriteEventToCgiHandler::WriteEventToCgiHandler(void) {}

WriteEventToCgiHandler::~WriteEventToCgiHandler(void) {}

void WriteEventToCgiHandler::handleEvent(Event& event){
    WriteEventToCgi *writeEventToCgi = static_cast<WriteEventToCgi *>(&event);
    ft::shared_ptr<Client> client = writeEventToCgi->getClient();
    ft::shared_ptr<IoReadAndWriteBuffer> buffer = client->getRequest()->getBody();

    ssize_t size = buffer->size();
    ssize_t writeSize = buffer->ioWrite(writeEventToCgi->getChannel()->getFd());
    std::cerr << "WriteEventToCgiHandler::handleEvent: size: " << size << std::endl;
    std::cerr << "WriteEventToCgiHandler::handleEvent: writeSize: " << writeSize << std::endl;
    if (buffer->size() == 0) {
        writeEventToCgi->offboardQueue();
    }
}