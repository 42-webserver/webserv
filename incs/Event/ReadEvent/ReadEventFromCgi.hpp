#ifndef READEVENTFROMCGI_HPP
# define READEVENTFROMCGI_HPP

# include "ReadEvent.hpp"

class ReadEventFromCgi: public ReadEvent {
	public:
		ReadEventFromCgi(void);
		virtual	~ReadEventFromCgi(void);

	public:
		virtual void	callEventHandler(void);
		virtual void	onboardQueue(void);
		virtual void	offboardQueue(void);
};
#endif