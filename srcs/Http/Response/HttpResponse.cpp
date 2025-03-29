#include "../../../incs/Http/Response/HttpResponse.hpp"
#include <Buffer/Exception/DisconnectionException.hpp>

HttpResponse::AccessKey::AccessKey()
{
}
HttpResponse::AccessKey::~AccessKey()
{
}

void HttpResponse::setFileSync(e_File_Sync fileSync, AccessKey key)
{
	(void)key;
	this->_fileSync = fileSync;
}

e_File_Sync HttpResponse::getFileSync(AccessKey key)
{
	(void)key;
	return this->_fileSync;
}

void HttpResponse::setResponseSize(e_ResponseSize responseSize , AccessKey key)
{
	(void)key;
	this->_responseSize = responseSize;
}
std::vector<char>& HttpResponse::getNormalCaseBuffer(AccessKey key)
{
	(void)key;
	return this->_NormalCaseBuffer;
}

e_ResponseSize HttpResponse::getResponseSize()
{
	return this->_responseSize;
}

ft::shared_ptr<IoReadAndWriteBuffer> &HttpResponse::getBigSizeBuffer(AccessKey key)
{
	(void)key;
	return this->_BigSizeBuffer;
}

void HttpResponse::allocateBigSizeBuffer(AccessKey key)
{
	(void)key;
	this->_BigSizeBuffer = ft::shared_ptr<IoReadAndWriteBuffer>(new IoReadAndWriteBuffer());
}
HttpResponse::HttpResponse() : _previousWriteSize(0), _responseSize(NotSet), _fileSync(NotSetting)
{
	this->_NormalCaseBuffer.reserve(e_normal_buffer_size);
}

HttpResponse::~HttpResponse()
{
}



// void HttpResponse::setFileName(std::string & fileName)
// {
// 	this->_fileName = fileName;
// }
// void HttpResponse::addCookie(const std::string & key, const std::string & value)
// {
// 	Cookie cookie(key, value);
// 	this->_cookies.push_back(cookie);
// }

// void HttpResponse::addHeader(const std::string & key, const std::string & value)
// {
// 	this->_headers.insert(std::pair<std::string, std::string>(key, value));
// }

// void HttpResponse::setStatusCode(HttpStatusCode code)
// {
// 	this->_statusCode = code;
// }

// std::string HttpResponse::getVersion()
// {
// 	return (std::string &)this->_version;
// }

// std::string HttpResponse::getFileName()
// {
// 	return this->_fileName;
// }

// std::vector<char> &HttpResponse::getBody()
// {
// 	return this->_body;
// }

// const std::vector<Cookie> &HttpResponse::getCookies() const
// {
// 	return (std::vector<Cookie> &)this->_cookies;
// }

// std::string HttpResponse::getProtocol()
// {
// 	return (std::string &)this->_protocol;
// }

// std::string HttpResponse::getReasonPhrase()
// {
// 	return HttpStatus::getReasonPhrase(this->_statusCode);
// }

// int HttpResponse::getStatusCode()
// {
// 	return HttpStatus::getStatusCode(this->_statusCode);
// }

// std::string HttpResponse::getHeader(const std::string & key)
// {
// 	std::multimap<std::string, std::string>::iterator it = this->_headers.find(key);
// 	if (it != this->_headers.end())
// 		return it->second;
// 	return "";
// }

// const std::multimap<std::string, std::string> &HttpResponse::getHeaders() const
// {
// 	return (std::multimap<std::string, std::string> &)this->_headers;
// }


// std::ostream &operator<<(std::ostream & os,const HttpResponse & response){
// 	os << "Version: " << response._version << std::endl;
// 	os << "Protocol: " << response._protocol << std::endl;
// 	os << "StatusCode: " << response._statusCode << std::endl;

// 	os << "Headers: " << std::endl;
// 	for (std::multimap<std::string, std::string>::const_iterator it = response._headers.begin(); it != response._headers.end(); it++)
// 		os << it->first << ": " << it->second << std::endl;
// 	os << "Cookies: " << std::endl;
// 	for (std::vector<Cookie>::const_iterator it = response._cookies.begin(); it != response._cookies.end(); it++)
// 		os << *it << std::endl;
// 	if (response._body.size() > 0){
// 		os << "Body: " << std::endl;
// 		for (std::vector<char>::const_iterator it = response._body.begin(); it != response._body.end(); it++)
// 			os << *it;
// 		os << std::endl;
// 	}else{
// 		os << "BodyFilename: " << response._fileName << std::endl;
// 	}
// 	return os;
// }

e_send_To_client_status HttpResponse::_sendNormalToClient(ft::shared_ptr<Channel> clientChannel)
{	
	ssize_t n;
	try {
		n = ft::_ioWrite(clientChannel->getFd(), this->_NormalCaseBuffer, this->_previousWriteSize);
	}
	catch (DisconnectionException &e) {
		return clientClose;
	}
	catch (std::exception &e) {
		//log
		throw ;
	}
	if (n < 0)
		return sending;
	this->_previousWriteSize += n;
	if (this->_previousWriteSize == this->_NormalCaseBuffer.size()) {
		this->_NormalCaseBuffer.clear();
		this->_previousWriteSize = 0;
		return sendingDone;
	}
	return sending;
}

e_send_To_client_status HttpResponse::_sendBigToClient(ft::shared_ptr<Channel> clientChannel)
{
	if (this->_NormalCaseBuffer.size() != 0) {
		if (_sendNormalToClient(clientChannel) == clientClose)
			return clientClose;
		return sending;
	}
	else {
		try {
			this->_BigSizeBuffer->ioWrite(clientChannel->getFd());
		}
		catch (DisconnectionException &e) {
			return clientClose;
		}
		catch (std::exception &e) {
			throw ;
		}
		if (this->_BigSizeBuffer->size() == 0)
			return sendingDone;
		return sending;
	}
}


e_send_To_client_status HttpResponse::sendToClient(ft::shared_ptr<Channel> clientChannel)
{
	if (this->_responseSize == NotSet) {
		throw std::runtime_error("HttpResponse::sendToClient : responseSize is NotSet");
		// exit(1);
	}
	if (this->_responseSize == NormalSize) {
		return this->_sendNormalToClient(clientChannel);
	}
	else {
		return this->_sendBigToClient(clientChannel);
	}
}

// bool HttpResponse::isSending()
// {
// 	return _isSending == true;
// }

// void HttpResponse::setCanSending()
// {
// 	this->_isSending = true;
// }

// const ft::shared_ptr<HttpResponseBuilder> &HttpResponse::getBuilder() 
// {
// 	return this->_builder;
// }

// void HttpResponse::allocateBuilder(ft::shared_ptr<HttpResponseBuilder> builder)
// {
// 	this->_builder = builder;
// }

void HttpResponse::setCgiSync(e_cgi_sync cgiSync)
{
	this->_cgiSync = cgiSync;
}

e_cgi_sync HttpResponse::getCgiSync()
{
	return this->_cgiSync;
}