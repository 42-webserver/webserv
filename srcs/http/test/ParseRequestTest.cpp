#include "../../../incs/http/parser/HttpRequestParser.hpp"

int main(){
	std::string bufferStr = "GET /apple-touch-icon-precomposed.png HTTP/1.1\r\n\
	Host: 10.19.206.66:4242\r\n\
	Accept: */*\r\n\
	Accept-Language: en-US,en;q=0.9\r\n\
	Connection: keep-alive\r\n\
	Accept-Encoding: gzip, deflate\r\n\
	User-Agent: Safari/18614.4.6.1.6 CFNetwork/1404.0.5 Darwin/22.3.0 like Gecko) Version/16.3 Safari/605.1.15 \r\n\
	Accept-Language: en-US,en;q=0.9\r\n\
	Accept-Encoding: gzip, deflate\r\n\
	Connection: keep-alive\r\n\
	ncoding: gzip, deflate\r\n\r\n";

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
	std::vector<char> body = httpRequestParser.getBuffer();
	std::multimap<std::string, std::string> headers = httpRequest.getHeaders();
	for (std::multimap<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
	std::cout << std::string(body.begin(), body.end()) << std::endl;

}