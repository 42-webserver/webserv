#ifndef READEVENTFROMCLIENTHANDLER_HPP
# define READEVENTFROMCLIENTHANDLER_HPP

#include "ReadEventHandler.hpp"
#include "../../Http/Parser/HttpRequestParser.hpp"
class ReadEventFromClientHandler : public ReadEventHandler {
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