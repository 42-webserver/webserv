#ifndef LISTENEVENT_HPP
# define LISTENEVENT_HPP

# include "../EventBase/Event.hpp"
# include "../../Log/Logger.hpp"
# include "../../Server/VirtualServerManager.hpp"
# include <Channel/SingleStreamable.hpp>
class ListenEvent: public Event, public SingleStreamable{
	public:
		ListenEvent(ft::shared_ptr<Channel> channel,
		ft::shared_ptr<VirtualServerManager> physical_server);
		virtual	~ListenEvent(void);

	private:
		ListenEvent(const ListenEvent &ref);
		ListenEvent	&operator=(const ListenEvent &rhs);

	private:
		ft::shared_ptr<VirtualServerManager>	_virtualServerManager;
	
	public:
		ft::shared_ptr<VirtualServerManager>	getVirtualServerManager(void) const;

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void);
		virtual void	offboardQueue(void);

	public:
		class FailToAcceptException: public std::exception {
			public:
				const char *what() const throw();
		};
		class FailToControlException: public std::exception {
			public:
				const char *what() const throw();
		};
};

#endif