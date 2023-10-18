#ifndef FTUTIL_HPP
# define FTUTIL_HPP

//Todo : event Key넣는 형식으로 Enum <- ServerEvent에 있는거
namespace ft {
	const int bufSize = 4 * 1024;
		enum EventType {
			READ_EVENT_FROM_CLIENT,
			WRITE_EVENT_TO_CLIENT,
			READ_EVENT_FROM_CGI,
			WRITE_EVENT_TO_GCI,
			LISTEN_EVENT,
			FILE_READ_EVENT,
			FILE_WRITE_EVENT,
			CACHE_READ_EVENT,
			CACHE_WRITE_EVENT,
			COUNT
		};
};

#endif