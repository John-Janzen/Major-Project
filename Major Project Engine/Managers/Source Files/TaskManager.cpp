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

void TaskManager::register_job(JobFunction function, void* content, const JOB_TYPE type)
{
	job_list.emplace_back(new Job(function, content, type));
	num_of_jobs++;
}

void TaskManager::register_job(Job * job)
{
	job_list.emplace_back(job);
	job = nullptr;
	num_of_jobs++;
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
	_threadpool_p->get_jobs(&job_list);
	job_list.clear();
	jobs_to_finish += num_of_jobs;
	num_of_jobs = 0;
}
