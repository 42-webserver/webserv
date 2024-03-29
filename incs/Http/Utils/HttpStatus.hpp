#ifndef HTTPSTATUS_HPP
#define HTTPSTATUS_HPP
#include <string>
#include <sstream>
#include <map>
typedef enum HttpStatusCode{
		CONTINUE = 100,
		SWITCHING_PROTOCOLS = 101,
		PROCESSING = 102,
		OK = 200,
		CREATED = 201,
		ACCEPTED = 202,
		NON_AUTHORITATIVE_INFORMATION = 203,
		NO_CONTENT = 204,
		RESET_CONTENT = 205,
		PARTIAL_CONTENT = 206,
		MULTI_STATUS = 207,
		MULTIPLE_CHOICES = 300,
		MOVED_PERMANENTLY = 301, //redirect (return)
		FOUND = 302,
		SEE_OTHER = 303,
		NOT_MODIFIED = 304,
		USE_PROXY = 305,
		TEMPORARY_REDIRECT = 307,
		BAD_REQUEST = 400,
		UNAUTHORIZED = 401,
		PAYMENT_REQUIRED = 402,
		FORBIDDEN = 403, //file 안열릴때 -> 권한이 없어서 안열림
		NOT_FOUND = 404, //파일이 없어서 안열릴 때
		METHOD_NOT_ALLOWED = 405, //allow element가 없을때 or allow 옵션에 없는
		NOT_ACCEPTABLE = 406,
		PROXY_AUTHENTICATION_REQUIRED = 407,
		REQUEST_TIMEOUT = 408, //time out시에
		CONFLICT = 409,
		GONE = 410,
		LENGTH_REQUIRED = 411,
		PRECONDITION_FAILED = 412,
		REQUEST_ENTITY_TOO_LARGE = 413,
		REQUEST_URI_TOO_LONG = 414,
		UNSUPPORTED_MEDIA_TYPE = 415,
		REQUESTED_RANGE_NOT_SATISFIABLE = 416,
		EXPECTATION_FAILED = 417,
		INTERNAL_SERVER_ERROR = 500,
		NOT_IMPLEMENTED = 501,
		BAD_GATEWAY = 502, //cgi 응답이 이상할 때
		SERVICE_UNAVAILABLE = 503,
		GATEWAY_TIMEOUT = 504, //cgi timeout
		HTTP_VERSION_NOT_SUPPORTED = 505,
		INSUFFICIENT_STORAGE = 507
} HttpStatusCode;



class HttpStatus{

public:
	static int getStatusCode(HttpStatusCode code);
	static std::string getReasonPhrase(HttpStatusCode code);
	static HttpStatusCode stringToHttpStatusCode(const std::string& httpResponseStr);

	class HttpStatusNotValidException : public std::exception{
	private:
	const char *_message;

public:
	public:
	HttpStatusNotValidException();
	HttpStatusNotValidException(std::string message);
	virtual const char * what () const throw();
	};
	
};

#endif