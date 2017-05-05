#pragma once
#include <thread>
#include <mutex>
#include <atomic>

#include "Job.h"

class WorkStealingQueue;

class Worker
{
private:
    class WorkQueue
    {
    public:
        WorkQueue();

        /* Only called by thread that owns this queue */
        void Push(Job* job);
        Job* Pop();

        /* Only called by non-queue owners */
        Job* Steal();

        /* Called to find the shortest / longest queue */
        size_t Length() const;

    private:
        friend class Worker;
        //                                     2048
        static constexpr size_t cs_max_size_ = 1 << 11;
        static constexpr size_t cs_mask_ = cs_max_size_ - 1;

        Job* jobs_[cs_max_size_];

        std::atomic<long> bottom_;
        std::atomic<long> top_;

        std::mutex jobs_mutex;
    };

public:
    Worker(size_t workerID, WorkStealingQueue* owner);
    Worker(Worker&& from);
    ~Worker();

    Worker(const Worker& other) = delete;
    WorkStealingQueue* GetOwner();


    WorkQueue jobs;

    friend class WorkStealingQueue;

private:
    std::thread thread;
    size_t id;
    WorkStealingQueue* owner;
    bool stopWork_;

    void Loop();
    Job* GetJob();
    void RunJob(Job* job);
};