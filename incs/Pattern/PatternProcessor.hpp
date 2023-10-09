#ifndef PATTERNSPROCESSOR_HPP
# define PATTERNSPROCESSOR_HPP


#include <Pattern/ProcessorObject.hpp>
#include <Pattern/FileReaderProcessor.hpp>
#include <Pattern/FileWriterProcessor.hpp>
#include <Pattern/FileDeleterProcessor.hpp>
#include <Pattern/CgiReaderProcessor.hpp>

typedef enum PatternType{
    FILE_WRITE = 0,
    FILE_READ,
    CGI_READ,
    DELETE,
    PatternSize
} PatternType;

class PatternProcessor {
private:
    ft::shared_ptr<VirtualServerManager> _vsm;
    ft::shared_ptr<HttpRequest> _request;
    ft::shared_ptr<ProcessorObject> _processorObject;
    static PatternProcessor *instance;
private:
    PatternProcessor();
    ~PatternProcessor();
private:
    void _injectVsm(ft::shared_ptr<VirtualServerManager> vsm);
    void _injectRequest(ft::shared_ptr<HttpRequest> request);
    void _injectProcessorObject(PatternType type);
private:
//delete
    PatternProcessor(const PatternProcessor &ref);
    PatternProcessor &operator=(const PatternProcessor &rhs);
public:
    static PatternProcessor &getInstance(PatternType type,
    ft::shared_ptr<VirtualServerManager> vsm, 
    ft::shared_ptr<HttpRequest> request);
    // void deleteInstance(void);
    void process();
};
#endif 