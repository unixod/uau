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

#ifndef UAU_AMF_CORE_ENVELOPE_H
#define UAU_AMF_CORE_ENVELOPE_H


#include <type_traits>
#include "envelope_fwd.h"
#include "handlerset.h"


namespace uau {
namespace amf {
namespace core {

//template<class T = void, class = void>
//class Envelope;

template<>
class Envelope<>{
public:
    template<class T>
    const T * is() {
//        if(auto content = dynamic_cast<
//                            typename std::add_pointer<
//                                Envelope<T>
//                            >::type
//                           >(this)) {
//            return &content->message();
//        } else {
            return nullptr;
//        }
    }

    template<class T>
    const T * is() const {
//        if(auto content = dynamic_cast<
//                            typename std::add_pointer<
//                                typename std::add_const<
//                                    Envelope<T>
//                                >::type
//                            >::type
//                           >(this)) {
//            return &content->message();
//        } else {
            return nullptr;
//        }
    }

    virtual ~Envelope() {}
};

template<class Message>
class Envelope<Message, typename std::enable_if<std::is_class<Message>::value>::type> : public Envelope<>, public Message {
public:
    typedef Message type;

public:
    template<class... Args>
    Envelope(Args&&... args) :
        Message{std::forward<Args>(args)...} {}

    Message & message() {
        return *this;
    }

    const Message & message() const {
        return *this;
    }
};

template<class Message>
class Envelope<Message, typename std::enable_if<not std::is_class<Message>::value>::type> : public Envelope<> {
public:
    typedef Message type;

public:
    Envelope() = default;
    Envelope(const Envelope &) = default;
    Envelope(Envelope &&) = default;

    Envelope & operator = (const Envelope &) = default;
    Envelope & operator = (Envelope &&) = default;

    Envelope(Message&& msg) :
        _message(msg) {}

    Message & message() & {
        return _message;
    }

    const Message & message() const & {
        return _message;
    }

    Message message() && {
        return std::move(_message);
    }

private:
    Message _message;
};

//template<class Message>
//class Envelope<Message> : public Envelope<> {
//public:
//    Envelope() = default;
//    Envelope(const Envelope &) = default;
//    Envelope(Envelope &&) = default;

//    Envelope & operator = (const Envelope &) = default;
//    Envelope & operator = (Envelope &&) = default;

//    Envelope(Message&& msg) :
//        _message(msg) {}

//    Message & message() & {
//        return _message;
//    }

//    const Message & message() const & {
//        return _message;
//    }

//    Message message() && {
//        return std::move(_message);
//    }

//private:
//    Message _message;
//};

//template<class T, class... Ts>
//class Envelope<T, Ts...>;


template<class...>
struct is_envelope;

template<class T>
struct is_envelope<T> : public is_envelope<
        typename std::remove_cv<
            typename std::remove_pointer<
                typename std::remove_reference<T>::type
            >::type
        >::type, void> {};

template<class T>
struct is_envelope<T, void> : public std::false_type {};

template<class T>
struct is_envelope<Envelope<T>, void> : public std::true_type {};


template<class...>
struct wrapped_type;

template<class T>
struct wrapped_type<T> : public wrapped_type<
        typename std::remove_cv<
            typename std::remove_pointer<
                typename std::remove_reference<T>::type
            >::type
        >::type, void> {};

template<class T>
struct wrapped_type<Envelope<T>, void> {
    typedef T type;
};

template<template<class> class Mod, bool cond, class T>
struct apply_if {
    typedef typename std::conditional<cond, typename Mod<T>::type, T>::type type;
};

template<class From, class To>
struct apply_cvalifiers {
    typedef typename apply_if<std::add_pointer, std::is_pointer<From>::value,
        typename apply_if<std::add_lvalue_reference, std::is_lvalue_reference<From>::value,
            typename apply_if<std::add_const, std::is_const<From>::value, To>::type
        >::type
    >::type type;
};

/**
 *  enelope_cast similar as dynamic_cast, but in addition
 *  casts to integral types too.
 */

template<class T, class E>
typename std::enable_if<std::is_reference<T>::value, T>::type
envelope_cast(E & envelope) {
}

template<class T, class E>
typename std::enable_if<std::is_pointer<T>::value, T>::type
enevelope_cast(E * envelope) {
    return nullptr;
}

template<class T, class E>
typename std::enable_if<std::is_integral<T>::value, T>::type
envelope_cast(E envelope) {
//    if (auto t = dynamic_cast<E<T>>(envelope)) {

//    }
}

} // namespace core
} // namespace amf

template<class T, class Payload = typename amf::core::apply_cvalifiers<T, typename amf::core::wrapped_type<T>::type>::type>
bool handlerSetMatcher(const amf::core::Envelope<> *envelope) {
    return dynamic_cast<Payload>(envelope);
}

///**
// * @brief HandlerSet's matcher specialization
// */
//template<class T>
//typename std::enable_if<amf::core::is_envelope<T>::value, bool>::type
//handlerSetMatcher(amf::core::Envelope<> * b) {
//    return dynamic_cast<typename amf::core::wrapped_type<T>::type *>(b);
//}

//template<class T>
//typename std::enable_if<amf::core::is_envelope<T>::value, bool>::type
//handlerSetMatcher(const amf::core::Envelope<> * b) {
//    return dynamic_cast<const typename amf::core::wrapped_type<T>::type *>(b);
//}

} // namespace uau


#endif // UAU_AMF_CORE_ENVELOPE_H
