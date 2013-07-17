/*
    UAU (Useful Algorithms and Utilities) - C++11 template library

    This file is part of UAU

    Feel free to contribute to this project at:
        http://github.com/unixod/uau

    Copyright (c) 2012, Eldar Zakirov <unixod@gmail.com>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
     2. Redistributions in binary form must reproduce the above
        copyright notice, this list of conditions and the following
        disclaimer in the documentation and/or other materials provided
        with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
    OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
    OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    The views and conclusions contained in the software and documentation are
    those of the authors and should not be interpreted as representing official
    policies, either expressed or implied, of Eldar Zakirov.
*/

#ifndef UAU_AMF_IMPL_MESSAGE_HANDLER_H
#define UAU_AMF_IMPL_MESSAGE_HANDLER_H


#include <vector>
#include <memory>


namespace uau {
namespace amf {


class Message;


/**
  @brief Message handler

  Usage:
  @code
    class Msg1 : public uau::amf::Message{...};
    class Msg2 : public uau::amf::Message{...};
    class Msg3 : public uau::amf::Message{...};
    class Msg4 : public uau::amf::Message{...};
    class Msg5 : public uau::amf::Message{...};
    class Msg6 : public uau::amf::Message{...};

    void someHandler(){
        ...
    }

    void someOtherHandler(int param, const std::string &){
        ...
    }

    void multiHandler(int param, const std::string &){
        ...
    }

    class SomeClass{
    public:
        void handler(...){
            ...
        }
    };

    void func(){
        MessageHandler<> handler;

        // setup handlers
        handler.setHandlerFor<Msg1>(someHandler);
        handler.setHandlerFor<Msg2>(someOtherHandler, 1, "second");
        SomeClass foo;
        handler.setHandlerFor<Msg3>(&SomeClass::handler, &foo, params...);
        hanedler.setHandlerFor<Msg4, Msg5, Msg6>(multiHandler);


        // handling
        Message *msg;

        msg = new Msg1;
        handler.handle(msg);    // someHandler()

        msg = new Msg2;
        handler.handle(msg);    // someOtherHandler(1, "second")

        msg = new Msg3;
        handler.handle(msg);    // foo.handler(params...)

        msg = new Msg4;
        handler.handle(msg);    // multiHandler()

        msg = new Msg5;
        handler.handle(msg);    // multiHandler()

        msg = new Msg6;
        handler.handle(msg);    // multiHandler()
    }
  @endcode
*/
template<class...>
class MessageHandler;

template<>
class MessageHandler<> {
public:
    virtual bool handle(Message *msg){
        return next ? next->handle(msg) : false;
    }

    template<class... Ts, class Callable, class... Args>
    void setHandlerFor(Callable &&f, Args&&... args){
        if(next)
            next->setHandlerFor<Ts...>(std::forward<Callable>(f), std::forward<Args>(args)...);
        else
            next.reset(new MessageHandler<Ts...>(std::forward<Callable>(f), std::forward<Args>(args)...));
    }

private:
    std::unique_ptr<MessageHandler<>> next;
};

template<class T>
class MessageHandler<T> : public MessageHandler<> {
public:
    template<class Callable, class... Args>
    MessageHandler(Callable &&f, Args&&... args) :
        hnd(std::bind(std::forward<Callable>(f), std::forward<Args>(args)...)) {}

    bool handle(Message *msg) {
        if(dynamic_cast<T*>(msg)){
            hnd();
            return true;
        }

        return MessageHandler<>::handle(msg);
    }

protected:
    std::function<void()> hnd;
};

template<class T, class... Ts>
class MessageHandler<T, Ts...> : public MessageHandler<Ts...> {
public:
    template<class Callable, class... Args>
    MessageHandler(Callable &&f, Args&&... args) :
        MessageHandler<Ts...>(std::bind(std::forward<Callable>(f), std::forward<Args>(args)...)) {}

    bool handle(Message *msg) {
        if(dynamic_cast<T*>(msg)) {
//            hnd();                    // error - why I must use MessageHandler<Ts...>::hnd(); instead?
            MessageHandler<Ts...>::hnd();
            return true;
        }

        return MessageHandler<Ts...>::handle(msg);
    }
};


} // namespace amf
} // namespace uau


#endif // UAU_AMF_IMPL_MESSAGE_HANDLER_H


