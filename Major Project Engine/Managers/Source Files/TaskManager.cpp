#include "TaskManager.h"

TaskManager::~TaskManager() {}

void TaskManager::Init()
{
	job_list = std::list<Job*>();
	waiting_jobs = std::list<Job*>();
}

void TaskManager::Close()
{
	auto job_it = job_list.begin();
	while (job_it != job_list.end())
	{
		if ((*job_it) != nullptr)
		{
			delete((*job_it));
			job_it = job_list.erase(job_it);
		}
	}

	job_it = waiting_jobs.begin();
	while (job_it != waiting_jobs.end())
	{
		if ((*job_it) != nullptr)
		{
			delete((*job_it));
			job_it = waiting_jobs.erase(job_it);
		}
	}
}

bool TaskManager::FrameStart()
{
	if (jobs_to_finish == 0 && num_of_jobs == 0)
		return true;
	return false;
}

bool TaskManager::HasJobs()
{
	return (!waiting_jobs.empty() || !job_list.empty());
}

void TaskManager::NotifyDone()
{
	jobs_to_finish--;
}

void TaskManager::RegisterJob(JobFunction function, const std::string name, void* content, const Job::JOB_TYPE type)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	job_list.emplace_back(new Job(function, name, content, type));
	num_of_jobs++;
}

void TaskManager::RegisterJob(Job * job, bool wait)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	if (wait)
	{
		waiting_jobs.emplace_back(job);
	}
	else
	{
		job_list.emplace_back(job);
		num_of_jobs++;
	}
}

void TaskManager::RegisterJob(Job * job, Job * parent_job)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	parent_job->IncrementWait();
	job->SetParent(parent_job);
	job_list.emplace_back(job);
	num_of_jobs++;
}

void TaskManager::RetryJob(Job * job)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	job_list.emplace_back(job);
}

void TaskManager::TransferJobs(ThreadManager * & threadpool)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	std::list<Job*>::iterator job_it = waiting_jobs.begin();
	while (job_it != waiting_jobs.end())
	{
		if ((*job_it)->GetWaiting() == 0)
		{
			job_list.emplace_back((*job_it));
			job_it = waiting_jobs.erase(job_it);
			num_of_jobs++;
			continue;
		}
		job_it++;
	}
	threadpool->GetJobs(&job_list);
	job_list.clear();
	jobs_to_finish += num_of_jobs;
	num_of_jobs = 0;
}
