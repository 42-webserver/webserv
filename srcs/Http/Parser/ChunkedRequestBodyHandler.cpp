#include "../../../incs/Http/Parser/ChunkedRequestBodyHandler.hpp"

ChunkedRequestBodyHandler::ChunkedRequestBodyHandler(ft::shared_ptr<HttpRequest> req)
: RequestBodyHandler(0, req)
{
}

ChunkedRequestBodyHandler::~ChunkedRequestBodyHandler(void)
{}

bool ChunkedRequestBodyHandler::handleBody(std::vector<char> &buffer) {
	// find chunk size
	for (size_t i = 0; i < buffer.size(); i++)
		std::cerr << buffer[i];
	std::cerr << std::endl;
	std::cerr << "ChunkedRequestBodyHandler::handleBody" << std::endl;
	
	std::vector<char>::iterator find = std::search(buffer.begin(), buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	while (find != buffer.end()) {
		size_t chunkSize = _hexToDec(std::string(buffer.begin(), find));
		std::cerr << "chunkSize: " << chunkSize << std::endl;
		buffer.erase(buffer.begin(), find + _crlfPattern.size());
		if (chunkSize == 0) {
			buffer.erase(buffer.begin(), buffer.begin() + _crlfPattern.size());
			return true;
		}

		//find chunk data
		find = std::search(buffer.begin(), buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
		if (find != buffer.end()) {
			size_t chunkDataSize = find - buffer.begin();
			std::cerr << "chunkDataSize: " << chunkDataSize << std::endl;
			if (chunkDataSize != chunkSize)
				throw ChunkDataSizeNotMatchException();
			ft::shared_ptr<IoReadAndWriteBuffer> body = this->_request->getBody();
			body->append(buffer.begin(), find);
			buffer.erase(buffer.begin(), find + _crlfPattern.size());
			std::cerr << "chunk data: " << std::string(buffer.begin(), find) << std::endl;
		}
		else 
			return false;
		//find chunk size
		find = std::search(buffer.begin(), buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	}
	return false;
}

void ChunkedRequestBodyHandler::_writeInMemory(std::vector<char> &writeBuffer) {
	this->_request->insertBody(writeBuffer);
}

int ChunkedRequestBodyHandler::_hexToDec(const std::string& hexStr) {
	int decValue;
	std::istringstream(hexStr) >> std::hex >> decValue;
	return decValue;
}

const char * ChunkedRequestBodyHandler::ChunkDataSizeNotMatchException::what() const throw()
{
	return "Chunk data size not match";
}