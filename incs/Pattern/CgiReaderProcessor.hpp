#ifndef CGIREADERPROCESSOR_HPP
# define CGIREADERPROCESSOR_HPP

#include <Pattern/ProcessorObject.hpp>
#include <Channel/CgiChannel.hpp>
#include <Cgi/CgiEnvSetter.hpp>



class CgiReaderProcessor : public ProcessorObject {
    public:
        CgiReaderProcessor(void);
        virtual ~CgiReaderProcessor(void);
        virtual e_pattern_Process_result process(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client, ft::shared_ptr<Channel> channel);
        virtual e_pattern_Process_result querryCanSending(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client);
    private:
        void _handleCgiProcess(ft::shared_ptr<CgiChannel> cgiChannel, ft::shared_ptr<Client> client,
            ft::shared_ptr<VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Channel> channel);
        void _onBoardReadFromCgi(int fd, ft::shared_ptr<Client> client);
        void _onBoardWriteToCgi(int fd, ft::shared_ptr<Client> client);
        void _onBoardCgiWait(pid_t pid);
        void _onBoardGateWayTimeout(pid_t pid);
    public:
        static std::string getInterPreterPath(const std::string & extention);
  
};
#endif