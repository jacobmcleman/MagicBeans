#pragma once
#include "Job.h"

namespace
{
	template<typename DataType>
	struct parallel_for_data
	{
		typedef void(*ParForFunc)(DataType*, size_t);

		//Data to operate on
		DataType* data;
		//Number of elements in the data set
		size_t count;
		//Function to use on the data
		ParForFunc function;
		//Maximum amount of elements a single job should process
		size_t blockSize;

		parallel_for_data(DataType* Data, size_t Count, ParForFunc Function, size_t SplitLine) :
			data(Data), count(Count), function(Function), blockSize(SplitLine)
		{

		}
	};

	template <typename JobData>
	void parallel_for_job(Job* job, void* data)
	{
		JobData& jobData = *reinterpret_cast<JobData*>(data);

		if (jobData.count > jobData.blockSize)
		{
			size_t leftSize = jobData.count / 2;
			JobData leftData(jobData.data, leftSize, jobData.function, jobData.blockSize);
			Job* left = CreateJob(parallel_for_job<JobData>, leftData, job);
			job->RunChild(left);

			size_t rightSize = jobData.count - leftSize;
			JobData rightData(jobData.data + leftSize, rightSize, jobData.function, jobData.blockSize);
			Job* right = CreateJob(parallel_for_job<JobData>, rightData, job);
			job->RunChild(right);
		}
		else
		{
			jobData.function(jobData.data, jobData.count);
		}
	}
}

template<typename DataType>
Job* parallel_for(DataType* data, size_t size, void(*function)(DataType*, size_t), size_t splitAt = 256)
{
	typedef parallel_for_data<DataType> JobData;

	JobData jobData(data, size, function, splitAt);

	Job* job = CreateJob(parallel_for_job<JobData>, jobData);

	return job;
}