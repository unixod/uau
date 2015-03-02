#include "uau/threadpool.h"
#include "threadpool_p.h"

uau::BlockingThreadPool::BlockingThreadPool(int i) :
    BlockingThreadPool(std::unique_ptr<BlockingThreadPoolPrivate>(new BlockingThreadPoolPrivate)) {

    d_ptr->inProgress = 0;
    d_ptr->exit = false;

    while(i--)
        d_ptr->ths.emplace_back(std::thread(&BlockingThreadPool::threadMain, this));
}


uau::BlockingThreadPool::BlockingThreadPool(std::unique_ptr<uau::BlockingThreadPoolPrivate> d) :
    d_ptr(std::move(d)) {}


uau::BlockingThreadPool::~BlockingThreadPool() {
    std::unique_lock<std::mutex> lck(d_ptr->mtx);

    d_ptr->exit = true;
    d_ptr->full.notify_all();
    lck.unlock();

    for(auto &t : d_ptr->ths)
        t.join();
}

void uau::BlockingThreadPool::wait() {
    std::unique_lock<std::mutex> lck(d_ptr->mtx);
    d_ptr->empty.wait(lck, [this]{
        return d_ptr->ths.empty() || (d_ptr->tasks.empty() && !d_ptr->inProgress);
    });
}

void uau::BlockingThreadPool::threadMain() {
    for(;;) {
        std::unique_lock<std::mutex> lck(d_ptr->mtx);

        d_ptr->empty.notify_all();

        d_ptr->full.wait(lck, [this]{return d_ptr->exit || !d_ptr->tasks.empty();});
        if(exit) break;

        Task task = d_ptr->tasks.front();
        d_ptr->tasks.pop();

        lck.unlock();

        ++d_ptr->inProgress;

        try{
            task();
        } catch (...) {}

        --d_ptr->inProgress;
    }
}

void uau::BlockingThreadPool::addTask(uau::BlockingThreadPool::Task &&task) {
    if(!d_ptr->ths.empty()) {
        std::unique_lock<std::mutex> lck(d_ptr->mtx);
        d_ptr->tasks.push(task);
        d_ptr->full.notify_all();  //in theory full.notify_one(); is better, but in practic the opposit, why???
    } else {
        task();
    }
}
