#include "TaskManager.h"

TaskManager::TaskManager(const std::size_t & thread_size) {}

TaskManager::~TaskManager() {}

void TaskManager::SetTimeLock(const float & time_lock)
{
	//_scheduler->SetTimeLock(time_lock);
}

void TaskManager::Close()
{
	auto job_it = waiting_jobs.begin();
	while (job_it != waiting_jobs.end())
	{
		if ((*job_it) != nullptr)
		{
			delete((*job_it));
			job_it = waiting_jobs.erase(job_it);
		}
	}
	//if (_scheduler != nullptr) delete _scheduler;
}

bool TaskManager::HasJobs()
{
	std::lock_guard<std::mutex> lk(list_lock);
	return (!waiting_jobs.empty() || !task_queue.Empty());
}

void TaskManager::RegisterJob(JobFunction function, const std::string name, void* content, const Job::JOB_ID type)
{
	Job * job = new Job(function, name, content, type);
	//_scheduler->CheckForJob(job);
	task_queue.Emplace(job);
	{
		std::lock_guard<std::mutex> lock(jobs_lock);
		num_of_jobs++;
	}
	
}

void TaskManager::RegisterJob(Job * & job, bool wait, Job * parent_job)
{
	//_scheduler->CheckForJob(job);
	if (parent_job != nullptr)
		job->AddParent(parent_job);

	if (dictionary.find(job->j_type) != dictionary.end())
	{
		for (auto jobs : dictionary.find(job->j_type)->second)
		{
			for (auto waits : waiting_jobs)
			{
				if (waits->j_type == jobs)
				{
					job->AddParent(waits);
				}
			}
		}
	}

	if (wait)
	{
		std::lock_guard<std::mutex> lk(list_lock);
		waiting_jobs.emplace_back(job);
	}
	else
	{
		
		task_queue.Emplace(job);
		{
			std::lock_guard<std::mutex> lock(jobs_lock);
			num_of_jobs++;
		}
	}
}

void TaskManager::RegisterJob(Job * && job, bool wait, Job * parent_job)
{
	//_scheduler->CheckForJob(job);
	if (parent_job != nullptr)
		job->AddParent(parent_job);

	if (dictionary.find(job->j_type) != dictionary.end())
	{
		for (auto jobs : dictionary.find(job->j_type)->second)
		{
			for (auto waits : waiting_jobs)
			{
				if (waits->j_type == jobs)
				{
					job->AddParent(waits);
				}
			}
		}
	}

	if (wait)
	{
		std::lock_guard<std::mutex> lk(list_lock);
		waiting_jobs.emplace_back(job);
	}
	else
	{	
		task_queue.Emplace(job);
		{
			std::lock_guard<std::mutex> lock(jobs_lock);
			num_of_jobs++;
		}
	}
}

void TaskManager::RetryJob(Job * & job)
{
	//_scheduler->CheckForJob(job);
	task_queue.Emplace(job);
}

int TaskManager::ManageJobs()
{
	std::lock_guard<std::mutex> lk(list_lock);

	std::list<Job*>::iterator job_it = waiting_jobs.begin();
	while (job_it != waiting_jobs.end())
	{
		if ((*job_it)->_awaiting == 0)
		{
			task_queue.Emplace((*job_it));
			job_it = waiting_jobs.erase(job_it);
			{
				std::lock_guard<std::mutex> lock(jobs_lock);
				num_of_jobs++;
			}
			continue;
		}
		job_it++;
	}
	//_scheduler->SortJobs(job_list, _threadpool->GetQueue());
	int temp;
	{
		std::lock_guard<std::mutex> lock(jobs_lock);
		temp = num_of_jobs;
		num_of_jobs = 0;
	}
	return temp;
}
