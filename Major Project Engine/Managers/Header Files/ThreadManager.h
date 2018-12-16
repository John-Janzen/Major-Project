#pragma once

#ifndef _THREADMANAGER_H
#define _THREADMANAGER_H

#include "Thread.h"

#include <queue>
#include <list>

/*
This Manager class is created as a singleton because
we don't need multiple instances of the thread manager.
A single class will do as most programs will only engage
with this class by calling RegisterJob.

Furthermore, main thread is going to be the only one to
use most of these functions.
*/
class ThreadManager
{
public:

	static const std::size_t MAX_THREADS = 8;

	explicit ThreadManager(const std::size_t & size);

	/*
	Deletes the threads and job list.
	*/
	~ThreadManager();

	/*
	Stops the threads.
	*/
	void Close();

	bool HasJobs();

	/*
	This function allocates jobs from the threads that are readily available.
	As long as there are jobs to allocate to the threads it will check
	all of the threads available to make sure they are not busy
	before handing a job off the queue.
	*/
	void AllocateJobs();

	/*
	Check if there is no jobs in the queue.
	*/
	bool JobsEmpty();

	/*
	Prints the stats for the jobs that the threads have
	completed. Along with the number of jobs in total
	that have been completed.
	*/
	void PrintJobs();

	/*
	Stops the threads by calling there stop
	function. Had to make this because a there
	isn't an easy way to destruct a singleton.
	*/
	void StopThreads();

	void GetJobs(std::list<Job*> * job_list);

	std::size_t GetNumThreads() { return num_of_threads; }

private:
	Thread * threads[MAX_THREADS];
	std::queue<Thread*> thread_queue;
	Thread * render_thread;

	std::queue<Job*> task_queue;
	std::size_t num_of_threads;
};

#endif // !_THREADMANAGER_H
