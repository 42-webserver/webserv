#ifndef PARSE_PATTERN_MATCHER_HPP
#define PARSE_PATTERN_MATCHER_HPP

#include <vector>

static const char			   	_crlf[] = "\r\n";
static const int				_crlfPatternSize = sizeof(_crlf) - 1;
static const std::vector<char>  _crlfPattern = std::vector<char>(_crlf, _crlf + _crlfPatternSize);

static const int 				_MAX_BODY_MEMORY_SIZE = 16 * 1024;
static const int				_BUFFER_SIZE = 4 * 1024;

#endif