#ifndef READEVENTFROMCLIENTHANDLER_HPP
# define READEVENTFROMCLIENTHANDLER_HPP

#include "ReadEventHandler.hpp"
#include "../../../incs/Event/ReadEvent/ReadEvent.hpp"
#include "../../../incs/FtUtil/ft.hpp"
#include "../../../incs/Server/VirtualServerManager.hpp"
#include "../../../incs/Event/EventQueue/EventQueue.hpp"
#include "../../../incs/Event/EventDto/EventDto.hpp"
#include "../../../incs/Event/EventBase/EventFactory.hpp"
#include "../../../incs/Log/Logger.hpp"
#include "../../Http/Parser/HttpRequestParser.hpp"
#include <Buffer/Buffer/IoOnlyReadBuffer.hpp>
#include <Event/WriteEvent/WriteEventToClient.hpp>

class ReadEventFromClient;
class ReadEventFromClientHandler : public ReadEventHandler {
	public:
		typedef enum{
			Reading,
			NonBlock,
			Closed,
			ConnectionCount
		}	e_client_connection_state;
		typedef void (ReadEventFromClientHandler::*_processFunc)(ReadEventFromClient *event);
	private:
		static _processFunc	_processFuncs[ConnectionCount];
	private:
		void _processReading(ReadEventFromClient *event);
		void _processNonBlock(ReadEventFromClient *event);
		void _processClosed(ReadEventFromClient *event);
		inline void _process(e_client_connection_state state, ReadEventFromClient *event);
		inline e_client_connection_state	_processMatcher(size_t n);
	public:
		ReadEventFromClientHandler(void);
		virtual	~ReadEventFromClientHandler(void);

	public:
		const ft::shared_ptr <HttpRequestParser>	&getHttpRequestParser(void);

	public:
		virtual void	handleEvent(Event &event);

	private:
		ft::shared_ptr<HttpRequestParser>	_HttpRequestParser;
};
#endif