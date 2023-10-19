#ifndef WriteEventToClientHandler_HPP
# define WriteEventToClientHandler_HPP

# include "WriteEventHandler.hpp"
#include "../../../incs/Http/Handler/ErrorPageHandler.hpp"
#include "../../../incs/Http/Response/HttpResponse.hpp"
#include "../../../incs/Log/Logger.hpp"
#include "../../../incs/Event/EventBase/EventFactory.hpp"
#include <Pattern/PatternProcessor.hpp>

class WriteEventToClient;
class WriteEventToClientHandler: public WriteEventHandler {

	public:
		WriteEventToClientHandler(void);
		virtual	~WriteEventToClientHandler(void);
		virtual void	handleEvent(Event &event);
	private:
		void _partialSending(ft::shared_ptr<HttpResponse> response, ft::shared_ptr<Client> client,
		WriteEventToClient *curEvent);
};
#endif