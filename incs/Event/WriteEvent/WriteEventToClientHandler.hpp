#ifndef WriteEventToClientHandler_HPP
# define WriteEventToClientHandler_HPP

# include "WriteEventHandler.hpp"
#include "../../../incs/Http/Response/HttpResponse.hpp"
#include "../../../incs/Log/Logger.hpp"
#include "../../../incs/Event/EventBase/EventFactory.hpp"
#include <Pattern/PatternProcessor.hpp>
#include <Http/Response/ErrorPageBuilder.hpp>

class WriteEventToClient;

typedef enum{
	e_handle_remain,
	e_handle_new,
	e_handle_wait,
	e_handle_end
}	e_handle_status;
class WriteEventToClientHandler: public WriteEventHandler {

	public:
		WriteEventToClientHandler(void);
		virtual	~WriteEventToClientHandler(void);
		virtual void	handleEvent(Event &event);
	private:
		void _partialSending(ft::shared_ptr<HttpResponse> response, ft::shared_ptr<Client> client,
		WriteEventToClient *curEvent);
		void _handleRemain(ft::shared_ptr<Client> client, WriteEventToClient *curEvent);
		void _handleNew(ft::shared_ptr<Client> client, WriteEventToClient *curEvent);
		void _handleWait(void);
		void _handleEnd(WriteEventToClient *curEvent);
		void _hanldeErrorPage(ft::shared_ptr<Client> client, WriteEventToClient *curEvent, HttpStatusCode statusCode);
		bool _queryIsExistHandleNew(ft::shared_ptr<Client> client);
		e_handle_status _queryHandleStatus(ft::shared_ptr<Client> client);
};
#endif