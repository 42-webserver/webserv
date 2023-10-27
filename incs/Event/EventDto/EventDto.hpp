#ifndef EVENTDTO_HPP
# define EVENTDTO_HPP

# include "../../Server/VirtualServerManager.hpp"
# include "../../Channel/Channel.hpp"
# include "../../Http/Request/HttpRequest.hpp"
# include <Client/Client.hpp>
class  EventDto {
	private: 
		ft::shared_ptr<Channel>					_channel;
		ft::shared_ptr<VirtualServerManager>	_virtualServerManager;
		ft::shared_ptr<IoReadAndWriteBuffer>	_buffer;
		ft::shared_ptr<Client>					_client;
		std::vector<char>*						_content;
		std::string								_path;
		std::string								_mode;
		pid_t									_pid;
	public:
		EventDto(ft::shared_ptr<Channel> channel, 
		ft::shared_ptr<VirtualServerManager> virtualServerManager = ft::shared_ptr<VirtualServerManager>(),
		std::vector<char> *content = NULL);

		EventDto(void);
		EventDto(ft::shared_ptr<Channel> channel,
		ft::shared_ptr<VirtualServerManager> virtualServerManager,
		ft::shared_ptr<Client> client,
		std::vector<char> *content = NULL);

		EventDto(pid_t pid, 
		std::vector<char> *content = NULL);

		EventDto(ft::shared_ptr<IoReadAndWriteBuffer> buffer,
		const std::string &path, std::string mode,
		std::vector<char> *content = NULL);
		EventDto(ft::shared_ptr<Client> client,
		ft::shared_ptr<Channel> channel,
		std::vector<char> *content = NULL);
		EventDto(std::vector<char> *content,
		const std::string &path, std::string mode);
		~EventDto(void);
	private:
		EventDto &operator=(const EventDto &rhs);
		EventDto(const EventDto &ref);
	public:
		const ft::shared_ptr<Channel>	&getChannel() const;
		const ft::shared_ptr<VirtualServerManager>	&getVirtualServerManager() const;
		const ft::shared_ptr<IoReadAndWriteBuffer>	&getBuffer() const;
		std::vector<char>	&getContent() const;
		const std::string	&getPath() const;
		const std::string	&getMode() const;
		pid_t				getPid() const;
		const ft::shared_ptr<Client>	&getClient() const;
};

#endif