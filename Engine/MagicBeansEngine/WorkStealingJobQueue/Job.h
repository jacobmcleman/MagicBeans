#pragma once

#include <atomic>
#include <exception>

struct Job;
class Worker;

typedef void(*JobFunction)(Job*, void*);

//Should be a power of 2 so that optimizations are good
static constexpr size_t Max_Jobs = (1 << 12);

struct Job
{
	JobFunction function;
	Job* parent;
	std::atomic<long> unfinished;

	//Add bytes at the end of the job struct to ensure it fully occupies cache lines
	//This is to prevent false sharing
	static constexpr size_t padbytes = 64 - (sizeof(JobFunction) + sizeof(Worker*) + sizeof(Job*) + sizeof(std::atomic<long>));

	//Since there is a bunch of bytes here it is possible to put all of the job's data in here
	//When the job is called this will be handed to the function
	char padding[padbytes];

	void RunChild(Job* job);

	std::atomic<Worker*> worker_;
};


/*
	Create a new job with optional parent.

	Parent jobs will be marked as completed only when
	all of their children are also completed
*/
Job* CreateJob(JobFunction function, Job* job = nullptr);

/*
	Allocate a new job, will use new and delete for now
	but should use a custom memory manager soon
*/
Job* AllocateJob();

/*
	Free a job (duh)
*/
void FreeJob(Job* job);

/*
	Query a job to see if it has finished running
*/
bool HasJobCompleted(const Job* job);

/*
	Called when a job is finished, and called on parent
	when a child's job is finished
*/
void Finish(Job* job);

/*
	Copy the data from T into the job's field so that it can make use of it
*/
template<typename T>
void SetJobData(Job* job, const T& jobData)
{
#ifdef _DEBUG
__pragma(warning(push))
__pragma(warning(disable:4127)) //This conditional expression should be const
  if (sizeof(T) > job->padbytes)
  {
    throw std::exception("Type is too large for job");
  }
__pragma(warning(pop))
#endif
	std::memcpy(job->padding, &jobData, sizeof(T));
}

/*
Create a job and copy given data into it
(optional) parent
*/
template<typename T>
Job* CreateJob(JobFunction function, const T& jobData, Job* parent = nullptr)
{
	Job* job = CreateJob(function, parent);
	SetJobData(job, jobData);
	return job;
}
