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
using is_inheritable = std::is_class<typename
                        std::remove_pointer<typename
                            std::remove_reference<T>::type>::type>;

// TODO: in future, when c++14 will become available, use std::is_final with std::is_class together
//template<class T>
//struct is_inheritable : std::integral_constant<bool,
//                            std::is_class<T>::value &&
//                            !std::is_final<T>::value> {};

template<class T, class U = void>
using when_inheritable = typename std::enable_if<is_inheritable<T>::value, U>;

template<class T, class U = void>
using when_not_inheritable = typename std::enable_if<not is_inheritable<T>::value, U>;


template<>
class Envelope<>{
public:
    template<class T>
    typename std::enable_if<std::is_pointer<T>::value,
        typename when_inheritable<T, T>::type>::type
    payload() const {
        static_assert(std::is_const<typename
                        std::remove_pointer<T>::type>::value,
                      "only const pointers are allowed");

        return dynamic_cast<T>(this);
    }

    template<class T>
    typename std::enable_if<std::is_reference<T>::value,
        typename when_inheritable<T, T>::type>::type
    payload() const {
        static_assert(std::is_const<typename
                        std::remove_reference<T>::type>::value,
                      "only const references or pointers are allowed for inheritable types");

        return dynamic_cast<T>(*this);
    }

    template<class T>
    typename std::enable_if<std::is_pointer<T>::value,
        typename when_not_inheritable<T, T>::type>::type
    payload() const {
        static_assert(std::is_const<typename
                        std::remove_pointer<T>::type>::value,
                      "only const pointers are allowed");

        typedef Envelope<typename
                    std::remove_cv<typename
                        std::remove_pointer<T>::type>::type> E;

        if (auto envelope = dynamic_cast<const E *>(this)) {
            return &envelope->_payload;
        } else {
            return nullptr;
        }
    }

    template<class T>
    typename std::enable_if<std::is_reference<T>::value,
        typename when_not_inheritable<T, T>::type>::type
    payload() const {
        static_assert(std::is_const<typename
                        std::remove_reference<T>::type>::value,
                      "only const references are allowed");

        typedef Envelope<typename
                    std::remove_cv<typename
                        std::remove_reference<T>::type>::type> E;

        if (auto envelope = dynamic_cast<const E *>(this)) {
            return envelope->_payload;
        } else {
            throw std::bad_cast{};
        }
    }

    template<class T, class... Args>
    typename std::enable_if<std::integral_constant<bool,
                                !std::is_pointer<T>::value &&
                                !std::is_reference<T>::value>::value,
        typename when_not_inheritable<T, T>::type>::type
    payload(Args&&... args) const {
        typedef Envelope<typename
                    std::remove_cv<T>::type> E;

        if (auto envelope = dynamic_cast<const E *>(this)) {
            return envelope->_payload;
        } else {
            return {std::forward<Args>(args)...};
        }
    }

    virtual ~Envelope() {}
};

template<class Payload>
class Envelope<Payload, typename when_inheritable<Payload>::type>
        : public Envelope<>, public Payload { // for the future is_final
public:
    typedef Payload type;

    static_assert(!std::is_pointer<type>::value,
                  "uau::amf::core::Envelope<T>, T must not be pointer type");
    static_assert(!std::is_reference<type>::value,
                  "uau::amf::core::Envelope<T>, T must not be reference type");
    static_assert(!std::integral_constant<bool,
                    std::is_const<type>::value ||
                    std::is_volatile<type>::value>::value,
                  "uau::amf::core::Envelope<T>, T must not have any cv-qualifiers");

public:
    template<class... Args>
    Envelope(Args&&... args) :
        Payload(std::forward<Args>(args)...) {}
};

template<class Payload>
class Envelope<Payload, typename when_not_inheritable<Payload>::type>
        : public Envelope<> {
public:
    typedef Payload type;

    static_assert(!std::is_pointer<type>::value,
                  "uau::amf::core::Envelope<T>, T must not be pointer type");
    static_assert(!std::is_reference<type>::value,
                  "uau::amf::core::Envelope<T>, T must not be reference type");
    static_assert(!std::integral_constant<bool,
                    std::is_const<type>::value ||
                    std::is_volatile<type>::value>::value,
                  "uau::amf::core::Envelope<T>, T must not have any cv-qualifiers");

public:
    Envelope() = default;
    Envelope(const Envelope &) = default;
    Envelope(Envelope &&) = default;

    Envelope & operator = (const Envelope &) = default;
    Envelope & operator = (Envelope &&) = default;

    template<class... Args>
    Envelope(Args&&... args) :
        _payload{std::forward<Args>(args)...} {}

public:
    Payload _payload;
};

} // namespace core
} // namespace amf


/**
 * @brief HandlerSet's matcher specializations
 */
template<class T>
typename amf::core::when_inheritable<T, bool>::type
handlerSetMatcher(const amf::core::Envelope<> * b) {
    sizeof(amf::core::Envelope<T>); // turn on assertions

    return dynamic_cast<typename
                std::add_pointer<typename
                    std::add_const<T>::type>::type>(b);
}

template<class T>
typename amf::core::when_not_inheritable<T, bool>::type
handlerSetMatcher(const amf::core::Envelope<> * b) {
    return dynamic_cast<const amf::core::Envelope<T> *>(b);
}

} // namespace uau


#endif // UAU_AMF_CORE_ENVELOPE_H
