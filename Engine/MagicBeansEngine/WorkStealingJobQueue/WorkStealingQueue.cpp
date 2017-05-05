#include "WorkStealingQueue.h"

#include <iostream>



WorkStealingQueue::WorkStealingQueue(size_t threadCount) : stopping_(false), workers_ready_(false), dummyWorker((size_t)-1, this)
{
  workers_.reserve(threadCount);
	for (worker_count = 0; worker_count < threadCount; ++worker_count)
	{
		workers_.emplace_back(worker_count, this);
	}

  //Strip the dummy worker of its functionality
  dummyWorker.stopWork_ = true;
  dummyWorker.thread.join();
}

WorkStealingQueue::WorkStealingQueue() : stopping_(false), workers_ready_(false), dummyWorker((size_t)-1, this)
{
	/* 
		TODO use my system report stuff to get this number 

		Use one less than the number of threads on this machine since the main thread
		will also be performing work (mostly stolen)
	*/
	size_t threadCount = std::thread::hardware_concurrency() - 1;
  workers_.reserve(threadCount);
	for (worker_count = 0; worker_count < threadCount; ++worker_count)
	{
		workers_.emplace_back(worker_count, this);
	}

	//Strip the dummy worker of its functionality
	dummyWorker.stopWork_ = true;
	dummyWorker.thread.join();

}

WorkStealingQueue::~WorkStealingQueue()
{
	workers_ready_ = false;

	//Stop and join all worker threads
	for (; worker_count > 0; --worker_count)
	{
		workers_[worker_count - 1].stopWork_ = true;
		workers_[worker_count - 1].thread.join();
	}
  
	workers_.clear();
}

void WorkStealingQueue::Run(Job * job)
{
	//If there are no workers (operating in single-thread mode)
	if (workers_.size() == 0)
	{
		dummyWorker.jobs.Push(job);

		//We now know at least 1 job exists and so stealing is now ok
		workers_ready_ = true;

		return;
	}

	//Find the shortest queue and add the job to that
	auto shortest = workers_.begin();

	for (auto it = workers_.begin(); it != workers_.end(); ++it)
	{
		size_t shortLen = shortest->jobs.Length();
		
		//If this length is 0 we're not going to beat that, just use it
		if (shortLen == 0)
		{
			break;
		}

		if (it->jobs.Length() < shortLen)
		{
			shortest = it;
		}
	}

	shortest->jobs.Push(job);
	
	//We now know at least 1 job exists and so stealing is now ok
	workers_ready_ = true;
}

void WorkStealingQueue::Wait(const Job * job)
{
	while (!HasJobCompleted(job))
	{
		//Since what we wanted isn't ready, do some work
		//Who knows, it may speed things up
		Job* nextJob = dummyWorker.GetJob();
		
		if (nextJob)
		{
			Execute(nextJob);
		}
	}
}

Job * WorkStealingQueue::AquireJob()
{
	if (workers_ready_ && worker_count >= 1)
	{
		//Pick a random worker and steal a job from them
		size_t choice = rand() % worker_count;
		//Returns null if nothing to steal from this one
		return workers_[choice].jobs.Steal();
	}
	else
	{
		//Will return null if nothing to steal
		return dummyWorker.jobs.Steal();
	}
}

void WorkStealingQueue::Execute(Job * job)
{
	dummyWorker.RunJob(job);
}

