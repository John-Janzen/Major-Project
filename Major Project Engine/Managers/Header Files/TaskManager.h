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

	/*
	Get Event that came in from Event Handler
	*/
	void HandleEvent(const EventType & e, void * data);

	/*
	Set the time lock for scheduler and myself
	*/
	void SetTimeLock(const float & time_limit);

	/*
	Close the Task Manager
	*/
	void Close();

	/*
	Check to see if I have any jobs
	*/
	bool HasJobs();

	/*
	Place new job on queue
	*/
	void RegisterJob(JobFunction function, const std::string name, void * content = nullptr, const job::JOB_ID type = job::JOB_DEFAULT);

	/*
	Place new job on queue
	*/
	void RegisterJob(Job * & job, bool wait = false, Job * parent_job = nullptr);

	/*
	Place new job on queue
	*/
	void RegisterJob(Job * && job, bool wait = false, Job * parent_job = nullptr);

	/*
	Place new job on queue for the main thread
	*/
	void MainThreadJob(Job *&& job);

	/*
	Deal with the jobs that I currently have
	Waiting jobs mostly.
	*/
	int ManageJobs();

	/*
	Get the shared queue for the thread manager by reference
	*/
	SharedQueue<Job*> & GetJobList() { return task_queue; }

	// Map of jobs that need to wait on other jobs
	std::map<job::JOB_ID, std::vector<job::JOB_ID>> dictionary;

private:

	Scheduler _scheduler;	// For scheduling jobs

	// List of jobs available
	SharedQueue<Job*> task_queue;

	// List of jobs waiting
	std::list<Job*> waiting_jobs;
	std::size_t num_of_wait = 0;

	std::atomic<int> num_of_jobs = 0;
	std::mutex list_lock, jobs_lock;
};

#endif // !_TASKMANAGER_H



