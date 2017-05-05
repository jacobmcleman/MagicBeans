#include <iostream>

#include "Worker_LockFree.h"
#include "WorkStealingQueue.h"

#include "Utilities.h"

#include "MT_Print.h"

//#define SPAM_ME
#ifdef LOCK_FREE

Worker::WorkQueue::WorkQueue()
{
#ifdef _DEBUG
	std::memset(jobs_, 0, sizeof(Job*) * cs_max_size_);
#endif

	bottom_ = 0;
	top_ = 0;
}

void Worker::WorkQueue::Push(Job * job)
{
	//Do not need to lock since only this thread
	//Is ever allowed to touch the front
	long b = bottom_;
	jobs_[b & cs_mask_] = job;

	COMPILER_BARRIER;

	bottom_ = b + 1;
}

Job * Worker::WorkQueue::Pop()
{
	long b = bottom_ - 1;
	bottom_.exchange(b);

  MEMORY_BARRIER;

	long t = top_;

	if (t <= b)
	{
		//Queue is non-empty
		Job* job = jobs_[b & (cs_mask_)];

		if (t != b)
		{
#ifdef _DEBUG
			if (job == nullptr || HasJobCompleted(job))
			{
				throw std::exception("Tried to return a job from the bottom of the queue that was already completed");
			}
#endif
			//If there it still more than one item left then this job
			//can't be stolen so we can return this one
			return job;
		}

		//Since there is only one job left, make sure no one else took
		//it while we were checking that other stuff
		if (!top_.compare_exchange_strong(t, t + 1))
		{
			//Someone else took it, no job for you today
			job = nullptr;
		}

		//Move the bottom to be above the top since the last job was finished
		bottom_ = t + 1;

#ifdef _DEBUG
		if (job == nullptr || HasJobCompleted(job))
		{
			throw std::exception("Tried to return the last job in the queue and it was already completed");
		}
#endif

		return job;
	}
	else
	{
		//No jobs here
		bottom_ = t;
		return nullptr;
	}
}

Job * Worker::WorkQueue::Steal()
{
	//Remember the values of the top/bottom when we started
	long t = top_;

	COMPILER_BARRIER;

	long b = bottom_;

	//Check that the queue is non-empty
	if (t < b)
	{
		//Get the top job from the queue
		Job* job = jobs_[t & cs_mask_];

		//Check to see if someone else stole the job
		//that we were about to steal
		if (top_.compare_exchange_strong(t, t + 1))
		{
			//The job wasn't stolen from right underneath
			//this worker's thieving nose, so we have stolen it
#ifdef _DEBUG
			if (job == nullptr || HasJobCompleted(job))
			{
				throw std::exception("Tried to steal a job from the top of the queue that was already completed");
			}
#endif
			return job;
		}
		else
		{
			//Some other worker took the job
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}

size_t Worker::WorkQueue::Length() const
{
	return bottom_ - top_;
}

Worker::Worker(size_t workerID, WorkStealingQueue * Owner) :
	id(workerID), owner(Owner), stopWork_(false)
{
	//Start working on the jobs from this thread
	thread = std::thread([&](Worker* worker) { worker->Loop(); }, this);
}

Worker::Worker(Worker && from) :
	id(from.id), owner(from.owner),
	stopWork_(from.stopWork_)
{
	//Stop the worker we're moving from from working
	from.stopWork_ = true;
	//Wait for the worker to stop then kill its thread
	while (!from.thread.joinable());
	from.thread.join();

	//Transfer the worker's job (de)queue
	std::swap(jobs.jobs_, from.jobs.jobs_);
	jobs.bottom_.store(from.jobs.bottom_.load());
	jobs.top_.store(from.jobs.top_.load());

	//Start up the new worker's thread working on this thread's queue
	thread = std::thread([&](Worker* worker) { worker->Loop(); }, this);
}

Worker::~Worker()
{
	stopWork_ = true;
	if (thread.joinable()) thread.join();
}

WorkStealingQueue * Worker::GetOwner()
{
	return owner;
}

void Worker::RunJob(Job * job)
{

#ifdef _DEBUG
	if (HasJobCompleted(job))
	{
		throw std::exception("Job has already been completed but is trying to be run again");
	}
#endif

#ifdef _DEBUG
	try
	{
#endif
		//If the job does not have a worker already, make its worker be this
		//Use atomic compare exchange so that this operation is atomic/safe
		Worker* expected_value = nullptr;
		if (job->worker_.compare_exchange_strong(expected_value, this))
		{
#ifdef SPAM_ME
        PrintMessage(std::string("Worker #") + std::to_string(id) + std::string(" Starting to work on a job"));
#endif
			job->function(job, job->padding);
			Finish(job);
			job->worker_ = nullptr;
#ifdef SPAM_ME
      PrintMessage(std::string("Worker #") + std::to_string(id) + " Finished a job");
#endif
		}
#ifdef _DEBUG
	}
	catch (const std::exception& e)
	{
		std::cout << "WORKER ERROR: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "UNIDENTIFIED WORKER ERROR" << std::endl;
	}
#endif
}

void Worker::Loop()
{
	//std::cout << "Worker Thread " << this->id << " Started" << std::endl;
	while (!stopWork_)
	{
		Job* job = GetJob();
		if (job != nullptr)
		{
			RunJob(job);
		}
		else
		{
			std::this_thread::yield();
		}
	}
}

Job * Worker::GetJob()
{
  //If this worker has jobs to do
	if (jobs.Length() > 0)
	{
    //Return the next job in this workers queue
		return jobs.Pop();
	}
  //Otherwise, attempt a steal
	else
	{
    //Ask this worker's "manager" for more work 
		if (owner != nullptr)
			return owner->AquireJob();
		else return nullptr;
	}
}
#endif