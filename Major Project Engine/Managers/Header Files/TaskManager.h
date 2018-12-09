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

	void register_job(JobFunction function, const std::string name, void * content = nullptr, const JOB_TYPE type = JOB_TYPE::ANY_TYPE);

	void register_job(Job * job, bool wait = false);

	void register_job(Job * job, Job * parent_job);

	void transfer_jobs();

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
};

#endif // !_TASKMANAGER_H



