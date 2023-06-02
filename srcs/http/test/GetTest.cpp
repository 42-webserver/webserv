#include "../../../incs/http/handler/HttpRequestHandler.hpp"
#include "../../../incs/http/request/HttpRequest.hpp"
#include "../../../incs/http/response/HttpResponse.hpp"
#include "../../../incs/http/parser/HttpParser.hpp"



std::string & getStr(){
	std::string *str = new std::string();
	str = new std::string("hello");
	std::cout << "addr : " << str << std::endl;
	return (*str);
}

int main(){
	HttpRequest *request;
	HttpResponse *response;
	HttpParser parser;
	HttpRequestHandler handler;
	std::string *requestString = new std::string("GET / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.64.1\r\nAccept: */*\r\n\r\n");

	request = parser.parseRequest(requestString);

	response = handler.handle(request, &server);

	std::string *respStr = parser.parseResponse(response);
	std::cout << *respStr << std::endl;
	delete request;
	delete response;
	delete requestString;
	// system("leaks webserv");
}