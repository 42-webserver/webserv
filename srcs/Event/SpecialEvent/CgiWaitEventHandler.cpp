#include <Event/SpecialEvent/CgiWaitEventHandler.hpp>
#include <Event/SpecialEvent/CgiWaitEvent.hpp>
#include <wait.h>
CgiWaitEventHandler::CgiWaitEventHandler(void) {}
CgiWaitEventHandler::~CgiWaitEventHandler(void) {}

void CgiWaitEventHandler::handleEvent(Event &event){
    CgiWaitEvent *cgiWaitEvent = static_cast<CgiWaitEvent *>(&event);
    pid_t cgiPid = cgiWaitEvent->getCgiPid();
    int status;
    if (waitpid(cgiPid, &status, WNOHANG) == cgiPid){
        try {
            cgiWaitEvent->offboardQueue();
        } catch (std::exception &e) {
            Logger::getInstance().error(e.what());
        }
    }
}