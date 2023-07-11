#include "../../../incs/Http/Parser/ChunkedRequestBodyHandler.hpp"

ChunkedRequestBodyHandler::ChunkedRequestBodyHandler(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req)
: RequestBodyHandler(0, req), _vsm(vsm)
{
	this->_filename = RouterUtils::findPath(vsm, req);
	FileUploader::checkFileExists(this->_filename);
}

ChunkedRequestBodyHandler::~ChunkedRequestBodyHandler(void)
{}

bool ChunkedRequestBodyHandler::handleBody(std::vector<char> &reqBuffer) throw(ChunkDataSizeNotMatchException){
	if (!this->_buffer.empty())
		reqBuffer.insert(reqBuffer.begin(), this->_buffer.begin(), this->_buffer.end());
	//find chunk size
	std::vector<char>::iterator find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	while (find != reqBuffer.end()) {
		size_t chunkSize = _hexToDec(std::string(reqBuffer.begin(), find));
		reqBuffer.erase(reqBuffer.begin(), find + _crlfPattern.size());
		if (chunkSize == 0) {
			reqBuffer.clear();
			return true;
		}

		//find chunk data
		find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
		if (find != reqBuffer.end()) {
			std::vector<char> tmp = std::vector<char>(reqBuffer.begin(), find);
			if (tmp.size() != chunkSize)
				throw ChunkDataSizeNotMatchException();
			reqBuffer.erase(reqBuffer.begin(), find + _crlfPattern.size());
			_uploadFile(tmp);
		}
		else {
			this->_buffer.insert(this->_buffer.end(), reqBuffer.begin(), reqBuffer.end());
			reqBuffer.clear();
			return false;
		}
		//find chunk size
		find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	}
	this->_buffer.insert(this->_buffer.end(), reqBuffer.begin(), reqBuffer.end());
	reqBuffer.clear();
	return false;
}

void ChunkedRequestBodyHandler::_uploadFile(std::vector<char> &reqBuffer) {
	FileUploader::fileUpload(reqBuffer, this->_filename);
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