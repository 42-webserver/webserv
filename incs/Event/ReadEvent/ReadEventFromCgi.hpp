#ifndef READEVENTFROMCGI_HPP
# define READEVENTFROMCGI_HPP

# include "ReadEvent.hpp"

class ReadEventCgi: public ReadEvent {
	public:
		ReadEventCgi(void);
		virtual	~ReadEventCgi(void);

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void) throw (std::exception);
		virtual void	offboardQueue(void) throw (std::exception);
};
#endif