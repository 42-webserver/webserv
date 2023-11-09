#include <Event/SpecialEvent/CgiKillEventHandler.hpp>
#include <Event/SpecialEvent/CgiKillEvent.hpp> 
#include <Log/Logger.hpp>

CgiKillEventHandler::CgiKillEventHandler(void) {}
CgiKillEventHandler::~CgiKillEventHandler(void) {}

void CgiKillEventHandler::handleEvent(Event &event) {
    CgiKillEvent *cgiKillEvent = static_cast<CgiKillEvent *>(&event);
    pid_t cgiPid = cgiKillEvent->getCgiPid();
    
    // waitpid가 0을 반환하면 자식 프로세스가 아직 종료되지 않았다는 의미입니다.
    int status;
    if (waitpid(cgiPid, &status, WNOHANG) == 0) {
        // 여전히 실행 중인 프로세스를 강제 종료합니다.
        if (kill(cgiPid, SIGKILL) == 0) {
            Logger::getInstance().error("cgi gateway timeout: kill success");
        }
        event.offboardQueue();
    } else {
        // 프로세스가 이미 종료되었으므로, 이벤트 큐에서만 제거합니다.
        event.offboardQueue();
    }
}