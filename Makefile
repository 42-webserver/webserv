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
		Event/Event.cpp \
		Event/ListenEvent.cpp \
		Event/ReadEvent.cpp \
		Event/TimeEvent.cpp \
		Event/WriteEvent.cpp \
		EventQueue/EventQueue.cpp \
		FileDescriptor/FileDescriptor.cpp \
		Http/Handler/ErrorPageHandler.cpp \
		Http/Parser/HttpRequestParser.cpp \
		Http/Parser/HttpResponseParser.cpp \
		Http/Parser/ChunkedRequestBodyHandler.cpp \
		Http/Parser/FileNameGenerator.cpp \
		Http/Parser/MultipartRequestBodyHandler.cpp \
		Http/Parser/NormalBodyHandler.cpp \
		Http/Parser/RequestBodyHandler.cpp \
		Http/Request/HttpRequest.cpp \
		Http/Response/HttpResponse.cpp \
		Http/Utils/HttpStatus.cpp \
		Http/Utils/Cookie.cpp \
		Http/Utils/FileUploader.cpp \
		Http/Utils/RouterUtils.cpp \
		Log/Logger.cpp \
		route/Match.cpp \
		route/RouteOrAliasHandler.cpp \
		Server/PhysicalServer.cpp \
		Server/PhysicalServerManager.cpp \
		Server/VirtualServer.cpp \
		Server/VirtualServerManager.cpp \
		Server/Webserv.cpp \
		Socket/Socket.cpp \
		EventDto/EventDto.cpp \
		Event/BufReadHandler.cpp \

OBJS_SUBDIR = 	${OBJS_DIR}/CGI \
				${OBJS_DIR}/Config \
				${OBJS_DIR}/Event \
				${OBJS_DIR}/EventQueue\
				${OBJS_DIR}/FileDescriptor \
				${OBJS_DIR}/http \
				${OBJS_DIR}/Http/handler \
				${OBJS_DIR}/Http/parser \
				${OBJS_DIR}/Http/request \
				${OBJS_DIR}/Http/response \
				${OBJS_DIR}/Http/utils \
				${OBJS_DIR}/Http/test \
				${OBJS_DIR}/Log \
				${OBJS_DIR}/route \
				${OBJS_DIR}/Server \
				${OBJS_DIR}/Socket \
				${OBJS_DIR}/EventDto \
				
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