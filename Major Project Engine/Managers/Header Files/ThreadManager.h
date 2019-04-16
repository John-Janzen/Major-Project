#pragma once

#ifndef _THREADMANAGER_H
#define _THREADMANAGER_H

#include "Thread.h"

#include <queue>
#include <array>

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

	explicit ThreadManager(std::priority_queue<Job*, std::vector<Job*>, Job> & queue, const std::size_t & size);

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

	std::size_t GetNumThreads() { return num_of_threads; }

private:
	std::array<Thread*, MAX_THREADS> threads = { nullptr };
	std::array<BlockingQueue<Job*>*, MAX_THREADS> t_queues = { nullptr };
	int rthread_num;

	std::condition_variable any_thread, render_thread;

	std::priority_queue<Job*, std::vector<Job*>, Job> & task_queue;
	std::size_t num_of_threads;

	int count = 0;
};

#endif // !_THREADMANAGER_H
