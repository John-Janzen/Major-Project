#pragma once

#ifndef _TASKMANAGER_H
#define _TASKMANAGER_H

#include "Scheduler.h"
#include "SharedQueue.h"
#include "EventHandler.h"

#include <list>
#include <atomic>
#include <mutex>
#include <map>
#include <iostream>

class TaskManager : public EventListener
{
public:
	TaskManager(const std::size_t & n_threads);

	~TaskManager();

	void HandleEvent(const EventType & e, void * data);

	void SetTimeLock(const float & time_limit);

	void Close();

	bool HasJobs();

	void RegisterJob(JobFunction function, const std::string name, void * content = nullptr, const job::JOB_ID type = job::JOB_DEFAULT);

	void RegisterJob(Job * & job, bool wait = false, Job * parent_job = nullptr);

	void MainThreadJob(Job *&& job);

	void RegisterJob(Job * && job, bool wait = false, Job * parent_job = nullptr);

	int ManageJobs();

	SharedQueue<Job*> & GetJobList() { return task_queue; }

	// Map of jobs that need to wait on other jobs
	std::map<job::JOB_ID, std::vector<job::JOB_ID>> dictionary;

private:

	Scheduler _scheduler;

	// List of jobs available
	SharedQueue<Job*> task_queue;

	// List of jobs waiting
	std::list<Job*> waiting_jobs;
	std::size_t num_of_wait = 0;

	std::atomic<int> num_of_jobs = 0;
	std::mutex list_lock, jobs_lock;
};

#endif // !_TASKMANAGER_H



