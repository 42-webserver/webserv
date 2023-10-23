#include <Pattern/PatternProcessor.hpp>

PatternProcessor::PatternProcessor(ft::shared_ptr<VirtualServerManager> vsm,
    PatternType type, ft::shared_ptr<Client> client) {
    this->_injectVsm(vsm);
    this->_injectProcessorObject(type);
    this->_injectClient(client);
}
PatternProcessor::~PatternProcessor() {}
void PatternProcessor::_injectVsm(ft::shared_ptr<VirtualServerManager> vsm) {
    this->_vsm = vsm;
}

void PatternProcessor::_injectProcessorObject(PatternType type) {
    switch (type) {
        case FILE_WRITE:
            this->_processorObject = ft::shared_ptr<ProcessorObject>(new FileWriterProcessor());
            break;
        case FILE_READ:
            this->_processorObject = ft::shared_ptr<ProcessorObject>(new FileReaderProcessor());
            break;
        case CGI_READ:
            this->_processorObject = ft::shared_ptr<ProcessorObject>(new CgiReaderProcessor());
            break;
        case DELETE:
            this->_processorObject = ft::shared_ptr<ProcessorObject>(new FileDeleterProcessor());
            break;
        case REDIRECTION:
            this->_processorObject = ft::shared_ptr<ProcessorObject>(new RedirectionProcessor());
            break;
        default:
            break;
    }
}
void PatternProcessor::_injectClient(ft::shared_ptr<Client> client) {
    this->_client = client;
}

e_pattern_Process_result PatternProcessor::process() {
    return this->_processorObject->process(this->_vsm, this->_client);
}

e_pattern_Process_result PatternProcessor::querryCanSending() {
    return this->_processorObject->querryCanSending(this->_vsm, this->_client);
}
