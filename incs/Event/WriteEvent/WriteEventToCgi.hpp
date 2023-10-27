#ifndef WRITEEVENTTOCGI_HPP
# define WRITEEVENTTOCGI_HPP

# include "WriteEvent.hpp"
# include <Http/Request/HttpRequest.hpp>
# include <Channel/SingleStreamable.hpp>
# include <Event/WriteEvent/WriteEventToCgiHandler.hpp>
# include <Client/Client.hpp>

class WriteEventToCgi: public WriteEvent, public SingleStreamable{
    public:
        WriteEventToCgi(ft::shared_ptr<Channel> channel, 
        ft::shared_ptr<Client> client);
        virtual	~WriteEventToCgi(void);

    public:
        virtual void	callEventHandler(void);
        virtual void	onboardQueue(void);
        virtual void	offboardQueue(void);
        ft::shared_ptr<Client> getClient(void);
    private:
        ft::shared_ptr<Client>	_client;
};

#endif