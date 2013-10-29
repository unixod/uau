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


namespace uau {
namespace amf {
namespace core {

template<class...>
class Envelope{
public:
    template<class T>
    const T * is() {
        if(auto content = dynamic_cast<
                            typename std::add_pointer<
                                Envelope<T>
                            >::type
                           >(this)) {
            return &content->message();
        } else {
            return nullptr;
        }
    }

    template<class T>
    const T * is() const {
        if(auto content = dynamic_cast<
                            typename std::add_pointer<
                                typename std::add_const<
                                    Envelope<T>
                                >::type
                            >::type
                           >(this)) {
            return &content->message();
        } else {
            return nullptr;
        }
    }

    virtual ~Envelope() {}
};

template<class Message>
class Envelope<Message> : public Envelope<> {
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

template<class T, class... Ts>
class Envelope<T, Ts...>;


} // namespace core
} // namespace amf
} // namespace uau


#endif // UAU_AMF_CORE_ENVELOPE_H
