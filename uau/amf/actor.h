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

#ifndef UAU_AMF_ACTOR_H
#define UAU_AMF_ACTOR_H


#include <memory>
#include <functional>
#include "message_handler.h"


namespace uau {
namespace amf {


class Message;
class ActorPrivate;


class Actor {
public:
    class Id;

    std::shared_ptr<Message> popFromOutput();   /*concurrent*/
    void pushToInput(std::unique_ptr<Message>); /*concurrent*/
    void activate();
    bool isActive() const;                      /*concurrent*/
    Id id() const;                              /*concurrent*/

    virtual ~Actor();                           // empty definition moved to cpp because std::unique_ptr's destructor requires full definition of ActorPrivate

protected:
    template<class... Msgs, class Derived, class FRet, class... FArgs, class... Args>
    void on(FRet (Derived::*mF)(FArgs...), Args&&... args) {
        auto h = std::bind(mF, static_cast<Derived*>(this), std::forward<Args>(args)...);
        handler.setHandlerFor<Msgs...>(std::move(h));
    }

    template<class... Msgs, class Callable, class... Args>
    typename std::enable_if<not std::is_member_function_pointer<Callable>::value>::type
    on(Callable &&f, Args&&... args) {
        auto h = std::bind(std::forward<Callable>(f), std::forward<Args>(args)...);
        handler.setHandlerFor<Msgs...>(std::move(h));
    }

    void send(std::unique_ptr<Message>);

protected:
    Actor();                                    // this class is abstract
    MessageHandler<> handler;
    std::shared_ptr<const Message> message;

protected:
    Actor(std::unique_ptr<ActorPrivate>);
    std::unique_ptr<ActorPrivate> d_ptr;
};


} // namespace amf
} // namespace uau


#endif // UAU_AMF_ACTOR_H


