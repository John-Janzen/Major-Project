#pragma once

#ifndef _TASKMANAGER_H
#define _TASKMANAGER_H

#include "Scheduler.h"
#include "SharedQueue.h"

#include <list>
#include <atomic>
#include <mutex>
#include <map>

class TaskManager
{
public:
	TaskManager(const std::size_t & thread_size);

	~TaskManager();

	void SetTimeLock(const float & time_lock);

	void Close();

	bool HasJobs();

	void RegisterJob(JobFunction function, const std::string name, void * content = nullptr, const Job::JOB_ID type = Job::JOB_DEFAULT);

	void RegisterJob(Job * & job, bool wait = false, Job * parent_job = nullptr);

	void RegisterJob(Job * && job, bool wait = false, Job * parent_job = nullptr);

	int ManageJobs();

	SharedQueue<Job*> & GetJobList() { return task_queue; }

	// Map of jobs that need to wait on other jobs
	std::map<Job::JOB_ID, std::vector<Job::JOB_ID>> dictionary;

private:

	//Scheduler * _scheduler;

	// List of jobs available
	SharedQueue<Job*> task_queue;

	// List of jobs waiting
	std::list<Job*> waiting_jobs;
	std::size_t num_of_wait = 0;

	std::atomic<int> num_of_jobs = 0;
	std::mutex list_lock, jobs_lock;
};

#endif // !_TASKMANAGER_H



