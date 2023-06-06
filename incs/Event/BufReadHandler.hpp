#ifndef BUFREADHANDLER_HPP
# define BUFREADHANDLER_HPP

# include "./ReadEvent.hpp"

class BufReadHandler: public ReadEventHandler {
	public:
		BufReadHandler();
		virtual ~BufReadHandler(void);

	private:
		BufReadHandler(const BufReadHandler&);
		BufReadHandler& operator=(const BufReadHandler&);

	public:
		virtual void	handleEvent(Event &event);
};

#endif