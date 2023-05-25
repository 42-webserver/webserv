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
		config/Config.cpp \
		event/Event.cpp \
		event/ListenEvent.cpp \
		event/ReadEvent.cpp \
		event/TimeEvent.cpp \
		event/WriteEvent.cpp \
		http/handler/ErrorPageHandler.cpp \
		http/handler/HttpRequestHandler.cpp \
		http/parser/HttpRequestParser.cpp \
		http/parser/HttpResponseParser.cpp \
		http/request/HttpRequest.cpp \
		http/response/HttpResponse.cpp \
		http/utils/HttpStatus.cpp \
		http/utils/Cookie.cpp \
		kqueue/Kqueue.cpp \
		server/Location.cpp \
		server/Server.cpp \
		server/Webserv.cpp \
		socket/Socket.cpp \

OBJS_SUBDIR = 	${OBJS_DIR}/cgi \
				${OBJS_DIR}/config \
				${OBJS_DIR}/event \
				${OBJS_DIR}/http \
				${OBJS_DIR}/http/handler \
				${OBJS_DIR}/http/parser \
				${OBJS_DIR}/http/request \
				${OBJS_DIR}/http/response \
				${OBJS_DIR}/http/utils \
				${OBJS_DIR}/http/test \
				${OBJS_DIR}/kqueue \
				${OBJS_DIR}/server \
				${OBJS_DIR}/socket

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