#ifndef CONCURRENCY_ALGO_H
#define CONCURRENCY_ALGO_H

#include <algorithm>
#include <vector>
#include <list>
#include "threadpool.h"


namespace uau{
typedef size_t ChunkSize;
typedef size_t ChunkCoord;
typedef std::list< std::pair<ChunkCoord, ChunkCoord> > ChunkSet;

namespace _intersection{
typedef std::vector<ChunkCoord> ChunkMap;
typedef std::vector<ChunkMap> map;

template<class InputIterator1, class InputIterator2>
void chunkMap(ChunkMap &ret, InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2){

    //std::equal analog, but with second sequence end checking
    auto equal = [](InputIterator1 _first1, InputIterator1 _last1, InputIterator2 _first2, InputIterator2 _last2) -> bool {
        for(; _first1 != _last1 && _first2 != _last2 && *_first1 == *_first2; ++_first1, ++_first2);
        return _first1 == _last1;
    };

    for(size_t pos = 0; first2 != last2; ++first2, ++pos){
        if(equal(first1, last1, first2, last2))
            ret.push_back(pos);
    }
}

size_t chunkLength(ChunkCoord itm, size_t len, map::const_iterator first, map::const_iterator last){
    size_t fnd = itm;
    if((first != last) &&
        ((fnd = [&](){
            auto begin = first->begin();
            auto end = first->end();
            auto iter = std::lower_bound(begin, end, fnd+1);
            return (iter != end) && (*iter == fnd+1) ? *iter : -1;
        }()) != -1)){
        return chunkLength(fnd, len, ++first, last) + 1;
    }
    return len;
}

void maxChunks(std::pair<ChunkSize, ChunkSet> &chunksGrp, int id, int k, size_t m, map::const_iterator first, map::const_iterator last){
    ChunkSize &maxLen = chunksGrp.first;
    ChunkSet &chunks = chunksGrp.second;

    size_t pos = 0;
    for(auto iter = first; iter != last; ++iter){
        auto begin = iter;
        auto end = last;
        ++begin;

        for(size_t i = id; i < iter->size(); i += k){
            auto &itm = (*iter)[i];
            auto len = chunkLength(itm, m, begin, end);
            if(len  >= maxLen){
                if(len > maxLen){
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

/*
  k - threads count, to parallel proccesing
  m - minimum len
*/
template<class InputIterator1, class InputIterator2>
std::pair<ChunkSize, ChunkSet> maxIntersections(int k, size_t m, InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2){
    BlockingThreadPool pool(k == 1 ? 0 : k);    //k == 1, then using main thread, not pool threads

    //making intersections map (set of chunks maps)
    auto seqSize1 = std::distance(first1, last1);
    if(!m || seqSize1 < m)
        return {};

    int chunksCnt = seqSize1 - m + 1;

    _intersection::map seqMap(chunksCnt);

    InputIterator1 chunk_end = first1;
    std::advance(chunk_end, m);

    for(int i = 0; i < chunksCnt; ++i){
        pool(_intersection::chunkMap<InputIterator1, InputIterator2>, std::ref(seqMap[i]), first1++, chunk_end++, first2, last2);
    }
    pool.wait();

    //finding largest chunks in intersections map
    std::vector< std::pair<ChunkSize, ChunkSet> > chunksGroups(k);
    for(int i = 0; i < k; ++i){
        pool(_intersection::maxChunks, std::ref(chunksGroups[i]), i, k, m, seqMap.begin(), seqMap.end());
    }
    pool.wait();

    //finding maximum groups
    ChunkSize maxChunkLen = 0;
    ChunkSet chunks;
    for(auto &group : chunksGroups){
        auto len = group.first;
        if(len  > maxChunkLen){
            maxChunkLen = len;
            chunks = std::move(group.second);
        } else if(len == maxChunkLen){
            chunks.splice(chunks.end(), group.second);
        }
    }

    return {maxChunkLen, chunks};
}

} // namespace uau

#endif // CONCURRENCY_ALGO_H
