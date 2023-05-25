#ifndef HANDLERCHAIN_HPP
 #define HANDLERCHAIN_HPP

// class HandlerChain {
// public:
//     virtual ~HandlerChain() {
//         delete next_handler;
//     }
    
//     void SetNextHandler(HandlerChain* handler) {
//         next_handler = handler;
//     }
//     void goNext() {
//         next_handler->HandleRequest();
//     }
//     virtual void HandleRequest(void) = 0; 

// protected:
//     HandlerChain* next_handler;
// };

template<typename ReturnType>
class HandlerChain {
public:
    virtual ~HandlerChain() {
        delete next_handler;
    }
    
    void SetNextHandler(HandlerChain<ReturnType>* handler) {
        next_handler = handler;
    }
    
    // ReturnType GoNext() {
    //     if (next_handler) {
    //         return next_handler->HandleRequest();
    //     } else {
    //         return ReturnType();
    //     }
    // }

    virtual ReturnType HandleRequest() = 0; 

protected:
    HandlerChain<ReturnType>* next_handler;
};
#endif
