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

	bool frame_start();

	void notify_done();

	void register_job(JobFunction function, void * content = nullptr, const JOB_TYPE type = JOB_TYPE::ANY_TYPE);

	void register_job(Job * job);

	void register_job(Job * job, Job * parent_job);

	void transfer_jobs();

private:

	TaskManager() {}
	ThreadManager * _threadpool_p;
	// List of jobs available
	// std::queue<std::unique_ptr<Job>> job_list;
	std::list<Job*> job_list;
	std::atomic<int> num_of_jobs = 0;
	std::atomic<int> jobs_to_finish;
};

#endif // !_TASKMANAGER_H



