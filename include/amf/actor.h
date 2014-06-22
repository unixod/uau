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
#include "core/abstract_actor.h"
#include "core/envelope.h"
#include "handlerset.h"

namespace uau {
namespace amf {

class ActorPrivate;

class Actor : public core::AbstractActor {
public:
    virtual ~Actor();

protected:
    Actor();

    template<class... Msgs, class Derived, class FRet, class... FArgs, class... Args>
    void on(FRet (Derived::*mF)(FArgs...), Args&&... args) {
        _handler.setHandlerFor<core::Envelope<Msgs>...>(mF, static_cast<Derived*>(this), std::forward<Args>(args)...);
    }

    template<class... Msgs, class Callable, class... Args>
    typename std::enable_if<not std::is_member_function_pointer<Callable>::value>::type
    on(Callable &&f, Args&&... args) {
        _handler.setHandlerFor<core::Envelope<Msgs>...>(std::forward<Callable>(f), std::forward<Args>(args)...);
    }

    template<class T>
    void send(T &&payload) {
        sendEnvelope(
            std::make_shared<const core::Envelope<T>>(
                std::forward<T>(payload)
            )
        );
    }

    /**
     * @brief get received message
     * @return received message
     */
    AbstractActor::Message message() const;

    template<class T>
    T messagePayload() {
//        return message()->payload<T>();
//        static_assert(std::integral_constant<bool,
//                        std::is_const<T>::value ||
//                        !std::is_pointer<T>::value>::value, "only const pointers are allowed");

//        static_assert(std::integral_constant<bool,
//                        std::is_const<T>::value ||
//                        !std::is_reference<T>::value>::value, "only const reference are allowed");

//        core::Envelope<
//                typename std::remove_pointer<
//                    typename std::remove_reference<
//                        typename std::remove_cv<T>::type
//                    >::type
//                >::type>
    }

//    template<class T>
//    typename amf::core::when_not_ingeritable<T, T>::type messagePayload() {
//        static_assert(std::integral_constant<bool,
//                        std::is_const<T>::value ||
//                        !std::is_pointer<T>::value>::value, "only const pointers are allowed");

//        static_assert(std::integral_constant<bool,
//                        std::is_const<T>::value ||
//                        !std::is_reference<T>::value>::value, "only const reference are allowed");
//    }

private:
    // core::AbstractActor //
    void push(AbstractActor::Id, AbstractActor::Message) final override;    /*concurrent*/
    AbstractActor::Message pull() final override;                           /*concurrent*/
    AbstractActor::Message tryToPull() final override;                      /*concurrent*/

    int inputMessageQueueSize() const override;
    int outputMessageQueueSize() const override;

    void activate() final override;                                         // blocks the current thread until the input queue is empty
    bool tryActivate() final override;

private:
    void sendEnvelope(AbstractActor::Message &&);
    // TODO: move this member to private class
    HandlerSet<core::Envelope<>> _handler;

protected:
    Actor(std::unique_ptr<ActorPrivate>);
    std::unique_ptr<ActorPrivate> d_ptr;
};

} // namespace amf
} // namespace uau

#endif // UAU_AMF_ACTOR_H
