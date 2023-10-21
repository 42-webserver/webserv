SHELL		= /bin/bash
IDX			= 0

NAME		= webserv
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -I${INCS_DIR} -std=c++98 -MD -O3
LDFLAGS		= 
RM			= rm -f

ifdef SANITIZE
	CXXFLAGS += -fsanitize=address -g3
	LDFLAGS += -fsanitize=address -g3
endif

INCS_DIR	= incs
SRCS_DIR	= srcs
OBJS_DIR	= objs

SRCS =	main.cpp \
		Config/AliasElement.cpp \
		Config/AllowMethodElement.cpp \
		Config/AutoIndexElement.cpp \
		Config/CgiPassElement.cpp \
		Config/ClientMaxBodySizeElement.cpp \
		Config/Config.cpp \
		Config/ConfigElement.cpp \
		Config/ConfigElementFactory.cpp \
		Config/ErrorPageElement.cpp \
		Config/IndexElement.cpp \
		Config/ListenElement.cpp \
		Config/LocationElement.cpp \
		Config/LocationTrieElement.cpp \
		Config/ReturnElement.cpp \
		Config/RootElement.cpp \
		Config/ServerElement.cpp \
		Config/ServerNameElement.cpp \
		Event/BufReadHandler.cpp \
		Event/EventBase/Event.cpp \
		Event/EventBase/EventFactory.cpp \
		Event/EventBase/EventHandler.cpp \
		Event/EventDto/EventDto.cpp \
		Event/EventQueue/EventQueue.cpp \
		Event/ListenEvent/ListenEvent.cpp \
		Event/ListenEvent/ListenEventHandler.cpp \
		Event/ReadEvent/ReadEvent.cpp \
		Event/ReadEvent/ReadEventFromClient.cpp \
		Event/ReadEvent/ReadEventFromClientHandler.cpp \
		Event/ReadEvent/ReadEventFromFile.cpp \
		Event/ReadEvent/ReadEventFromFileHandler.cpp \
		Event/ReadEvent/ReadEventHandler.cpp \
		Event/ReadEvent/ReadEventFromCache.cpp \
		Event/ReadEvent/ReadEventFromCacheHandler.cpp \
		Event/WriteEvent/WriteEvent.cpp \
		Event/WriteEvent/WriteEventHandler.cpp \
		Event/WriteEvent/WriteEventToClient.cpp \
		Event/WriteEvent/WriteEventToClientHandler.cpp \
		Event/WriteEvent/WriteEventToFile.cpp \
		Event/WriteEvent/WriteEventToFileHandler.cpp \
		Event/WriteEvent/WriteEventToCache.cpp \
		Event/WriteEvent/WriteEventToCacheHandler.cpp \
		Event/Exception/KqueueError.cpp \
		Http/Handler/ErrorPageHandler.cpp \
		Http/Handler/HttpCgiResponseHandler.cpp \
		Http/Handler/HttpGetResponseHandler.cpp \
		Http/Parser/HttpRequestParser.cpp \
		Http/Parser/HttpResponseParser.cpp \
		Http/Parser/ChunkedRequestBodyHandler.cpp \
		Http/Parser/FileNameGenerator.cpp \
		Http/Parser/MultipartRequestBodyHandler.cpp \
		Http/Parser/NormalBodyHandler.cpp \
		Http/Parser/RequestBodyHandler.cpp \
		Http/Request/HttpRequest.cpp \
		Http/Request/MultipartRequest.cpp \
		Http/Response/HttpResponse.cpp \
		Http/Utils/HttpStatus.cpp \
		Http/Utils/Cookie.cpp \
		Http/Utils/RouterUtils.cpp \
		Log/Logger.cpp \
		route/Match.cpp \
		route/RouteOrAliasHandler.cpp \
		Server/PhysicalServer.cpp \
		Server/PhysicalServerManager.cpp \
		Server/VirtualServer.cpp \
		Server/VirtualServerManager.cpp \
		Server/Webserv.cpp \
		Channel/ByteStream.cpp \
		Channel/CgiChannel.cpp \
		Channel/Channel.cpp \
		Channel/FileStream.cpp \
		Channel/Socket.cpp \
		Channel/SingleStreamable.cpp \
		Channel/DualStreamable.cpp \
		Buffer/Buffer/BaseBuffer.cpp \
		Buffer/Buffer/IoOnlyReadBuffer.cpp \
		Buffer/Buffer/IoReadAndWriteBuffer.cpp \
		Buffer/Node/BaseNode.cpp \
		Buffer/Node/LargeNode.cpp \
		Buffer/Node/NormalNode.cpp \
		Pattern/CgiReaderProcessor.cpp \
		Pattern/FileDeleterProcessor.cpp \
		Pattern/FileReaderProcessor.cpp \
		Pattern/FileWriterProcessor.cpp \
		Pattern/PatternProcessor.cpp \
		Pattern/RedirectionProcessor.cpp \
		Pattern/ProcessorObject.cpp \
		Client/Client.cpp \
		Client/ClientLimitExceededException.cpp \
		FileManager/Cache/cache.cpp \
		FileManager/Cache/LruCache.cpp \
		FileManager/FileManager/FileData.cpp \
		FileManager/FileManager/FileManager.cpp \
		FileManager/FileTableManager/FileTableManager.cpp \
		Hash/Sha256_copy.cpp \
		Hash/Etag.cpp \

