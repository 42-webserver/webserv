#include <Pattern/CgiReaderProcessor.hpp>

// To do: implement CgiReaderProcessor
CgiReaderProcessor::CgiReaderProcessor(void) {
}
CgiReaderProcessor::~CgiReaderProcessor(void) {
}

void CgiReaderProcessor::_handleCgiProcess(ft::shared_ptr<CgiChannel> cgiChannel, ft::shared_ptr<Client> client,
    ft::shared_ptr<VirtualServerManager> virtualServerManager, ft::shared_ptr<Channel> channel) {
        try {
            const std::map<std::string, std::string>& env = 
            CgiEnvSetter::getInstance().getEnv(client, channel, virtualServerManager);
            EnvpManager envpManager = CgiEnvSetter::setEnvAll(env);
            cgiChannel->_dupFdInCgiProcess();
            cgiChannel->_closeServerSideFd();
            cgiChannel->_closeCgiSideFd();

            EventQueue &eventQueue = EventQueue::getInstance();
            eventQueue.deleteInstance();

            std::string cgiPath =  RouterUtils::findCgiScriptPath(virtualServerManager, client->getRequest());
            if (cgiPath.empty()) {
                throw std::runtime_error("cgiPath is empty -> logical error");
            }
            execve(cgiPath.c_str(), NULL, envpManager.getEnvp());
            if (errno == ENOENT) {
                throw NotFoundException();
            }
            else if (errno == EACCES || errno == EPERM || errno == EISDIR) {
                throw ForbiddenException();
            }
            else {
                throw std::runtime_error("execve error");
            }
        }
        catch (const std::exception &e) {
            //log error CgiChannel execute,e.what
            // throw ;
        }
        exit(EXIT_FAILURE);
}

void CgiReaderProcessor::_onBoardReadFromCgi(ft::shared_ptr<Channel> channel, ft::shared_ptr<Client> client) {
    EventFactory& eventFactory = EventFactory::getInstance();

    EventDto eventDto(client, channel);
    ft::shared_ptr<Event> event = eventFactory.createEvent(ft::READ_EVENT_FROM_CGI, eventDto);
    event->onboardQueue();
}

void CgiReaderProcessor::_onBoardWriteToCgi(ft::shared_ptr<Channel> channel, ft::shared_ptr<Client> client) {
    EventFactory& eventFactory = EventFactory::getInstance();

    EventDto eventDto(client, channel);
    ft::shared_ptr<Event> event = eventFactory.createEvent(ft::WRITE_EVENT_TO_GCI, eventDto);
    event->onboardQueue();
}

void CgiReaderProcessor::_onBoardCgiWait(pid_t pid) {
    EventFactory& eventFactory = EventFactory::getInstance();

    EventDto eventDto(pid);
    ft::shared_ptr<Event> event = eventFactory.createEvent(ft::CGI_WAITING_EVENT, eventDto);
    event->onboardQueue();
}
e_pattern_Process_result CgiReaderProcessor::process(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client,
    ft::shared_ptr<Channel> channel) {
    ft::shared_ptr<CgiChannel> cgiChannel;
    pid_t pid;
    try {
        cgiChannel->build();
    }
    catch (const std::exception &e) {
        //log error CgiChannel build,e.what
        throw ;
    }
    pid = fork();
    if (pid == -1) {
        throw std::runtime_error("fork error");
    }
    else if (pid == 0) {
        _handleCgiProcess(cgiChannel, client, virtualServerManager, channel);
    }
    else {
        //parent
        cgiChannel->_closeCgiSideFd();
        try {
            _onBoardReadFromCgi(cgiChannel->getChannel(e_server_read), client);
            cgiChannel->destroy(e_server_read);
            if (client->getRequest()->getBodySize() > 0) {
                _onBoardWriteToCgi(cgiChannel->getChannel(e_server_write), client);
            }
            cgiChannel->destroy(e_server_write);
            _onBoardCgiWait(pid);
            // cgiChannel->_setNonBlockServerSideFd();
        }
        catch (const std::exception &e) {
            //log error CgiChannel setNonBlock,e.what
            throw ;
        }
        
    }
    return WAITING;
}

e_pattern_Process_result CgiReaderProcessor::querryCanSending(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client) {
    (void)virtualServerManager;
    ft::shared_ptr<HttpResponse> response = client->getResponse();
    if (response->getCgiSync() == e_cgi_reading_done) {
        return SUCCESS;
    }
    return WAITING;
}