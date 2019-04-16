#include "TaskManager.h"

TaskManager::TaskManager(const std::size_t & thread_size) 
{}

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

bool TaskManager::FrameStart()
{
	//_scheduler->StartFrame();
	if (jobs_to_finish == 0 && num_of_jobs == 0)
		return true;
	return false;
}

bool TaskManager::HasJobs()
{
	std::lock_guard<std::mutex> lk(safety_lock);
	return (!waiting_jobs.empty() || !task_queue.empty());
}

void TaskManager::NotifyDone()
{
	jobs_to_finish--;
}

void TaskManager::RegisterJob(JobFunction function, const std::string name, void* content, const Job::JOB_ID type)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	Job * job = new Job(function, name, content, type);
	//_scheduler->CheckForJob(job);
	task_queue.emplace(job);
	num_of_jobs++;
}

void TaskManager::RegisterJob(Job * & job, bool wait, Job * parent_job)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	//_scheduler->CheckForJob(job);
	if (parent_job != nullptr)
	{
		parent_job->_awaiting++;
		job->_parent_job = parent_job;
	}

	if (wait)
	{
		waiting_jobs.emplace_back(job);
	}
	else
	{
		task_queue.emplace(job);
		num_of_jobs++;
	}
}

void TaskManager::RegisterJob(Job * && job, bool wait, Job * parent_job)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	//_scheduler->CheckForJob(job);
	if (parent_job != nullptr)
	{
		parent_job->_awaiting++;
		job->_parent_job = parent_job;
	}

	if (wait)
	{
		waiting_jobs.emplace_back(job);
	}
	else
	{	
		task_queue.emplace(job);
		num_of_jobs++;
	}
}

void TaskManager::RetryJob(Job * & job)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	//_scheduler->CheckForJob(job);
	task_queue.emplace(job);
}

void TaskManager::ManageJobs()
{
	std::lock_guard<std::mutex> lk(safety_lock);
	if (waiting_jobs.empty() && task_queue.empty())
		return;

	std::list<Job*>::iterator job_it = waiting_jobs.begin();
	while (job_it != waiting_jobs.end())
	{
		if ((*job_it)->_awaiting == 0)
		{
			task_queue.emplace((*job_it));
			job_it = waiting_jobs.erase(job_it);
			num_of_jobs++;
			continue;
		}
		job_it++;
	}
	//_scheduler->SortJobs(job_list, _threadpool->GetQueue());
	//jobs_to_finish += num_of_jobs;
	num_of_jobs = 0;
}
