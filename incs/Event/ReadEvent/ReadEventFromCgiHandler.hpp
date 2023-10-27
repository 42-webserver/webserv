#ifndef READEVENTFROMCGIHANDLER_HPP
# define READEVENTFROMCGIHANDLER_HPP

# include "ReadEventHandler.hpp"
# include <vector>
#include <unistd.h>
#include <Http/Response/ErrorPageBuilder.hpp>
#include <map>
#include <algorithm>
#include <cstring>
#include <Http/Response/CgiResponseBuilder.hpp>
class ReadEventFromCgi;
class ReadEventFromCgiHandler: public ReadEventHandler{
    public:
        typedef enum{
            e_parse_cgi_start,
            e_parse_cgi_header,
            e_parse_cgi_body,
            e_parse_cgi_end
        }   e_parse_cgi_state;

    public:
        ReadEventFromCgiHandler();
            virtual	~ReadEventFromCgiHandler(void);

    public:
        virtual void   handleEvent(Event &event);
    private:
        void            _handleCgiStart(ReadEventFromCgi &readEventFromCgi);
        void          _handleCgiHeader(ReadEventFromCgi &readEventFromCgi);
        void          _handleCgiBody(ReadEventFromCgi &readEventFromCgi);
        void          _handleCgiEnd(ReadEventFromCgi &readEventFromCgi);
        void           _buildCgiResponseHeader(ReadEventFromCgi &readEventFromCgi);
        void            _parseToBody(ReadEventFromCgi &readEventFromCgi);
        void            _addHeader(const std::string& key, const std::string& value);
        std::string           _spaceTrim(const std::string& str);
        bool            _checkStartBody(std::vector<char>::iterator find, std::vector<char>::iterator end);
        std::vector<char>::iterator _begin();
        std::vector<char>::iterator _end();

    public:
        std::map<std::string, std::string> &getHeaders();
    private:
        std::vector<char> _buffer;
        e_parse_cgi_state _state;
        size_t            _realSize;
        std::map<std::string, std::string> _headers;
        size_t          _previousReadSize;
        bool            _firstInCgiBody;
        bool            _isContentLengthSet;
};

#endif