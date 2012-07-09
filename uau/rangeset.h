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

#ifndef RANGESET_H
#define RANGESET_H

#include <list>
#include <algorithm>

namespace uau{

/**
  @brief Represents of ranges sequence

  Usage:
  @code
    void func(){
        uau::RangeSet<int> rng;
        rng.assign_range(0, 10);    //encompasses one range: {0, 10} inclusive
        rng.exclude(5, 8);          //now rng encompasses two ranges: {0, 4} and {9, 10}
        rng.exclude(0, 6);          //now rng encompasses only: {9, 10}
    }
  @endcode

  RangeSet can also be based on custom (STL-compatible) sequence containers, for example:
  @code
    void func(){
        uau::RangeSet<int, QList> rng;
        rng.assign_range(0, 10);    //consists one range: {0, 10} inclusive
        ....
        auto v = rng.toVector();    //QList's interface using
    }
  @endcode
*/
template <class T, template<class ...> class Sequence = std::list>
class RangeSet : public Sequence< std::pair<T, T> >{
public:
    /**
      @brief    initial range
      @param    min lower buond (inclusive)
      @param    max upper bound (inclusive)
    */
    void assign_range(T min, T max);

    /**
      @param    min lower bound (inclusive)
      @param    max upper bound (inclusive)
    */
    void exclude(T min, T max);
};

template <class T, template<class ...> class Sequence>
void RangeSet<T, Sequence>::assign_range(T min, T max){
    this->clear();
    this->push_back(std::make_pair(min, max));
}

template <class T, template<class ...> class Sequence>
void RangeSet<T, Sequence>::exclude(T min, T max){
    auto includes = [](typename RangeSet::const_reference range,
                       typename RangeSet::const_reference val){
        return range.second < val.first;
    };

    auto ranges = std::equal_range(this->begin(), this->end(), std::make_pair(min, max), includes);

    if(ranges.first != ranges.second){
        auto lower = ranges.first;          //inclusive lower bound of range
        auto upper = --ranges.second;       //inclusive upper bound of range

        if(lower == upper){                  //splitting range
            lower = this->insert(lower, *lower);
            upper = lower;
            ++upper;
        }

        //erasing
        if(lower->first < min){
            lower->second = --min;
            ++lower;
        }
        if(max < upper->second){
            upper->first = ++max;
            --upper;
        }

        this->erase(lower, ++upper);
    }
}

} //namespace uau

#endif // RANGESET_H
