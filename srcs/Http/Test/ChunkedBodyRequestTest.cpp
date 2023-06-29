#include "../../../incs/Http/Parser/HttpRequestParser.hpp"

std::string createChunkedRequestBody(const std::string& bodyData) {
	std::string requestBody;
	requestBody += "POST / Http/1.1\r\n";
	requestBody += "Host: localhost:8080\r\n";
	requestBody += "User-Agent: curl/7.64.1\r\n";
	requestBody += "Transfer-Encoding: chunked\r\n";
	requestBody += "\r\n";

	// 청크 데이터
	std::vector<std::string> chunks;
	const size_t chunkSize = 9;  // 각 청크의 크기 설정 
	size_t offset = 0;
	while (offset < bodyData.length()) {
		chunks.push_back(bodyData.substr(offset, chunkSize));
		offset += chunkSize;
	}

	for (size_t i = 0; i < chunks.size(); i++) {
		std::string chunk = chunks[i];
		requestBody += std::to_string(chunk.length()) + "\r\n";  // 청크 크기 헤더
		requestBody += chunk + "\r\n";  // 청크 데이터
	}

	requestBody += "0\r\n";  // 마지막 청크 표시
	requestBody += "\r\n";  // 청크 데이터 종료

	return requestBody;
}

int main(){
	std::string bodyData = "This is the request body data. !!!!! hello world !!!!!";
	std::string chunkedBodyRequest = createChunkedRequestBody(bodyData);
	
	
	std::vector<char> httpReqeustBuffer;
	httpReqeustBuffer.assign(chunkedBodyRequest.begin(), chunkedBodyRequest.end());
	HttpRequestParser httpRequestParser;
	httpRequestParser.parseRequest(httpReqeustBuffer, 1000000);
	ft::shared_ptr <HttpRequest> httpRequest = httpRequestParser.getHttpRequest();
	std::cout << httpRequest->getUri() << "\n" << \
	httpRequest->getProtocol() << std::endl;
	std::cout << httpRequest->getMethod() << std::endl;
	std::cout << httpRequest->getVersion() << std::endl;
	std::cout << httpRequestParser.getState() << std::endl;
	std::cout << httpRequest->getBody().data() << std::endl;
	std::cout << httpRequest->isBodyLong() << std::endl;
	std::cout << httpRequest->getBodyDataFilename() << std::endl;


}