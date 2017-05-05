#pragma once
#include <deque>
#include <vector>
#include <thread>
#include <mutex>
#include "Worker.h"
#include "Job.h"



class WorkStealingQueue
{
public:
	WorkStealingQueue(size_t threadCount);
	WorkStealingQueue();
	~WorkStealingQueue();

	void Run(Job* job);
	void Wait(const Job* job);

	//Steal a job from some worker
	Job* AquireJob();

	//This is a worker whose functionality is disabled
	//This is what the main thread uses to do work
	Worker dummyWorker;

	friend class Worker;

private:
	size_t worker_count;
	bool workers_ready_;

	std::vector<Worker> workers_;

	bool stopping_;

	void Execute(Job* job);
};
