#ifndef HANDLERCHAIN_HPP
 #define HANDLERCHAIN_HPP

class HandlerChain {
public:
    virtual ~HandlerChain() {
        delete next_handler;
    }
    
    void SetNextHandler(HandlerChain* handler) {
        next_handler = handler;
    }
    
    virtual void HandleRequest(int request) = 0; 

protected:
    HandlerChain* next_handler;
};

#endif
