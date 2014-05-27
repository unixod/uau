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

/** @file concurrency_algo.h
    Concurrency algorithms header
*/

#ifndef UAU_CONCURRENCY_ALGO_H
#define UAU_CONCURRENCY_ALGO_H

#include <algorithm>
#include <vector>
#include <list>
#include "threadpool.h"

namespace uau {

typedef size_t ChunkSize;
typedef size_t ChunkCoord;
typedef std::list< std::pair<ChunkCoord, ChunkCoord> > ChunkSet;

namespace _intersection {
typedef std::vector<ChunkCoord> ChunkMap;
typedef std::vector<ChunkMap> map;

template<class InputIterator1, class InputIterator2>
void chunkMap(ChunkMap &ret, InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {

    //std::equal analog, but with second sequence end checking
    auto equal = [](InputIterator1 _first1, InputIterator1 _last1, InputIterator2 _first2, InputIterator2 _last2) -> bool {
        for(; _first1 != _last1 && _first2 != _last2 && *_first1 == *_first2; ++_first1, ++_first2);
        return _first1 == _last1;
    };

    for(size_t pos = 0; first2 != last2; ++first2, ++pos) {
        if(equal(first1, last1, first2, last2))
            ret.push_back(pos);
    }
}

size_t chunkLength(ChunkCoord itm, size_t len, map::const_iterator first, map::const_iterator last) {
    size_t fnd = itm;
    if((first != last) &&
        ((fnd = [&]{
            auto begin = first->begin();
            auto end = first->end();
            auto iter = std::upper_bound(begin, end, fnd);
            return (iter != end) && (*iter == fnd+1) ? *iter : -1;
        }()) != -1)) {
        return chunkLength(fnd, len, ++first, last) + 1;
    }
    return len;
}

void maxChunks(std::pair<ChunkSize, ChunkSet> &chunksGrp, int id, int k, size_t m, map::const_iterator first, map::const_iterator last) {
    ChunkSize &maxLen = chunksGrp.first;
    ChunkSet &chunks = chunksGrp.second;

    size_t pos = 0;
    for(auto iter = first; iter != last; ++iter) {
        auto begin = iter;
        auto end = last;
        ++begin;

        for(size_t i = id; i < iter->size(); i += k) {
            auto &itm = (*iter)[i];
            auto len = chunkLength(itm, m, begin, end);
            if(len  >= maxLen) {
                if(len > maxLen) {
                    maxLen = len;
                    chunks.clear();
                }
                chunks.emplace_back(pos, itm);
            }
        }
        ++pos;
    }
}

} // namespace _intersection

/**
  @brief    Constructs the set of maximum intersections of the two sorted ranges
  @param    k   threads count, to parallel proccesing
  @param    m   minimum len
  @param    first1  An input iterator.
  @param    last1  An input iterator.
  @param    first2  An input iterator.
  @param    last2  An input iterator.
  @return   returns chucks group. Chuncks group consists of two parts:
            - chunk size (size of every chunk in group)
            - sequence of chunks (coords in first and second input sequences)

  For more details of this algorithm see: http://habrahabr.ru/post/145198/
*/
template<class InputIterator1, class InputIterator2>
std::pair<ChunkSize, ChunkSet> maxIntersections(int k, size_t m, InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
    BlockingThreadPool pool(k == 1 ? 0 : k);    //k == 1, then using main thread, not pool threads

    //making intersections map (set of chunks maps)
    auto seqSize1 = std::distance(first1, last1);
    if(!m || seqSize1 < m)
        return {};

    int chunksCnt = seqSize1 - m + 1;

    _intersection::map seqMap(chunksCnt);

    InputIterator1 chunk_end = first1;
    std::advance(chunk_end, m);

    for(int i = 0; i < chunksCnt; ++i) {
        pool(_intersection::chunkMap<InputIterator1, InputIterator2>, std::ref(seqMap[i]), first1++, chunk_end++, first2, last2);
    }
    pool.wait();

    //finding largest chunks in intersections map
    std::vector< std::pair<ChunkSize, ChunkSet> > chunksGroups(k);
    for(int i = 0; i < k; ++i) {
        pool(_intersection::maxChunks, std::ref(chunksGroups[i]), i, k, m, seqMap.begin(), seqMap.end());
    }
    pool.wait();

    //finding maximum groups
    ChunkSize maxChunkLen = 0;
    ChunkSet chunks;
    for(auto &group : chunksGroups) {
        auto len = group.first;
        if(len  > maxChunkLen) {
            maxChunkLen = len;
            chunks = std::move(group.second);
        } else if(len == maxChunkLen) {
            chunks.splice(chunks.end(), group.second);
        }
    }

    return {maxChunkLen, chunks};
}

} // namespace uau

#endif // UAU_CONCURRENCY_ALGO_H
