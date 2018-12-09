#include "TaskManager.h"

TaskManager::~TaskManager() {}

void TaskManager::Init(ThreadManager * t_manager)
{
	_threadpool_p = t_manager;
}

void TaskManager::Close()
{

}

bool TaskManager::frame_start()
{
	this->transfer_jobs();
	if (jobs_to_finish == 0 && num_of_jobs == 0)
		return true;
	return false;
}

void TaskManager::notify_done()
{
	jobs_to_finish--;
}

void TaskManager::register_job(JobFunction function, const std::string name, void* content, const JOB_TYPE type)
{
	job_list.emplace_back(new Job(function, name, content, type));
	num_of_jobs++;
}

void TaskManager::register_job(Job * job, bool wait)
{
	if (wait)
	{
		waiting_jobs.emplace_back(job);
		job = nullptr;
	}
	else
	{
		job_list.emplace_back(job);
		job = nullptr;
		num_of_jobs++;
	}
}

void TaskManager::register_job(Job * job, Job * parent_job)
{
	parent_job->increment_wait();
	job->set_parent(parent_job);
	job_list.emplace_back(job);
	job = nullptr;
	num_of_jobs++;
}

void TaskManager::transfer_jobs()
{
	std::list<Job*>::iterator job_it = waiting_jobs.begin();
	while (job_it != waiting_jobs.end())
	{
		if ((*job_it)->get_waiting() == 0)
		{
			job_list.emplace_back((*job_it));
			job_it = waiting_jobs.erase(job_it);
			continue;
		}
		job_it++;
	}
	_threadpool_p->get_jobs(&job_list);
	job_list.clear();
	jobs_to_finish += num_of_jobs;
	num_of_jobs = 0;
}
