#include <Pattern/CgiReaderProcessor.hpp>

// To do: implement CgiReaderProcessor
CgiReaderProcessor::CgiReaderProcessor(void) {
}
CgiReaderProcessor::~CgiReaderProcessor(void) {
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
        //child
        try {
            const std::map<std::string, std::string>& env = 
            CgiEnvSetter::getInstance().getEnv(client, channel, virtualServerManager);
            EnvpManager envpManager = CgiEnvSetter::setEnvAll(env);
            cgiChannel->_dupFdInCgiProcess();
            cgiChannel->_closeServerSideFd();
            cgiChannel->_closeCgiSideFd();
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
            throw ;
        }
        exit(EXIT_FAILURE);
    }
    else {
        //parent
        // cgiChannel->setPid(pid);
        // client->setChannel(cgiChannel);
    }
    return SUCCESS;
}

e_pattern_Process_result CgiReaderProcessor::querryCanSending(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client) {
    (void)virtualServerManager;
    (void)client;
    return SUCCESS;
}