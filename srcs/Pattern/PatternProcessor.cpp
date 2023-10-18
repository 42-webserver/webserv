#include <Pattern/PatternProcessor.hpp>

PatternProcessor *PatternProcessor::instance = NULL;
PatternProcessor::PatternProcessor() {}
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
PatternProcessor &PatternProcessor::getInstance(PatternType type,
ft::shared_ptr<VirtualServerManager> vsm,
ft::shared_ptr<Client> client) {
    if (PatternProcessor::instance == NULL)
        PatternProcessor::instance = new PatternProcessor();
    
    PatternProcessor::instance->_injectVsm(vsm);
    PatternProcessor::instance->_injectProcessorObject(type);
    PatternProcessor::instance->_injectClient(client);
    return *PatternProcessor::instance;
}

void PatternProcessor::process() {
    this->_processorObject->process(this->_vsm, this->_client);
}

void PatternProcessor::clear(void) {
    this->_vsm = ft::shared_ptr<VirtualServerManager>();
    this->_processorObject = ft::shared_ptr<ProcessorObject>();
    this->_client = ft::shared_ptr<Client>();
}