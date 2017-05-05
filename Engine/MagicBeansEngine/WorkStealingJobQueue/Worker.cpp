#include <iostream>

#include "Worker.h"
#include "WorkStealingQueue.h"

#include "Utilities.h"

#include "MT_Print.h"

//#define SPAM_ME

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
  SCOPED_LOCK(jobs_mutex);
	jobs_[bottom_ & cs_mask_] = job;
	bottom_++;
}

Job * Worker::WorkQueue::Pop()
{
  SCOPED_LOCK(jobs_mutex);
	
  const long jobCount = bottom_ - top_;

  if (jobCount <= 0)
  {
      return nullptr;
  }
  else
  {
      --bottom_;
      return jobs_[bottom_];
  }
}

Job * Worker::WorkQueue::Steal()
{
  SCOPED_LOCK(jobs_mutex);

  const long jobCount = bottom_ - top_;

  if (jobCount <= 0)
  {
    return nullptr;
  }
  else
  {
    Job* job = jobs_[top_];
    ++top_;
    return job;
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
