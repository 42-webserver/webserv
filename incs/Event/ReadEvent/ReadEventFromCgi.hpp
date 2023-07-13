#ifndef READEVENTFROMCGI_HPP
# define READEVENTFROMCGI_HPP

# include "ReadEvent.hpp"

class ReadEventFromCgi: public ReadEvent {
	public:
		ReadEventFromCgi(void);
		virtual	~ReadEventFromCgi(void);

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void) throw (std::exception);
		virtual void	offboardQueue(void) throw (std::exception);
};
#endif