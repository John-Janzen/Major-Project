#pragma once

#ifndef _TASKMANAGER_H
#define _TASKMANAGER_H

#include "Scheduler.h"

#include <list>
#include <atomic>
#include <mutex>

class TaskManager
{
public:
	TaskManager(const std::size_t & thread_size);

	~TaskManager();

	void SetTimeLock(const float & time_lock);

	void Close();

	bool FrameStart();

	bool HasJobs();

	void NotifyDone();

	const std::atomic<int> & GetJobsToFinish() const { return jobs_to_finish; }

	void RegisterJob(JobFunction function, const std::string name, void * content = nullptr, const Job::JOB_ID type = Job::JOB_DEFAULT);

	void RegisterJob(Job * & job, bool wait = false, Job * parent_job = nullptr);

	void RegisterJob(Job * && job, bool wait = false, Job * parent_job = nullptr);

	void RetryJob(Job * & job);

	void ManageJobs();

	std::priority_queue<Job*, std::vector<Job*>, Job> & GetJobList() { return task_queue; }

private:

	//Scheduler * _scheduler;

	// List of jobs available
	std::priority_queue<Job*, std::vector<Job*>, Job> task_queue;

	// List of jobs waiting
	std::list<Job*> waiting_jobs;
	std::size_t num_of_wait = 0;

	std::atomic<int> num_of_jobs = 0;
	std::atomic<int> jobs_to_finish = 0;
	std::mutex safety_lock;
};

#endif // !_TASKMANAGER_H



