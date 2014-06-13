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

// Helpers
template<class T>
using is_inheritable = std::is_class<T>;

// TODO: in future, when c++14 will become available, use std::is_final with std::is_class together
//template<class T>
//struct is_inheritable : std::integral_constant<bool,
//                            std::is_class<T>::value &&
//                            !std::is_final<T>::value> {};

template<class T, class U = void>
using when_ingeritable = typename std::enable_if<is_inheritable<T>::value, U>;

template<class T, class U = void>
using when_not_ingeritable = typename std::enable_if<not is_inheritable<T>::value, U>;


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

template<class Payload>
class Envelope<Payload, typename when_ingeritable<Payload>::type> : public Envelope<>, public Payload { // for the future is_final
public:
    typedef Payload type;

    static_assert(!std::is_pointer<type>::value, "uau::amf::core::Envelope<T>, T must not be pointer type");
    static_assert(!std::is_reference<type>::value, "uau::amf::core::Envelope<T>, T must not be reference type");
    static_assert(!std::integral_constant<bool,
                    std::is_const<type>::value ||
                    std::is_volatile<type>::value>::value, "uau::amf::core::Envelope<T>, T must not have any cv-qualifiers");

public:
    template<class... Args>
    Envelope(Args&&... args) :
        Payload{std::forward<Args>(args)...} {}

    Payload & payload()
    {
        return *this;
    }

    const Payload & payload() const
    {
        return *this;
    }
};

template<class Payload>
class Envelope<Payload, typename when_not_ingeritable<Payload>::type> : public Envelope<> {
public:
    typedef Payload type;

    static_assert(!std::is_pointer<type>::value, "uau::amf::core::Envelope<T>, T must not be pointer type");
    static_assert(!std::is_reference<type>::value, "uau::amf::core::Envelope<T>, T must not be reference type");
    static_assert(!std::integral_constant<bool,
                    std::is_const<type>::value ||
                    std::is_volatile<type>::value>::value, "uau::amf::core::Envelope<T>, T must not have any cv-qualifiers");

public:
    Envelope() = default;
    Envelope(const Envelope &) = default;
    Envelope(Envelope &&) = default;

    Envelope & operator = (const Envelope &) = default;
    Envelope & operator = (Envelope &&) = default;

    template<class... Args>
    Envelope(Args&&... args) :
        _payload{std::forward<Args>(args)...} {}

    Payload & payload() &
    {
        return _payload;
    }

    const Payload & payload() const &
    {
        return _payload;
    }

    Payload payload() &&
    {
        return std::move(_payload);
    }

private:
    Payload _payload;
};


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


/**
 * @brief HandlerSet's matcher specializations
 */
template<class T>
typename amf::core::when_ingeritable<T, bool>::type
handlerSetMatcher(const amf::core::Envelope<> * b) {
    sizeof(amf::core::Envelope<T>); // turn on assertions

    return dynamic_cast<
            typename std::add_pointer<
                typename std::add_const<T>::type
            >::type
           >(b);
}

template<class T>
typename amf::core::when_not_ingeritable<T, bool>::type
handlerSetMatcher(const amf::core::Envelope<> * b) {
    return dynamic_cast<
            const amf::core::Envelope<T> *
           >(b);
}

} // namespace uau


#endif // UAU_AMF_CORE_ENVELOPE_H
