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

#ifndef LIBUAU_THREADPOOL_H
#define LIBUAU_THREADPOOL_H


#include <memory>


namespace uau {


class BlockingThreadPoolPrivate;


/**
  @brief Thread pool (blocking version)

  Usage:
  @code
    void someTask1() {
        ...
    }

    void someTask2(int rank,  std::vector<int> &out) {
        ...
    }

    class SomeTask3 {
    public:
        void doWork(...) {
            ...
        }
    };

    void func() {
        uau::BlockingThreadPool pool(5); //reserving 5 worker threads

        for(...) {
            pool(someTask1);
        }
        pool.wait();


        typedef std::vector<int> Result;
        int cnt = ...
        std::vector<Result> res(cnt);
        for(int i = 0; i < cnt; i++) {
            pool(someTask2, i, std::ref(res[i]));
        }
        pool.wait();

        SomeTask3 task3;
        pool(&SomeTask3::doWork, task3, ...);
        pool.wait();
    }
  @endcode
*/
class BlockingThreadPool {
    typedef std::function<void()> Task;

public:
    BlockingThreadPool(int i);

    ~BlockingThreadPool();

    template<typename Callable, typename... Args>
    void operator()(Callable &&f, Args&&... args) {
        auto task = std::bind(std::forward<Callable>(f), std::forward<Args>(args)...);
        addTask(std::move(task));
    }

    void wait();

private:
    void threadMain();
    inline void addTask(Task &&);

protected:
    friend class BlockingThreadPoolPrivate;
    BlockingThreadPool(std::unique_ptr<BlockingThreadPoolPrivate>);
    std::unique_ptr<BlockingThreadPoolPrivate> d_ptr;
};


} // namespace uau


#endif // LIBUAU_THREADPOOL_H


