#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <Http/Request/HttpRequest.hpp>
#include <Http/Response/HttpResponse.hpp>
#include <Http/Utils/HttpMethod.hpp>
#include <Server/VirtualServerManager.hpp>
#include <Server/VirtualServer.hpp>
#include <Http/Utils/RouterUtils.hpp>
#include "../../libs/shared_ptr/shared_ptr.hpp"
#include "../../libs/Library/Optional.hpp"
#include <queue>
#include <Pattern/PatternType.hpp>

static const size_t MAX_QUEUE_SIZE = 15;

typedef enum {
    None       = 0b00, // 00
    Write      = 0b01, // 01
    Read       = 0b10, // 10
    ReadWrite  = 0b11, // 11
} e_client_event_queue_state;

class Client{
private:
    std::queue<ft::shared_ptr<HttpRequest> > requests;
    ft::Optional<ft::shared_ptr<HttpResponse> > response;
    e_client_event_queue_state _eventQueueState;
private:
    Client(const Client& other);
    Client& operator=(const Client& other);
public:
    Client(
    e_client_event_queue_state eventQueueState = None);
    ~Client();
    // bool operator==(const Client& other) const;
    void clientKill(void);
    void addRequest(ft::shared_ptr<HttpRequest> request);
    bool isRequestEmpty(void);
    bool isResponseEmpty(void);
    bool isQueueMax(void);
    bool isClientDie(void);
    bool isFinalRequest(void);
    ft::shared_ptr<HttpRequest> getRequest(void);
    void processCurrentRequestDone(void);
    bool isEventQueueStateTurnOn(e_client_event_queue_state state);
    e_client_event_queue_state queryClientEventQueueState(void);
    void addClientEventQueueState(e_client_event_queue_state state);
    void removeClientEventQueueState(e_client_event_queue_state state);
    void allocateResponse(void);
    // void deallocateResponse(void);
    void clearResponseAndRequest(void);
    ft::shared_ptr<HttpResponse> getResponse(void);
    void setResponse(ft::shared_ptr<HttpResponse> response);
    // const Client* getClientId(void) const;
public:
    PatternType getPatternType(const ft::shared_ptr<VirtualServerManager>& vsm) const;
};

#endif