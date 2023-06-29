#include "../../../incs/Http/Parser/RequestBodyHandler.hpp"

RequestBodyHandler::RequestBodyHandler(int readBodySize)
: _readBodySize(readBodySize)
{}

RequestBodyHandler::~RequestBodyHandler(void)
{}