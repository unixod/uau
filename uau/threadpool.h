#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <atomic>
#include <thread>
#include <vector>
#include <queue>

namespace uau{

class BlockingThreadPool{
    std::atomic<int> inProgress;
    std::mutex mtx;
    std::condition_variable  full;
    std::condition_variable  empty;

    typedef std::function<void()> Task;
    std::queue<Task> tasks;
    std::vector<std::thread> ths;

    volatile bool exit;
public:
    BlockingThreadPool(int i) : inProgress(0), exit(false){
        while(i--)
            ths.emplace_back(std::thread(std::mem_fun(&BlockingThreadPool::threadMain), this));
    }
    ~BlockingThreadPool(){
        {
            std::unique_lock<std::mutex> lck(mtx);
            exit = true;
        }
        full.notify_all();
        for(auto &t : ths)
            t.join();
    }

    template<typename Callable, typename... Args>
    void operator()(Callable &&f, Args&&... args){
        if(!ths.empty()){
            auto task = std::bind(std::forward<Callable>(f), std::forward<Args>(args)...);
            std::unique_lock<std::mutex> lck(mtx);
            tasks.push(task);
            full.notify_all();  //in theory full.notify_one(); is better, but in practic the opposit, why???
        } else {
            f(std::forward<Args>(args)...);
        }
    }

    void wait(){
        std::unique_lock<std::mutex> lck(mtx);
        empty.wait(lck, [&](){return ths.empty() || (tasks.empty() && !inProgress);});
    }

private:
    void threadMain(){
        while(1){
            Task task;
            {
                std::unique_lock<std::mutex> lck(mtx);
                full.wait(lck, [&tasks, &exit](){return exit || !tasks.empty();});
                if(exit) break;

                task = tasks.front();
                tasks.pop();
            }
            ++inProgress;
            task();
            --inProgress;
            empty.notify_one();
        }
    }
};


} //namespace uau

#endif // THREADPOOL_H


