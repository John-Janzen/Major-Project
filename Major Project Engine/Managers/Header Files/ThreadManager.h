#pragma once

#ifndef _THREADMANAGER_H
#define _THREADMANAGER_H

#include "Thread.h"
#include "BlockingQueue.h"

#include <memory>
#include <vector>

/*
This Manager class is created as a singleton because
we don't need multiple instances of the thread manager.
A single class will do as most programs will only engage
with this class by calling register_job.

Furthermore, main thread is going to be the only one to
use most of these functions.
*/
class System;
class ThreadManager
{
public:

	static const std::size_t MAX_THREADS = 8;

	static ThreadManager& Instance()
	{
		static ThreadManager inst;
		return inst;
	}

	/*
	Deletes the threads and job list.
	*/
	~ThreadManager();

	void Init(const std::size_t & size);

	/*
	Stops the threads.
	*/
	void Close();

	/*
	This function allocates jobs from the threads that are readily available.
	As long as there are jobs to allocate to the threads it will check
	all of the threads available to make sure they are not busy
	before handing a job off the queue.
	*/
	void allocate_jobs();

	/*
	To register a job in the manager any class can call this function
	with the job type and function.
	*/
	void register_job(JobFunction function, void* content = nullptr, const JOB_TYPE j_type = JOB_TYPE::ANY_TYPE);

	/*
	Register a job that the someone already made with the
	make_unique function.
	*/
	void register_job(Job * job);

	void register_job(Job * job, Job * parent_job);

	/*
	Check if there is no jobs in the queue.
	*/
	bool jobs_empty();

	/*
	Check if the queue is full. (20 is an
	arbitrary number for testing purposes.
	*/
	bool jobs_full();

	/*
	Prints the stats for the jobs that the threads have
	completed. Along with the number of jobs in total
	that have been completed.
	*/
	void print_total_jobs();

	/*
	Stops the threads by calling there stop
	function. Had to make this because a there
	isn't an easy way to destruct a singleton.
	*/
	void stop_threads();

	std::size_t get_num_threads() { return num_of_threads; }

private:
	Thread * threads[MAX_THREADS];
	ThreadManager() {}

	Thread * io_thread = nullptr;

	std::size_t num_of_threads;

	// List of jobs available
	// std::queue<std::unique_ptr<Job>> job_list;
	BlockingQueue<Job*> * job_list;
};

#endif // !_THREADMANAGER_H
