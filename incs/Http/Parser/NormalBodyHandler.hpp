#ifndef NORMAL_BODY_HANDLER_HPP
#define NORMAL_BODY_HANDLER_HPP

#include "RequestBodyHandler.hpp"

class NormalBodyHandler : public RequestBodyHandler
{
public:
	bool handleBody(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
	NormalBodyHandler(void);
	~NormalBodyHandler(void);

private:
	void writeInFile(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
	void writeInMemory(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
};

#endif