#include "Job.h"
#include "WorkStealingQueue.h"
#include <atomic>

#include "MT_Print.h"

//static ObjectAllocator s_JobAllocator(sizeof(Job), OAConfig(true, 16, 0, false, 0));

static thread_local Job g_JobAllocator[4096];
static std::atomic<size_t> g_JobIndex;
static std::atomic<size_t> g_AllocatedJobs;

//#define SPAM_ME


Job * CreateJob(JobFunction function, Job* parent)
{
	if (parent != nullptr)
	{
		++(parent->unfinished);
	}

	//Get a new job from whatever is being used to manage memory
	Job* job = AllocateJob();
	job->function = function;
	job->parent =parent;
	job->unfinished = 1;
	job->worker_ = nullptr;

	return job;
}

Job * AllocateJob()
{
	//return new Job();
  const size_t index = ++g_JobIndex;

#ifdef _DEBUG
  //In debug mode keep count of allocated jobs to avoid going over the limit
  size_t allocated = ++g_AllocatedJobs;

  if (allocated > Max_Jobs)
  {
    throw std::exception("Too many jobs! Slow down!");
  }
#endif // _DEBUG

  return &g_JobAllocator[(index - 1) & (Max_Jobs - 1)];
}

void FreeJob(Job* job)
{
	//delete job;
  (void)job;
  //Since jobs are reused I don't think anything needs to be here?

  

#ifdef _DEBUG
  //In debug mode keep count of allocated jobs to avoid going over the limit
  --g_AllocatedJobs;

  //Also check that jobs were actually safe to discard
  if (job->unfinished != -1)
  {
	  throw std::exception("Job was freed when it was not in finished state!");
  }

  //Set the memory to zeroes to reduce potential fuckups
  //(also makes memory debugging easier)
  std::memset(job, 0, sizeof(Job));
  //BUT make sure the unfinished jobs is still -1 so things can
  //know that this job is finished
  job->unfinished = -1;

#endif // _DEBUG

}

bool HasJobCompleted(const Job * job)
{
	return job->unfinished < 0;
}

void Finish(Job * job)
{
	//One of this job's job's is finished
	long unfinished = --(job->unfinished);

#ifdef SPAM_ME
	PrintMessage(std::string("Finished a job, ") + std::to_string(unfinished) + " children of it left");
#endif

	//If the jobs are now all done
	if (unfinished == 0)
	{
#ifdef SPAM_ME
      PrintMessage("That was the last part of this job, prodding parent if it exists");
#endif
		if (job->parent)
		{
			Finish(job->parent);
		}

		/*
			The unfinished counter being -1 indicates that this final
			process of notification and cleanup has finished and that
			it is now safe to free this job
		*/
		--(job->unfinished);

#ifdef SPAM_ME
		PrintMessage("Freeing a job");
#endif

		//This job is hella-done, so free it (no-op in release mode)
		FreeJob(job);
	}
}

void Job::RunChild(Job * job)
{
	if (worker_.load() != nullptr)
	{
		worker_.load()->jobs.Push(job);
		//worker_.load()->GetOwner()->Run(job);

	}
	else
	{
		throw std::exception("Job is not currently being run, cannot run as child of it");
	}
}
