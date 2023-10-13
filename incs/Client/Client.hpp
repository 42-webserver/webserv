#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <Http/Request/HttpRequest.hpp>
#include <Http/Response/HttpResponse.hpp>
#include <Http/Utils/HttpMethod.hpp>
#include <Server/VirtualServerManager.hpp>
#include <Server/VirtualServer.hpp>
#include <Http/Utils/RouterUtils.hpp>
#include "../../libs/shared_ptr/shared_ptr.hpp"
#include "../../libs/Library/Optional.hpp"
#include <queue>
#include <Pattern/PatternProcessor.hpp>
#include <Client/ClientIdManager.hpp>
static size_t MAX_QUEUE_SIZE = 10;

typedef enum {
    InWriting,
    InReading,
    StateCount
}   e_client_queue_state;

class Client{
private:
    std::queue<ft::shared_ptr<HttpRequest> > requests;
    ft::Optional<ft::shared_ptr<HttpResponse> > response;
    bool _queueState[StateCount];
    int _id;
public:
    Client();
    ~Client();
    void addRequest(ft::shared_ptr<HttpRequest> request);
    bool isRequestEmpty(void);
    bool isResponseEmpty(void);
    bool isQueueMax(void);
    bool isInEventQueue(e_client_queue_state state);
    void build();
private:
    PatternType getPatternType(ft::shared_ptr<VirtualServerManager> vsm);
};

#endif