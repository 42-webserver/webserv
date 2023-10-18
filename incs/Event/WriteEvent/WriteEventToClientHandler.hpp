#ifndef WriteEventToClientHandler_HPP
# define WriteEventToClientHandler_HPP

# include "WriteEventHandler.hpp"
#include "../../../incs/Http/Handler/ErrorPageHandler.hpp"
#include "../../../incs/Http/Response/HttpResponse.hpp"
#include "../../../incs/Log/Logger.hpp"
#include "../../../incs/Event/EventBase/EventFactory.hpp"
#include <Pattern/PatternProcessor.hpp>

class WriteEventToClientHandler: public WriteEventHandler {

	public:
		WriteEventToClientHandler(void);
		virtual	~WriteEventToClientHandler(void);
		virtual void	handleEvent(Event &event);
};
#endif