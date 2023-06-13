#include "../../../incs/http/parser/RequestBodyHandler.hpp"

RequestBodyHandler::RequestBodyHandler(int readBodySize)
: _readBodySize(readBodySize)
{}

RequestBodyHandler::~RequestBodyHandler(void)
{}