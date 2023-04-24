#include "HttpStatus.hpp"

HttpStatus::HttpStatusNotValidException::HttpStatusNotValidException(std::string message)
{
    this->_message = message.c_str();
}

HttpStatus::HttpStatusNotValidException::HttpStatusNotValidException(){
    this->_message = "Status code not valid";
}

const char *HttpStatus::HttpStatusNotValidException::what() const throw(){
    return this->_message;
}

int HttpStatus::getStatusCode(HttpStatusCode code)
{
    if (code == CONTINUE)
        return 100;
    else if (code == SWITCHING_PROTOCOLS)
        return 101;
    else if (code == PROCESSING)
        return 102;
    else if (code == OK)
        return 200;
    else if (code == CREATED)
        return 201;
    else if (code == ACCEPTED)
        return 202;
    else if (code == NON_AUTHORITATIVE_INFORMATION)
        return 203;
    else if (code == NO_CONTENT)
        return 204;
    else if (code == RESET_CONTENT)
        return 205;
    else if (code == PARTIAL_CONTENT)
        return 206;
    else if (code == MULTIPLE_CHOICES)
        return 300;
    else if (code == MOVED_PERMANENTLY)
        return 301;
    else if (code == FOUND)
        return 302;
    else if (code == SEE_OTHER)
        return 303;
    else if (code == NOT_MODIFIED)
        return 304;
    else if (code == USE_PROXY)
        return 305;
    else if (code == TEMPORARY_REDIRECT)
        return 307;
    else if (code == BAD_REQUEST)
        return 400;
    else if (code == UNAUTHORIZED)
        return 401;
    else if (code == PAYMENT_REQUIRED)
        return 402;
    else if (code == FORBIDDEN)
        return 403;
    else if (code == NOT_FOUND)
        return 404;
    else if (code == METHOD_NOT_ALLOWED)
        return 405;
    else if (code == NOT_ACCEPTABLE)
        return 406;
    else if (code == PROXY_AUTHENTICATION_REQUIRED)
        return 407;
    else if (code == REQUEST_TIMEOUT)
        return 408;
    else if (code == CONFLICT)
        return 409;
    else if (code == GONE)
        return 410;
    else if (code == LENGTH_REQUIRED)
        return 411;
    else if (code == PRECONDITION_FAILED)
        return 412;
    else if (code == REQUEST_ENTITY_TOO_LARGE)
        return 413;
    else if (code == REQUEST_URI_TOO_LONG)
        return 414;
    else if (code == UNSUPPORTED_MEDIA_TYPE)
        return 415;
    else if (code == REQUESTED_RANGE_NOT_SATISFIABLE)
        return 416;
    else if (code == EXPECTATION_FAILED)
        return 417;
    else if (code == INTERNAL_SERVER_ERROR)
        return 500;
    else if (code == NOT_IMPLEMENTED)
        return 501;
    else if (code == BAD_GATEWAY)
        return 502;
    else if (code == SERVICE_UNAVAILABLE)
        return 503;
    else if (code == GATEWAY_TIMEOUT)
        return 504;
    else if (code == HTTP_VERSION_NOT_SUPPORTED)
        return 505;
    else if (code == INSUFFICIENT_STORAGE)
        return 507;
    throw HttpStatus::HttpStatusNotValidException();
}

std::string HttpStatus::getReasonPhrase(HttpStatusCode code)
{

    if (code == CONTINUE)
        return "Continue";
    else if (code == SWITCHING_PROTOCOLS)
        return "Switching Protocols";
    else if (code == PROCESSING)
        return "Processing";
    else if (code == OK)
        return "OK";
    else if (code == CREATED)
        return "Created";
    else if (code == ACCEPTED)
        return "Accepted";
    else if (code == NON_AUTHORITATIVE_INFORMATION)
        return "Non-Authoritative Information";
    else if (code == NO_CONTENT)
        return "No Content";
    else if (code == RESET_CONTENT)
        return "Reset Content";
    else if (code == PARTIAL_CONTENT)
        return "Partial Content";
    else if (code == MULTIPLE_CHOICES)
        return "Multiple Choices";
    else if (code == MOVED_PERMANENTLY)
        return "Moved Permanently";
    else if (code == FOUND)
        return "Found";
    else if (code == SEE_OTHER)
        return "See Other";
    else if (code == NOT_MODIFIED)
        return "Not Modified";
    else if (code == USE_PROXY)
        return "Use Proxy";
    else if (code == TEMPORARY_REDIRECT)
        return "Temporary Redirect";
    else if (code == BAD_REQUEST)
        return "Bad Request";
    else if (code == UNAUTHORIZED)
        return "Unauthorized";
    else if (code == PAYMENT_REQUIRED)
        return "Payment Required";
    else if (code == FORBIDDEN)
        return "Forbidden";
    else if (code == NOT_FOUND)
        return "Not Found";
    else if (code == METHOD_NOT_ALLOWED)
        return "Method Not Allowed";
    else if (code == NOT_ACCEPTABLE)
        return "Not Acceptable";
    else if (code == PROXY_AUTHENTICATION_REQUIRED)
        return "Proxy Authentication Required";
    else if (code == REQUEST_TIMEOUT)
        return "Request Timeout";
    else if (code == CONFLICT)
        return "Conflict";
    else if (code == GONE)
        return "Gone";
    else if (code == LENGTH_REQUIRED)
        return "Length Required";
    else if (code == PRECONDITION_FAILED)
        return "Precondition Failed";
    else if (code == REQUEST_ENTITY_TOO_LARGE)
        return "Request Entity Too Large";
    else if (code == REQUEST_URI_TOO_LONG)
        return "Request-URI Too Long";
    else if (code == UNSUPPORTED_MEDIA_TYPE)
        return "Unsupported Media Type";
    else if (code == REQUESTED_RANGE_NOT_SATISFIABLE)
        return "Requested Range Not Satisfiable";
    else if (code == EXPECTATION_FAILED)
        return "Expectation Failed";
    else if (code == INTERNAL_SERVER_ERROR)
        return "Internal Server Error";
    else if (code == NOT_IMPLEMENTED)
        return "Not Implemented";
    else if (code == BAD_GATEWAY)
        return "Bad Gateway";
    else if (code == SERVICE_UNAVAILABLE)
        return "Service Unavailable";
    else if (code == GATEWAY_TIMEOUT)
        return "Gateway Timeout";
    else if (code == HTTP_VERSION_NOT_SUPPORTED)
        return "HTTP Version Not Supported";
    else if (code == INSUFFICIENT_STORAGE)
        return "Insufficient Storage";
    throw HttpStatus::HttpStatusNotValidException();
}