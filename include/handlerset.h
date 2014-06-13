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

#ifndef UAU_HANDLER_SET_H
#define UAU_HANDLER_SET_H

#include <vector>
#include <memory>
#include "typeset.h"

namespace uau {

/**
 * @brief HandlerSet matching function
 */
template<class Derived, class Base>
bool handlerSetMatcher(Base b) {
    return dynamic_cast<Derived>(b);
}

/**
 * @brief Map of typed actions
 *
 * Usage:
 * @code
 *  class Msg1 : public SomeBase {...};
 *  class Msg2 : public SomeBase {...};
 *  class Msg3 : public SomeBase {...};
 *  class Msg4 : public SomeBase {...};
 *  class Msg5 : public SomeBase {...};
 *  class Msg6 : public SomeBase {...};
 *  class Msg7 : public SomeBase {...};
 *  class Msg8 : public SomeBase {...};
 *
 *  // handlers
 *  void handlerA() {
 *      std::cout << "handlerA() was invoked" << std::endl;
 *  }
 *
 *  void handlerB() {
 *      std::cout << "handlerB() was invoked" << std::endl;
 *  }
 *
 *  void handlerC(int param, const std::string &str) {
 *      std::cout << "handlerC(" << param << ", "  << str << ") was invoked" << std::endl;
 *  }
 *
 *  void handlerD(char ch, const SomeBase *msg, const std::string &str) {
 *      if(dynamic_cast<const Msg6 *>(msg))
 *          std::cout << "handlerD was invoked" << std::endl;
 *      else
 *          std::cout << "handlerD was invoked with unexpected message" << std::endl;
 *  }
 *
 *  class SomeClass {
 *  public:
 *      void handlerE(...) {
 *          std::cout << "SomeClass::handlerE(...) was invoked" << std::endl;
 *      }
 *  };
 *
 *  void func() {
 *      HandlerSet<SomeBase> handlers;
 *       // setup handlers
 *      handlers.setHandlerFor<Msg1>(handlerA);
 *      handlers.setHandlerFor<Msg2, Msg3, Msg4>(handlerB);
 *      handlers.setHandlerFor<Msg5>(handlerC, 1, "second");
 *      handlers.setHandlerFor<Msg6>(handlerD, 'c', std::placeholders::_1, "arg");
 *      // handlers.setHandlerFor<Msg6, Msg7>(handlerD, 'c', std::placeholders::_1, "arg");     // this not implemented yet
 *      SomeClass foo;
 *      handlers.setHandlerFor<Msg8>(&SomeClass::handlerE, &foo, ...);
 *
 *      // handling
 *      std::unique_ptr<SomeBase> msg(new Msg1);
 *      handlers.handle(msg.get());    // handlerA() was invoked
 *
 *      msg.reset(new Msg2);
 *      handlers.handle(msg.get());    // handlerB() was invoked
 *
 *      msg.reset(new Msg3);
 *      handlers.handle(msg.get());    // handlerB() was invoked
 *
 *      msg.reset(new Msg4);
 *      handlers.handle(msg.get());    // handlerB() was invoked
 *
 *      msg.reset(new Msg5);
 *      handlers.handle(msg.get());    // handlerC(1, "second") was invoked
 *
 *      msg.reset(new Msg6);
 *      handlers.handle(msg.get());    // handlerD was invoked
 *
 *      msg.reset(new Msg7);
 *      handlers.handle(msg.get());    // handlerD was invoked with unexpected message
 *
 *      msg.reset(new Msg8);
 *      handlers.handle(msg.get());    // SomeClass::handlerE(...) was invoked
 *  }
 * @endcode
 */
template<class...>
class HandlerSet;

template<class BaseType>
class HandlerSet<BaseType> {
public:
    constexpr HandlerSet() noexcept = default;
    HandlerSet(HandlerSet &&) noexcept = default;

    virtual ~HandlerSet() {}

    HandlerSet & operator = (HandlerSet &&) noexcept = default;

    bool empty() const noexcept { return _next == nullptr; }

    virtual bool handle(const BaseType *msg) {
        return _next ? _next->handle(msg) : false;
    }

    template<class... Ts, class Callable, class... Args>
    void setHandlerFor(Callable &&f, Args&&... args) {
        disable(TypeSet<Ts...>());

        if(_next)
            _next->setHandlerFor<Ts...>(std::forward<Callable>(f), std::forward<Args>(args)...);
        else
            _next.reset(new HandlerSet<BaseType, Ts...>(std::forward<Callable>(f), std::forward<Args>(args)...));
    }

protected:
    virtual void disable(const TypeSet<> &handlerTypes) {
        if(_next)
            _next->disable(handlerTypes);
    }

private:
    std::unique_ptr<HandlerSet<BaseType>> _next;
};


template<class BaseType, class T>
class HandlerSet<BaseType, T> : public HandlerSet<BaseType> {
    typedef typename std::remove_const<
        typename std::remove_pointer<T>::type
    >::type HandledType;

    typedef HandlerSet<BaseType> Parent;

public:
    template<class Callable, class... Args>
    HandlerSet(Callable &&f, Args&&... args) :
        _hnd(std::bind(std::forward<Callable>(f), std::forward<Args>(args)...)) {}

    bool handle(const BaseType *msg) override {
        if(!_disabled && handlerSetMatcher<const HandledType *>(msg)) {
            _hnd(msg);
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
    std::function<void(const BaseType *)> _hnd;

private:
    bool _disabled = false;
};


template<class BaseType, class T, class... Ts>
class HandlerSet<BaseType, T, Ts...> : public HandlerSet<BaseType, Ts...> {
    typedef typename std::remove_const<
        typename std::remove_pointer<T>::type
    >::type HandledType;

    typedef HandlerSet<BaseType, Ts...> Parent;

public:
    template<class Callable, class... Args>
    HandlerSet(Callable &&f, Args&&... args) :
        Parent(std::bind(std::forward<Callable>(f), std::forward<Args>(args)...)) {}

    bool handle(const BaseType *msg) override {
        if(!_disabled && handlerSetMatcher<const HandledType *>(msg)) {
            Parent::_hnd(msg);
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

} // namespace uau

#endif // UAU_HANDLER_SET_H
