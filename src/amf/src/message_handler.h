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

#ifndef UAU_AMF_TYPED_ACTION_MAP_H
#define UAU_AMF_TYPED_ACTION_MAP_H


#include <vector>
#include <memory>
#include "typeset.h"


namespace uau {
namespace amf {


/**
  @brief Map of typed actions

  Usage:
  @code
    class Msg1 : public SomeBase {...};
    class Msg2 : public SomeBase {...};
    class Msg3 : public SomeBase {...};
    class Msg4 : public SomeBase {...};
    class Msg5 : public SomeBase {...};
    class Msg6 : public SomeBase {...};

    // actions
    void someHandler() {
        ...
    }

    void someOtherHandler(int param, const std::string &) {
        ...
    }

    void multiHandler(int param, const std::string &) {
        ...
    }

    class SomeClass {
    public:
        void handler(...) {
            ...
        }
    };

    void func() {
        TypedActionMap<SomeBase> handler;

        // setup actions
        handler.setHandlerFor<Msg1>(someHandler);
        handler.setHandlerFor<Msg2>(someOtherHandler, 1, "second");
        SomeClass foo;
        handler.setHandlerFor<Msg3>(&SomeClass::handler, &foo, params...);
        hanedler.setHandlerFor<Msg4, Msg5, Msg6>(multiHandler);


        // handling
        std::unique_ptr<SomeBase> msg(new Msg1);
        handler.handle(msg.get());    // someHandler()

        msg.reset(new Msg2);
        handler.handle(msg.get());    // someOtherHandler(1, "second")

        msg.reset(new Msg3);
        handler.handle(msg.get());    // foo.handler(params...)

        msg.reset(new Msg4);
        handler.handle(msg.get());    // multiHandler()

        msg.reset(new Msg5);
        handler.handle(msg.get());    // multiHandler()

        msg.reset(new Msg6);
        handler.handle(msg.get());    // multiHandler()
    }
  @endcode
*/
template<class...>
class TypedActionMap;

template<class BaseType>
class TypedActionMap<BaseType> {
public:
    constexpr TypedActionMap() noexcept = default;

    TypedActionMap(TypedActionMap &&) noexcept = default;

    TypedActionMap & operator = (TypedActionMap &&) noexcept = default;

    virtual ~TypedActionMap() {}


    bool empty() const noexcept {
        return _next == nullptr;
    }

    virtual bool handle(const BaseType *msg) {
        return _next ? _next->handle(msg) : false;
    }

    template<class... Ts, class Callable, class... Args>
    void setHandlerFor(Callable &&f, Args&&... args) {
        disable(TypeSet<Ts...>());

        if(_next)
            _next->setHandlerFor<Ts...>(std::forward<Callable>(f), std::forward<Args>(args)...);
        else
            _next.reset(new TypedActionMap<BaseType, Ts...>(std::forward<Callable>(f), std::forward<Args>(args)...));
    }

protected:
    virtual void disable(const TypeSet<> &handlerTypes) {
        if(_next)
            _next->disable(handlerTypes);
    }

private:
    std::unique_ptr<TypedActionMap<BaseType>> _next;
};


template<class BaseType, class T>
class TypedActionMap<BaseType, T> : public TypedActionMap<BaseType> {
    typedef typename std::add_const<
        typename std::remove_pointer<T>::type
    >::type HandledType;

    typedef TypedActionMap<BaseType> Parent;

public:
    template<class Callable, class... Args>
    TypedActionMap(Callable &&f, Args&&... args) :
        _hnd(std::bind(std::forward<Callable>(f), std::forward<Args>(args)...)) {}

    bool handle(const BaseType *msg) override {
        if(!_disabled && dynamic_cast<HandledType*>(msg)) {
            _hnd();
            return true;
        }

        return Parent::handle(msg);
    }

protected:
    void disable(const TypeSet<> &handlerTypes) override {
        _disabled = handlerTypes.contains<T>();
        Parent::disable(handlerTypes);
    }

protected:
    std::function<void()> _hnd;

private:
    bool _disabled = false;
};


template<class BaseType, class T, class... Ts>
class TypedActionMap<BaseType, T, Ts...> : public TypedActionMap<BaseType, Ts...> {
    typedef typename std::add_const<
        typename std::remove_pointer<T>::type
    >::type HandledType;

    typedef TypedActionMap<BaseType, Ts...> Parent;

public:
    template<class Callable, class... Args>
    TypedActionMap(Callable &&f, Args&&... args) :
        Parent(std::bind(std::forward<Callable>(f), std::forward<Args>(args)...)) {}

    bool handle(const BaseType *msg) override {
        if(!_disabled && dynamic_cast<HandledType*>(msg)) {
            Parent::_hnd();
            return true;
        }

        return Parent::handle(msg);
    }

protected:
    void disable(const TypeSet<> &handlerTypes) override {
        _disabled = handlerTypes.contains<T>();
        Parent::disable(handlerTypes);
    }

private:
    bool _disabled = false;
};


} // namespace amf
} // namespace uau


#endif // UAU_AMF_TYPED_ACTION_MAP_H
