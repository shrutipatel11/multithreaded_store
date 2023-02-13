#pragma once

#include<thread>
#include<vector>
#include<iostream>
#include<list>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<atomic>
#include<functional>

using std::cout;
using std::endl;
using std::move;
using std::thread;
using std::function;
using std::lock_guard;
using std::mutex;
using std::unique_lock;
using std::vector;
using std::list;
using std::atomic_int;
using std::atomic_bool;
using std::condition_variable;

class threadpool {

public:
    std::vector<std::thread> workers;
    std::list<function<void()>> tasks;

    //For synchronization
    std::mutex queue_lock;
    std::condition_variable freenow;

    void setMaxThreads(int threads){
        for(int i=0; i<threads; i++){
            workers.push_back(thread([this,i]{this->get_task();}));
        }
    }

    void get_task() {
        function<void()> task;
        while(1){
            std::unique_lock<std::mutex> lock(queue_lock);
            this->freenow.wait(lock, [this]()->bool{return (this->tasks.size());});
            task = this->tasks.front();
            this->tasks.pop_front();
            task();
        }
    }

    ~threadpool() {
        joinAll();
    }

    void addTasks(function<void()> task) {
        std::unique_lock<std::mutex> lock(queue_lock);
        tasks.push_back(task);
        freenow.notify_one();
    }

    void joinAll() {
        for (auto &thread : workers){
            if (thread.joinable()){
                thread.join();
            }
        }
    }
};
