#ifndef PATTERNSPROCESSOR_HPP
# define PATTERNSPROCESSOR_HPP


#include <Pattern/ProcessorObject.hpp>
#include <Pattern/FileReaderProcessor.hpp>
#include <Pattern/FileWriterProcessor.hpp>
#include <Pattern/FileDeleterProcessor.hpp>
#include <Pattern/CgiReaderProcessor.hpp>
#include <Pattern/RedirectionProcessor.hpp>
#include <Client/Client.hpp>
#include <Pattern/PatternType.hpp>


class PatternProcessor {
private:
    ft::shared_ptr<VirtualServerManager> _vsm;
    ft::shared_ptr<ProcessorObject> _processorObject;
    ft::shared_ptr<Client> _client;
    static PatternProcessor *instance;
private:
    PatternProcessor();
    ~PatternProcessor();
private:
    void _injectVsm(ft::shared_ptr<VirtualServerManager> vsm);
    void _injectProcessorObject(PatternType type);
    void _injectClient(ft::shared_ptr<Client> client);
private:
//delete
    PatternProcessor(const PatternProcessor &ref);
    PatternProcessor &operator=(const PatternProcessor &rhs);
public:
    static PatternProcessor &getInstance(PatternType type,
    ft::shared_ptr<VirtualServerManager> vsm, 
    ft::shared_ptr<Client> client);
    // void deleteInstance(void);
    void process();
    void clear();
};
#endif 