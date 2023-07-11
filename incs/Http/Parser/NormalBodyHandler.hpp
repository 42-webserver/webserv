#ifndef NORMAL_BODY_HANDLER_HPP
#define NORMAL_BODY_HANDLER_HPP

#include "RequestBodyHandler.hpp"

class NormalBodyHandler : public RequestBodyHandler
{
public:
	bool handleBody(std::vector<char> &reqBuffer);
	NormalBodyHandler(ft::shared_ptr<HttpRequest> req);
	~NormalBodyHandler(void);

private:
	void writeInFile(std::vector<char> &reqBuffer);
	void writeInMemory(std::vector<char> &reqBuffer);
};

#endif