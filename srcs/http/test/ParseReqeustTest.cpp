#include "../../../incs/http/parser/HttpRequestParser.hpp"

int main(){
	std::string bufferStr = "GET / HTTP/1.1\r\nHost: loca";
	std::vector<char> httpReqeustBuffer;
	httpReqeustBuffer.assign(bufferStr.begin(), bufferStr.end());
	HttpRequest httpRequest;
	HttpRequestParser httpRequestParser(&httpRequest);
	httpRequestParser.parseRequest(httpReqeustBuffer);
	std::cout << httpRequest.getUri() << "\n" << \
	httpRequest.getProtocol() << std::endl;
	std::cout << httpRequest.getMethod() << std::endl;
	std::cout << httpRequest.getVersion() << std::endl;
	std::cout << httpRequestParser.getState() << std::endl;
	std::cout << httpRequestParser.getBuffer() << std::endl;
}