OBJS_SUBDIR = 	${OBJS_DIR}/CGI \
				${OBJS_DIR}/Config \
				${OBJS_DIR}/Event \
				${OBJS_DIR}/Event/EventBase \
				${OBJS_DIR}/Event/EventDto \
				${OBJS_DIR}/Event/EventQueue \
				${OBJS_DIR}/Event/ListenEvent \
				${OBJS_DIR}/Event/ReadEvent \
				${OBJS_DIR}/Event/WriteEvent \
				${OBJS_DIR}/Event/Exception \
				${OBJS_DIR}/Channel \
				${OBJS_DIR}/Http \
				${OBJS_DIR}/Http/handler \
				${OBJS_DIR}/Http/parser \
				${OBJS_DIR}/Http/request \
				${OBJS_DIR}/Http/response \
				${OBJS_DIR}/Http/utils \
				${OBJS_DIR}/Http/test \
				${OBJS_DIR}/Log \
				${OBJS_DIR}/route \
				${OBJS_DIR}/Server \
				${OBJS_DIR}/Buffer/Buffer \
				${OBJS_DIR}/Buffer/Node \
				${OBJS_DIR}/Pattern \
				${OBJS_DIR}/Client \
				${OBJS_DIR}/FileManager/Cache \
				${OBJS_DIR}/FileManager/FileTableManager \
				${OBJS_DIR}/FileManager/FileManager \
				${OBJS_DIR}/FileManager/Exception \
				${OBJS_DIR}/Hash \

LIBS := ${addprefix ${LIBS_DIR}/, ${LIBS}}
SRCS := ${addprefix ${SRCS_DIR}/, ${SRCS}}
OBJS := ${SRCS:${SRCS_DIR}/%.cpp=${OBJS_DIR}/%.o}
DEPS := ${OBJS:.o=.d}

SRCS_LEN	= ${shell echo ${SRCS} | wc -w}


all: ${NAME}


${NAME}: ${OBJS}
	@if [ ${IDX} -gt 0 ]; then\
		printf "\b"; echo "done ";\
	fi
	@${CXX} ${LDFLAGS} -o ${NAME} ${OBJS}
	@echo "Build ${NAME}: done"


${OBJS_DIR}/%.o: ${SRCS_DIR}/%.cpp | ${OBJS_DIR}
	${eval IDX = ${shell expr ${IDX} + 1}}
	@if [ ${IDX} = 1 ]; then\
		echo -n "Build dependencies in ${NAME} ...  ";\
	fi
	@printf "%3d%%\b\b\b\b" ${shell expr ${IDX} \* 100 / ${SRCS_LEN}}
	@${CXX} ${CXXFLAGS} -g -c $< -o $@


${OBJS_DIR}:
	@echo "Build ${NAME}"
	@mkdir -p ${OBJS_SUBDIR}


sanitize:
	@${MAKE} -j8 SANITIZE=1 all


clean:
	@echo "Remove dependencies in ${NAME}"
	@${RM} -r ${OBJS_DIR}


fclean:
	@echo "Remove dependencies in ${NAME}"
	@${RM} -r ${OBJS_DIR}
	@echo "Remove ${NAME}"
	@${RM} ${NAME}
	

re:
	@echo "Rebuild ${NAME}"
	@${MAKE} fclean
	@${MAKE} -j8 all


.PHONY: all clean fclean re


.NOTPARALLEL: all clean fclean re


-include ${DEPS}