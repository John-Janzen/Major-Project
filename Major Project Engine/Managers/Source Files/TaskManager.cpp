#include "TaskManager.h"

TaskManager::~TaskManager() {}

void TaskManager::Init(const std::size_t & thread_size)
{
	_scheduler = new Scheduler(thread_size);
	_threadpool = new ThreadManager(thread_size);
	job_list = std::list<Job*>();
	waiting_jobs = std::list<Job*>();
}

void TaskManager::SetTimeLock(const float & time_lock)
{
	_scheduler->SetTimeLock(time_lock);
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


	_threadpool->Close();

	if (_scheduler != nullptr) delete _scheduler;
	if (_threadpool != nullptr) delete _threadpool;
}

bool TaskManager::FrameStart()
{
	_scheduler->StartFrame();
	if (jobs_to_finish == 0 && num_of_jobs == 0)
		return true;
	for (auto job : job_list)
	{
		job->_age++;
		_scheduler->SetScale(job);
	}
	return false;
}

bool TaskManager::HasJobs()
{
	std::lock_guard<std::mutex> lk(safety_lock);
	return (!waiting_jobs.empty() || !job_list.empty());
}

void TaskManager::NotifyDone()
{
	jobs_to_finish--;
}

void TaskManager::RegisterJob(JobFunction function, const std::string name, void* content, const job::JOB_ID type)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	Job * job = new Job(function, name, content, type);
	_scheduler->CheckForJob(job);
	job_list.emplace_back(job);
	num_of_jobs++;
}

void TaskManager::RegisterJob(Job * job, bool wait)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	_scheduler->CheckForJob(job);
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
	_scheduler->CheckForJob(job);
	parent_job->_awaiting++;
	job->_parent_job = parent_job;
	job_list.emplace_back(job);
	num_of_jobs++;
}

void TaskManager::RetryJob(Job * job)
{
	std::lock_guard<std::mutex> lk(safety_lock);
	_scheduler->CheckForJob(job);
	job_list.emplace_back(job);
}

void TaskManager::ThreadPoolAlloc()
{
	_threadpool->AllocateJobs();
}

void TaskManager::ManageJobs()
{
	std::lock_guard<std::mutex> lk(safety_lock);
	if (waiting_jobs.empty() && job_list.empty())
		return;

	std::list<Job*>::iterator job_it = waiting_jobs.begin();
	while (job_it != waiting_jobs.end())
	{
		if ((*job_it)->_awaiting == 0)
		{
			job_list.emplace_back((*job_it));
			job_it = waiting_jobs.erase(job_it);
			num_of_jobs++;
			continue;
		}
		job_it++;
	}
	_scheduler->SortJobs(job_list, _threadpool->GetQueue());
	jobs_to_finish += num_of_jobs;
	num_of_jobs = 0;
}

std::list<Job*>& TaskManager::GetJobList()
{
	return job_list;
}
