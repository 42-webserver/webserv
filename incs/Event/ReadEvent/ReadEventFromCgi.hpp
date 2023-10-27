#ifndef READEVENTFROMCGI_HPP
# define READEVENTFROMCGI_HPP

# include "ReadEvent.hpp"
# include <Client/Client.hpp>
# include <Channel/Channel.hpp>
# include <Channel/SingleStreamable.hpp>
# include "ReadEventFromCgiHandler.hpp"
class ReadEventFromCgi: public ReadEvent, public SingleStreamable {
	public:
		ReadEventFromCgi(ft::shared_ptr<Channel> readPipe,
			ft::shared_ptr<Client> client);
		virtual	~ReadEventFromCgi(void);

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void);
		virtual void	offboardQueue(void);
		ft::shared_ptr<Client> getClient(void) const;

	private:
		ft::shared_ptr<Client> _client;
};
#endif