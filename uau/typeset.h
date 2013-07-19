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

#ifndef UAU_TYPESET_H
#define UAU_TYPESET_H


namespace uau {


/**
 * @brief Encapsulates set of types
 *
 * Usage:
 * @code
 *      class First;
 *      class Second;
 *      class Third;
 *      class Fifth;
 *
 *      void func(const uau::TypeSet<> &ts) {
 *          std::cout << std::boolalpha;
 *          std::cout << "ts.contains<const First>() == "
 *                          << ts.contains<const First>() << std::endl;
 *          std::cout << "ts.contains<volatile Second, Fifth &, const First>() == "
 *                          << ts.contains<volatile Second, Fifth &, const First>() << std::endl;
 *          std::cout << "ts.contains<int, char, Third *>() == "
 *                          << ts.contains<int, char, Third *>() << std::endl;
 *          std::cout << "ts.contains<char, long>() == "
 *                          << ts.contains<char, long>() << std::endl;
 *          std::cout << "ts.contains<float>() == "
 *                          << ts.contains<float>() << std::endl;
 *          std::cout << "ts.contains<double>() == "
 *                          << ts.contains<double>() << std::endl;
 *      }
 *
 *      func(uau::TypeSet<const First, volatile Second, Third *, Fifth &, int, char>());
 *
 *      // ts.contains<const First>() == true
 *      // ts.contains<volatile Second, Fifth &, const First>() == true
 *      // ts.contains<int, char, Third *>() == true
 *      // ts.contains<char, long>() == false
 *      // ts.contains<float>() == false
 *      // ts.contains<double>() == false
 * @endcode
 */
template<class...>
class TypeSet {
public:
    template<class T>
    bool contains() const noexcept {
        return dynamic_cast<const TypeSet<T> *>(this);
    }

    template<class T, class T1, class... Ts>
    bool contains() const noexcept {
        return dynamic_cast<const TypeSet<T> *>(this) && contains<T1, Ts...>();
    }

    virtual ~TypeSet() {}

    TypeSet & operator = (TypeSet &&) = delete;
};

template<class T>
class TypeSet<T> : public virtual TypeSet<> {};

template<class T, class... Ts>
class TypeSet<T, Ts...> : private TypeSet<Ts...>, public TypeSet<T> {};


} // namespace uau


#endif // UAU_TYPESET_H


