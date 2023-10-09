#include <Pattern/PatternProcessor.hpp>

PatternProcessor *PatternProcessor::instance = NULL;
PatternProcessor::PatternProcessor() {}
PatternProcessor::~PatternProcessor() {}
void PatternProcessor::_injectVsm(ft::shared_ptr<VirtualServerManager> vsm) {
    this->_vsm = vsm;
}
void PatternProcessor::_injectRequest(ft::shared_ptr<HttpRequest> request) {
    this->_request = request;
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
        default:
            break;
    }
}
PatternProcessor &PatternProcessor::getInstance(PatternType type,
ft::shared_ptr<VirtualServerManager> vsm,
ft::shared_ptr<HttpRequest> request) {
    if (PatternProcessor::instance == NULL)
        PatternProcessor::instance = new PatternProcessor();
    
    PatternProcessor::instance->_injectVsm(vsm);
    PatternProcessor::instance->_injectRequest(request);
    PatternProcessor::instance->_injectProcessorObject(type);
    return *PatternProcessor::instance;
}

void PatternProcessor::process() {
    this->_processorObject->process(this->_vsm, this->_request);
}