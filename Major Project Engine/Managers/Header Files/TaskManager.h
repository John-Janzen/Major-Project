#pragma once

#ifndef _TASKMANAGER_H
#define _TASKMANAGER_H

#include "ThreadManager.h"
#include "BlockingQueue.h"

#include <list>
#include <atomic>

class TaskManager
{
public:
	~TaskManager();

	static TaskManager& Instance()
	{
		static TaskManager inst;
		return inst;
	}

	void Init(ThreadManager * t_manager);

	void Close();

	bool FrameStart();

	bool HasJobs();

	void NotifyDone();

	void RegisterJob(JobFunction function, const std::string name, void * content = nullptr, const Job::JOB_TYPE type = Job::ANY_TYPE);

	void RegisterJob(Job * job, bool wait = false);

	void RegisterJob(Job * job, Job * parent_job);

	void RetryJob(Job * job);

	void TransferJobs();

private:

	//static const std::size_t MAX_JOBS = 30;

	TaskManager() {}
	ThreadManager * _threadpool_p;

	// List of jobs available
	std::list<Job*> job_list;

	// List of jobs waiting
	std::list<Job*> waiting_jobs;
	std::size_t num_of_wait = 0;

	std::atomic<int> num_of_jobs = 0;
	std::atomic<int> jobs_to_finish;
	std::mutex safety_lock;
};

#endif // !_TASKMANAGER_H



