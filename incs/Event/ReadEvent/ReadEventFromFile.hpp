#ifndef READEVENFROMFILE_HPP
# define READEVENFROMFILE_HPP

# include "ReadEvent.hpp"

class ReadEventFromFile: public ReadEvent {
    public:
        ReadEventFromFile(ft::shared_ptr<Channel> channel, 
            ft::shared_ptr<VirtualServerManager> virtualServerManager);
        virtual	~ReadEventFromFile(void);

    public:
        const ft::shared_ptr<VirtualServerManager>	&getVirtualServerManger(void) const;

    public:
        virtual void	callEventHandler(void);
        virtual void	onboardQueue(void);
        virtual void	offboardQueue(void);
    
    private:
        ft::shared_ptr<VirtualServerManager>	_virtualServerManager;
};

#endif