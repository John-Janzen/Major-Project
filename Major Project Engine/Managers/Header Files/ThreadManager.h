#pragma once

#ifndef _THREADMANAGER_H
#define _THREADMANAGER_H

#include "ThreadDebugger.h"
#include "SharedQueue.h"

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

	explicit ThreadManager(SharedQueue<Job*> & queue, const std::size_t & size);

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
	void AllocateJobs(const int num_new_jobs);

	/*
	Prints the stats for the jobs that the threads have
	completed. Along with the number of jobs in total
	that have been completed.
	*/
	void PrintJobs();

	void NewFrame();

	/*
	Stops the threads by calling there stop
	function. Had to make this because a there
	isn't an easy way to destruct a singleton.
	*/
	void StopThreads();

	void RetryJob(Job * & job)
	{
		task_queue.Emplace(job);
	}

	void NotifyDone()
	{
		std::lock_guard<std::mutex> lock(finished_job);
		jobs_to_finish--;
	}

	std::atomic<int> jobs_to_finish = 0;

	const std::size_t GetNumThreads() const { return num_of_threads; }

	/// DEBUGGING SECTION
	void LoadDebugger(const float & rate, const std::size_t & count) { t_debug.LoadDebug(rate, num_of_threads); }
	void ShowDebugger() { t_debug.ShowDebug(); }
	void RenderDebugger() { t_debug.RenderDebug(threads, t_framestart); }
	void HideDebugger() { t_debug.HideDebug(); }
	Uint32 GetDebugWindowID() { return t_debug.GetWindowID(); }
	void CheckDebugMouseLoc(const SDL_MouseMotionEvent & mme) { t_debug.CheckMouseLocation(mme); }

private:

	ThreadDebugger t_debug;

	std::array<Thread*, Thread::MAX_THREADS> threads = { nullptr };
	std::array<BlockingQueue<Job*>*, Thread::MAX_THREADS> t_queues = { nullptr };
	std::size_t rthread_num;

	std::condition_variable any_thread, render_thread;

	SharedQueue<Job*> & task_queue;
	std::size_t num_of_threads;
	std::mutex finished_job;

	std::chrono::high_resolution_clock::time_point t_framestart;
	
	int count = 1;
};

#endif // !_THREADMANAGER_H
