#include "../../../incs/Http/Parser/RequestBodyHandler.hpp"

RequestBodyHandler::RequestBodyHandler(int readBodySize, ft::shared_ptr<HttpRequest> req)
: _readBodySize(readBodySize), _request(req)
{}

RequestBodyHandler::~RequestBodyHandler(void)
{}